#pragma once
#include <Windows.h>
#include "Math.h"

/// <summary>
/// デバッグカメラ
/// </summary>
class DebugCamera {
public:

	void Initialize();

	void Update();

	const Matrix4x4& GetViewMatrix() const { return matView_; }
	const Matrix4x4& GetProjectionMatrix() const { return matProjection_; }

private:

	//X,Y,Z軸周りのローカル回転角
	Vector3 rotation_ = { 0.0f, 0.0f, 0.0f };

	//ローカル座標
	Vector3 translation_ = { 0, 0, -50 };

	Matrix4x4 matRot_ = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	//ワールド行列
	Matrix4x4 matWorld_{};

	// ビュー行列
	Matrix4x4 matView_{};

	// 射影行列
	Matrix4x4 matProjection_{};

	//ViewProjection行列
	Matrix4x4 viewProjectionMatrix_{};

};

