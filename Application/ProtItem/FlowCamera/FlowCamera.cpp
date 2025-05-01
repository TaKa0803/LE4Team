#include "FlowCamera.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"

FlowCamera::FlowCamera(const EulerWorldTransform* world)
{
	//カメラのインスタンス取得
	camera_ = Camera::GetInstance();
	camera_->Initialize();
	//カメラのワールドポインタをセット
	camera_->SetTarget(world);

	std::unique_ptr<GVariGroup>gvg = std::make_unique<GVariGroup>("FlowCamera");
	gvg->SetTreeData(camera_->GetDebugTree());
	gvg->SetValue("カメラ角度", &rotate_);
	gvg->SetValue("カメラの距離", &farTarget_);
	
}

void FlowCamera::Init()
{
	camera_->SetCameraDirection(farTarget_);
	camera_->SetFocusPointRotate(rotate_);
}

void FlowCamera::Update()
{
#ifdef _DEBUG
	//回転量を渡す
	camera_->SetCameraDirection(farTarget_);
	camera_->SetFocusPointRotate(rotate_);
#endif // _DEBUG

	camera_->Update();
}
