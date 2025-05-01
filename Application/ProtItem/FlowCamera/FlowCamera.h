#pragma once
#include"Camera/Camera.h"

class FlowCamera {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	FlowCamera(const EulerWorldTransform*world);
	~FlowCamera()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private://**パラメータ変数**//

	//カメラポインタ
	Camera* camera_ = nullptr;

	//カメラの距離
	float farTarget_ = -50.0f;
	
	//カメラ角度
	Vector3 rotate_;
};