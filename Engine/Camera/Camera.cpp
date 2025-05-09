#include"Camera.h"
#include"WinApp/WinApp.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"
#include<imgui.h>
#include<numbers>


Camera* Camera::GetInstance()
{
	//インスタンス取得
	static Camera instance;
	return &instance;
}



void Camera::Initialize() {

	//メインカメラ行列初期化
	mainCamera_.Initialize();

	//ポイントカメラ用行列初期化
	CameraMotionSupport_.Initialize();

	//追従ワールド初期化
	FeaturedWorldTransform_ = nullptr;

	//ポイントカメラの回転の設定
	CameraMotionSupport_.rotate_ = { 0.6f,0.0f,0.0f };

	//親子関係の処理
	mainCamera_.parent_ = (&CameraMotionSupport_);

	//行列更新
	CameraMotionSupport_.UpdateMatrix();
	mainCamera_.UpdateMatrix();

	//各種更新に必要な処理
	view_ = Inverse(mainCamera_.matWorld_);
	//プロジェクション生成
	projection_ = MakePerspectiveFovMatrix(0.45f, (float)WindowApp::kClientWidth / (float)WindowApp::kClientHeight, 0.1f, FarZ);
	//ビュープロジェクション行列生成
	viewProjection_ = view_ * projection_;

	//ツリー作成
	tree_.name_ = "カメラ";
	tree_.SetValue("カメラのZの値", &farFeaturedPos_);
	tree_.SetValue("カメラ位置", &mainCamera_.translate_);
	tree_.SetValue("カメラ回転", &mainCamera_.rotate_);
	tree_.SetValue("カメラ注目点", &CameraMotionSupport_.translate_);
	tree_.SetValue("カメラ注目点の回転", &CameraMotionSupport_.rotate_);


}

void Camera::Update() {

	//カメラの当たり判定無効化
	isCollision_ = false;

	//カメラ距離を一旦再設定
	mainCamera_.translate_.z = farFeaturedPos_;

	////回転量Xの制限
	if (CameraMotionSupport_.rotate_.x < minRotateX) {
		CameraMotionSupport_.rotate_.x = minRotateX;
	}
	else if (CameraMotionSupport_.rotate_.x > maxRotateX) {
		CameraMotionSupport_.rotate_.x = maxRotateX;
	}

	//一週以上している場合制限
	if (CameraMotionSupport_.rotate_.y > (float)std::numbers::pi * 2.0f) {
		CameraMotionSupport_.rotate_.y -= (float)std::numbers::pi * 2.0f;
	}
	if (CameraMotionSupport_.rotate_.y < -(float)std::numbers::pi * 2.0f) {
		CameraMotionSupport_.rotate_.y += (float)std::numbers::pi * 2.0f;
	}

	//注目するものがあるとき
	if (FeaturedWorldTransform_) {
		//座標のみ取得するフラグが起動しているとき
		if (isOnlyGetPosition) {
			//カメラ座標にポイントカメラ用基点の座標を足す
			CameraMotionSupport_.translate_ = camerapos_ + FeaturedWorldTransform_->GetWorldTranslate();
			//行列更新
			CameraMotionSupport_.UpdateMatrix();
		}
		else {
			//フラグが機能していないとき
			//カメラ座標をそのまま渡す
			CameraMotionSupport_.translate_ = camerapos_;
			//行列更新
			CameraMotionSupport_.UpdateMatrix();
			//起動していないとき行列をかけて親子関係処理
			CameraMotionSupport_.matWorld_ = CameraMotionSupport_.matWorld_ * FeaturedWorldTransform_->matWorld_;
		}
	}

	//メインカメラの更新
	mainCamera_.UpdateMatrix();

	//各種更新に必要な処理
	view_ = Inverse(mainCamera_.matWorld_);
	//プロジェクション更新
	projection_ = MakePerspectiveFovMatrix(0.45f, (float)WindowApp::kClientWidth / (float)WindowApp::kClientHeight, 0.1f, FarZ);
	//ビュープロジェクション更新
	viewProjection_ = view_ * projection_;
	//ビューポート更新
	viewPort_ = MakeViewPortMatrix(0, 0, (float)WindowApp::kClientWidth, (float)WindowApp::kClientHeight, 0, 1);

#pragma region Segment設定
	//カメラ位置計算
	segment_.origin = mainCamera_.GetWorldTranslate();
	//距離計算
	segment_.diff = TransformNormal({ 0,0,1 }, mainCamera_.matWorld_);
	segment_.diff.SetNormalize();
	segment_.diff *= mainCamera_.translate_.z * -1.0f;
#pragma endregion


}

void Camera::UpdateMatrixes() {
	//行列更新
	CameraMotionSupport_.UpdateMatrix();
	mainCamera_.UpdateMatrix();

	//各種更新に必要な処理
	//ビュー更新
	view_ = Inverse(mainCamera_.matWorld_);
	//プロジェクション更新
	projection_ = MakePerspectiveFovMatrix(0.45f, (float)WindowApp::kClientWidth / (float)WindowApp::kClientHeight, 0.1f, FarZ);
	//ビュープロジェクション更新
	viewProjection_ = view_ * projection_;
}

void Camera::IsCollision(OBBCollider* obb)
{


	//カメラのレイとの処理
	//t定義
	float t = 0;
	//分割量
	float division = farFeaturedPos_;

	//注目点
	Vector3 featuredPos = GetFeaturedWorld().GetWorldTranslate();

	while (t <= 1.0f) {
		//Zが設定値のmatWorldを生成
		EulerWorldTransform world = mainCamera_;
		world.translate_.z = farFeaturedPos_;
		world.UpdateMatrix();
		//カメラ位置
		Vector3 cameraPos = world.GetWorldTranslate();
		//距離だけは設定場所に


		//目標地点からカメラまで円をイージング
		Vector3 pos = {
			cameraPos.x * (t)+featuredPos.x * (1.0f - t),
			cameraPos.y * (t)+featuredPos.y * (1.0f - t),
			cameraPos.z * (t)+featuredPos.z * (1.0f - t),
		};

		//円コライダーを仮生成
		SphereCollider* sphere = new SphereCollider();
		sphere->Initialize("sphere");
		sphere->SetTranslate(pos);
		sphere->SetRadius(0.1f);
		sphere->Update();

		//あたったかチェック
		Vector3 temp;
		if (sphere->IsCollision(*obb, temp, 1)) {
			///あたったら再配置
			//注目点とあたった点との距離計算
			float length = Length(featuredPos - pos);
			if (mainCamera_.translate_.z < -length + direction) {
				mainCamera_.translate_.z = -length + direction;
				UpdateMatrixes();
			}
			//obb->SetColor(true);
			isCollision_ = true;
			//終了
			return;
		}
		else {
			//処理が行われていない場合
			if (!isCollision_) {
				mainCamera_.translate_.z = farFeaturedPos_;
			}
			//obb->SetColor(false);
		}

		t += 1.0f / -division;

		delete sphere;
	}
}

void Camera::SetCameraDirection(const float farZ) {
	//値を保存
	farFeaturedPos_ = farZ;
	//値を適応
	mainCamera_.translate_.z = farZ;
}

Vector3 Camera::SetDirection4Camera(const Vector3& velo)const
{
	//引数のベクトルをカメラから見た方向に傾ける
	Vector3 ans = TransformNormal(velo, mainCamera_.matWorld_);
	return ans;
}
