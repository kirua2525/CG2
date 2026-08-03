#include "DebugCamera.h"
#include <cmath>

//Initializeの関数定義
void DebugCamera::Initialize()
{
	//各種メンバ変数の初期化(必要に応じて)
	rotation_ = { 0.0f, 0.0f, 0.0f };
    translation_ = { 0.0f, 0.0f, -50.0f };

	// 射影行列の初期化
	matProjection_ = MakePerspectiveFovMatrix(0.45f, 1280.0f / 720.0f, 0.1f, 1000.0f);

}


//Updateの関数定義
void DebugCamera::Update()
{

    float deltaX = 0.0f;
    float deltaY = 0.0f;
	float deltaZ = 0.0f;

    //累計の回転行列を合成
    //matRot_ = matRotDelta * matRot_;
	//回転スピード
	const float rotSpeed = 0.02f;

	//入力によるカメラの移動や回転
	if (GetAsyncKeyState(VK_UP) & 0x8000/*x軸周り回転の入力があったら*/) {
		// x軸周りの角度を加算する
		deltaX += 0.01f;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000/*x軸周り回転の入力があったら（見下ろす)*/) {
		deltaX -= 0.01f;
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		deltaY += 0.01f;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		deltaY -= 0.01f;
	}

	if (GetAsyncKeyState('Q') & 0x8000) { 
		deltaZ += rotSpeed; 
	}
	if (GetAsyncKeyState('E') & 0x8000) { 
		deltaZ -= rotSpeed; 
	}


    //// Matrix4x4 matRot = MakeRotateMatrix(rotation_);
    //Matrix4x4 matRotDelta = MakeIdentityMatrix();
    //matRotDelta *= MakeRotateXMatrix(deltaX);
    //matRotDelta *= MakeRotateYMatrix(deltaY);

	// 今回の回転変化量からデルタ回転行列を作成
	Vector3 rotDelta = { deltaX, deltaY, deltaZ };
	Matrix4x4 matRotDelta = MakeRotateMatrix(rotDelta);

	// 累計の回転行列を合成
	matRot_ = Multiply(matRotDelta, matRot_);

	const float moveSpeed = 0.5f; // 移動速度
	Vector3 move = { 0.0f, 0.0f, 0.0f };

	// 入力によるカメラの移動や回転
	// 前進 / 後退 (W / S)
	if (GetAsyncKeyState('W') & 0x8000) { move.z += moveSpeed; }
	if (GetAsyncKeyState('S') & 0x8000) { move.z -= moveSpeed; }

	// 左右移動 (D / A)
	if (GetAsyncKeyState('D') & 0x8000) { move.x += moveSpeed; }
	if (GetAsyncKeyState('A') & 0x8000) { move.x -= moveSpeed; }

	// 上下移動 (R: 上昇 / F: 下降)
	if (GetAsyncKeyState('R') & 0x8000) { move.y += moveSpeed; }
	if (GetAsyncKeyState('F') & 0x8000) { move.y -= moveSpeed; }

	// カメラの回転に合わせた移動ベクトルに変換
	move = TransformNormal(move, matRot_);

	translation_.x += move.x;
	translation_.y += move.y;
	translation_.z += move.z;

	//ビュー行列の更新
	//角度から回転行列を計算する
	//matRot = MakeRotateMatrix(rotation_);
	//matRot = matRot_;

    //座標から平行移動を計算する
	Matrix4x4 matTrans = MakeTranslateMatrix(translation_);

    //累積回転行列と平行移動行列からワールド行列を計算する
	matWorld_ = Multiply(matRot_, matTrans);

    //ワールド行列の逆行列をビュー行列に代入する
	matView_ = Inverse(matWorld_);

	// ViewProjection行列の更新
	viewProjectionMatrix_ = Multiply(matView_, matProjection_);

	//回転行列と平行移動行列からワールド行列を計算する
    //Matrix4x4 matTrans = MakeTranslateMatrix(translation_);
   //matWorld_ = Multiply(matRot, matTrans);

	//ワールド行列の逆再生をビュー行列に代入する
    //viewMatrix_ = Inverse(matWorld_);

    // ViewProjection行列の更新
    //iewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);

}

