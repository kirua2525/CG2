#pragma once

struct Vector3 {

	float x, y, z;

};

struct Matrix4x4 {

	float m[4][4];

	// 単位行列を作成する静的関数
	static Matrix4x4 Identity() {
		Matrix4x4 result = {};
		result.m[0][0] = 1.0f; result.m[1][1] = 1.0f;
		result.m[2][2] = 1.0f; result.m[3][3] = 1.0f;
		return result;
	}
};

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 MakeTranslateMatrix(const Vector3& trans);
Matrix4x4 MakeRotateMatrix(const Vector3& rot);
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
Matrix4x4 QuickInverse(const Matrix4x4& m);
Matrix4x4 MakePerspectiveFovLH(float fovY, float aspectRatio, float nearZ, float farZ);

/// <summary>
/// デバッグカメラ
/// </summary>
class DebugCamera
{
public:

	void Initialize();

	void Update();

	const Matrix4x4& GetViewMatrix() const { return viewMatrix_; }
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix_; }
	const Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix_; }

private:

	//X,Y,Z軸周りのローカル回転角
	//Vector3 rotation_ = { 0, 0, 0 };

	//累計回転行列
	Matrix4x4 matRot_;

	//ローカル座標
	Vector3 translation_ = { 0, 0, -50 };

	// ワールド行列
	Matrix4x4 matWorld_;

	// ビュー行列
	Matrix4x4 viewMatrix_;

	// 射影行列
	Matrix4x4 projectionMatrix_;

	// ビュープロジェクション行列
	Matrix4x4 viewProjectionMatrix_;

};

