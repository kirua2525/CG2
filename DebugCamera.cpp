#include "DebugCamera.h"
#include <cmath>
#include <Windows.h>

//// 行列の乗算 (4x4 * 4x4)
inline Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
    Matrix4x4 result = {};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                result.m[i][j] += m1.m[i][k] * m2.m[k][j];
            }
        }
    }
    return result;
}

// 2. 平行移動行列の作成
Matrix4x4 MakeTranslateMatrix(const Vector3& trans) {
    Matrix4x4 result = Matrix4x4::Identity();
    result.m[3][0] = trans.x;
    result.m[3][1] = trans.y;
    result.m[3][2] = trans.z;
    return result;
}

// X, Y, Z軸の回転行列を合成して作成（RollPitchYaw）
inline Matrix4x4 MakeRotateMatrix(const Vector3& rot) {
    // X軸回転
    Matrix4x4 rotX = Matrix4x4::Identity();
    rotX.m[1][1] = std::cos(rot.x);  rotX.m[1][2] = std::sin(rot.x);
    rotX.m[2][1] = -std::sin(rot.x); rotX.m[2][2] = std::cos(rot.x);

    // Y軸回転
    Matrix4x4 rotY = Matrix4x4::Identity();
    rotY.m[0][0] = std::cos(rot.y);  rotY.m[0][2] = -std::sin(rot.y);
    rotY.m[2][0] = std::sin(rot.y);  rotY.m[2][2] = std::cos(rot.y);

    // Z軸回転
    Matrix4x4 rotZ = Matrix4x4::Identity();
    rotZ.m[0][0] = std::cos(rot.z);  rotZ.m[0][1] = std::sin(rot.z);
    rotZ.m[1][0] = -std::sin(rot.z); rotZ.m[1][1] = std::cos(rot.z);

    // Z * X * Y の順で合成
    return Multiply(Multiply(rotZ, rotX), rotY);
}

// 平行移動行列の作成
//inline Matrix4x4 MakeTranslateMatrix(const Vector3& trans) {
//    Matrix4x4 result = Matrix4x4::Identity();
//    result.m[3][0] = trans.x;
//    result.m[3][1] = trans.y;
//    result.m[3][2] = trans.z;
//    return result;
//}

// ベクトルを行列（回転部分のみ）で変換
inline Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
    Vector3 result;
    result.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0];
    result.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1];
    result.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2];
    return result;
}

// アフィン変換行列の逆行列（回転と平行移動のみの行列用）
inline Matrix4x4 QuickInverse(const Matrix4x4& m) {
    Matrix4x4 result = Matrix4x4::Identity();
    // 回転部分（3x3）の転置
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.m[i][j] = m.m[j][i];
        }
    }
    // 移動部分の計算
    Vector3 trans = { m.m[3][0], m.m[3][1], m.m[3][2] };
    Vector3 invTrans = TransformNormal({ -trans.x, -trans.y, -trans.z }, result);
    result.m[3][0] = invTrans.x;
    result.m[3][1] = invTrans.y;
    result.m[3][2] = invTrans.z;
    return result;
}

// 透視投影行列（左手系）の作成
inline Matrix4x4 MakePerspectiveFovLH(float fovY, float aspectRatio, float nearZ, float farZ) {
    Matrix4x4 result = {};
    float h = 1.0f / std::tan(fovY * 0.5f);
    float w = h / aspectRatio;

    result.m[0][0] = w;
    result.m[1][1] = h;
    result.m[2][2] = farZ / (farZ - nearZ);
    result.m[2][3] = 1.0f;
    result.m[3][2] = -nearZ * farZ / (farZ - nearZ);
    return result;
}


//Initializeの関数定義
void DebugCamera::Initialize()
{
	//各種メンバ変数の初期化(必要に応じて)
	//rotation_ = { 0.0f, 0.0f, 0.0f };
    translation_ = { 0.0f, 0.0f, -50.0f };

    float fovRad = 60.0f * (3.14159265f / 180.0f);
    projectionMatrix_ = MakePerspectiveFovLH(fovRad, 16.0f / 9.0f, 0.1f, 1000.0f);

}


//Updateの関数定義
void DebugCamera::Update()
{

    float deltaX = 0.0f;
    float deltaY = 0.0f;

    //累計の回転行列を合成
    //matRot_ = matRotDelta * matRot_;

	//入力によるカメラの移動や回転
	if (GetAsyncKeyState(VK_UP) & 0x8000/*x軸周り回転の入力があったら*/) {
		// x軸周りの角度を加算する
		deltaX -= 0.03f;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000/*x軸周り回転の入力があったら（見下ろす)*/) {
		deltaX += 0.03f;
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		deltaY -= 0.03f;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		deltaY += 0.03f;
	}

    //// Matrix4x4 matRot = MakeRotateMatrix(rotation_);
    //Matrix4x4 matRotDelta = MakeIdentityMatrix();
    //matRotDelta *= MakeRotateXMatrix(deltaX);
    //matRotDelta *= MakeRotateYMatrix(deltaY);

	// 今回の回転変化量からデルタ回転行列を作成
	Vector3 rotDelta = { deltaX, deltaY, 0.0f };
	Matrix4x4 matRotDelta = MakeRotateMatrix(rotDelta);

	// 累計の回転行列を合成
	matRot_ = Multiply(matRotDelta, matRot_);

	// ローカル参照用としてmatRotにセット
	Matrix4x4 matRot = matRot_;

	// 入力によるカメラの移動や回転
	if (GetAsyncKeyState('W') & 0x8000/*前移動の入力があったら*/) {

		const float speed = 0.5f;

		// カメラ移動ベクトル
		Vector3 move = { 0, 0, speed };
		// 移動ベクトルを角度分だけ回転させる
		move = TransformNormal(move, matRot);

		// 移動ベクトル分だけ座標を加算する
		translation_.x += move.x;
		translation_.y += move.y;
		translation_.z += move.z;

	}

	if (GetAsyncKeyState('D') & 0x8000/*右移動の入力があったら*/) {
		const float speed = 0.5f;

		// カメラ移動ベクトル
		Vector3 move = { speed, 0, 0 };

		// 移動ベクトルを角度分だけ回転させる
		move = TransformNormal(move, matRot);

		// 移動ベクトル分だけ座標を加算する
		translation_.x += move.x;
		translation_.y += move.y;
		translation_.z += move.z;

	}

	//ビュー行列の更新
	//角度から回転行列を計算する
	//matRot = MakeRotateMatrix(rotation_);
	matRot = matRot_;

    //座標から平行移動を計算する
	Matrix4x4 matTrans = MakeTranslateMatrix(translation_);

    //累積回転行列と平行移動行列からワールド行列を計算する
	matWorld_ = Multiply(matRot, matTrans);

    //ワールド行列の逆行列をビュー行列に代入する
	viewMatrix_ = QuickInverse(matWorld_);

	// ViewProjection行列の更新
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);

	//回転行列と平行移動行列からワールド行列を計算する
    //Matrix4x4 matTrans = MakeTranslateMatrix(translation_);
   //matWorld_ = Multiply(matRot, matTrans);

	//ワールド行列の逆再生をビュー行列に代入する
    //viewMatrix_ = QuickInverse(matWorld_);

    // ViewProjection行列の更新
    //iewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);

}

