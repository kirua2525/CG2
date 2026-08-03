#pragma once
#include <cmath>


struct Vector4 {

	float x, y, z, w;

};

struct Vector3 {

	float x, y, z;

};

struct Vector2 {

	float x, y;

};

struct Matrix4x4 {

	float m[4][4];

};

//4x4単位行列を作成する関数
inline Matrix4x4 MakeIdentity4x4() {
	return Matrix4x4{ {

		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}

		}
	};
}


inline Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result{};
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0.0f;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

inline Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {

	Matrix4x4 result = MakeIdentity4x4();
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	return result;

}

inline Matrix4x4 MakeRotateMatrix(const Vector3& rotate) {
	Matrix4x4 rotateX = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, std::cos(rotate.x), std::sin(rotate.x), 0.0f,
		0.0f, -std::sin(rotate.x), std::cos(rotate.x), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	Matrix4x4 rotateY = {
		std::cos(rotate.y), 0.0f, -std::sin(rotate.y), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		std::sin(rotate.y), 0.0f, std::cos(rotate.y), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	Matrix4x4 rotateZ = {
		std::cos(rotate.z), std::sin(rotate.z), 0.0f, 0.0f,
		-std::sin(rotate.z), std::cos(rotate.z), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	// Z -> X -> Y の順で回転行列を合成
	return Multiply(rotateX, Multiply(rotateY, rotateZ));
}

inline Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]
	};
	return result;
}



// 透視投影行列 (プロジェクション行列) を作成する関数
inline Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspect, float nearClip, float farClip) {
	Matrix4x4 result{};
	float cot = 1.0f / std::tan(fovY / 2.0f);

	result.m[0][0] = cot / aspect;
	result.m[1][1] = cot;
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1.0f;
	result.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);

	return result;
}

// 逆行列を計算する関数 (カメラのView行列変換用)
inline Matrix4x4 Inverse(const Matrix4x4& m) {
	// 回転部分 (左上3x3) の転置
	Matrix4x4 result = MakeIdentity4x4();
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			result.m[i][j] = m.m[j][i];
		}
	}
	// 平行移動部分の反転
	Vector3 translation = { m.m[3][0], m.m[3][1], m.m[3][2] };
	result.m[3][0] = -(translation.x * result.m[0][0] + translation.y * result.m[1][0] + translation.z * result.m[2][0]);
	result.m[3][1] = -(translation.x * result.m[0][1] + translation.y * result.m[1][1] + translation.z * result.m[2][1]);
	result.m[3][2] = -(translation.x * result.m[0][2] + translation.y * result.m[1][2] + translation.z * result.m[2][2]);

	return result;
}
