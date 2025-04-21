#include "ProtPlayer.h"
#include"DeltaTimer/DeltaTimer.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"

#pragma region 状態クラス
#include"ProtItem/Player/behavior/Move/ProtPlayerMove.h"
#include"ProtItem/Player/behavior/Roll/ProtPlayerRoll.h"
#pragma endregion

#include<numbers>

ProtPlayer::ProtPlayer()
{
	//オブジェクト生成
	GameObject::Initialize("Player");

	//入力クラス生成
	input_ = std::make_unique<ProtPlayerInput>();

	//コライダークラス生成
	collider_ = std::make_unique<SphereCollider>();
	//初期化
	collider_->Initialize("プレイヤー", world_);

	//プレイヤーポインタ設定
	IProtBehavior::SetPlayer(this);

	//状態の数指定
	behaviors_.resize((size_t)Behavior::Count);

	//生成
	behaviors_[(size_t)Behavior::Move] = std::make_unique<ProtPlayerMove>();
	behaviors_[(size_t)Behavior::Roll] = std::make_unique<ProtPlayerRoll>();


	std::unique_ptr<GVariGroup>gvg = std::make_unique<GVariGroup>("Player");
	gvg->SetMonitorValue("ヒットフラグ", &parameters_.isHit);
	gvg->SetMonitorValue("回避クールタイム", &parameters_.currentRollSec);
	gvg->SetValue("体力", &parameters_.hp);

	gvg->SetTreeData(collider_->GetDebugTree());

	//全ての状態のツリーをセット
	for (auto& behavior : behaviors_) {
		if (behavior) {
			gvg->SetTreeData(behavior->tree_);
		}
	}

}

void ProtPlayer::Update()
{
	//リクエストがある場合
	if (behaviorRequest_) {
		//リクエストの値を渡す
		behaviorName_ = behaviorRequest_.value();
		//リクエスト初期化
		behaviorRequest_ = std::nullopt;
		//状態初期化
		behaviors_[(int)behaviorName_]->Init();
	}

	//回避のクールタイム更新
	parameters_.currentRollSec -= (float)DeltaTimer::deltaTime_;

	//もし時間が0以下なら0に
	if (parameters_.currentRollSec < 0)parameters_.currentRollSec = 0;

	//状態更新
	behaviors_[(int)behaviorName_]->Update();

	//行列更新
	GameObject::GameUpdate();

	//コライダー更新
	collider_->Update();
}

void ProtPlayer::Draw()
{
	//描画
	GameObject::Draw();

	//コライダー描画
	collider_->Draw();
}

Vector3 ProtPlayer::SetBody2Input()
{
	//入力を取得
	Vector3 velocity = input_->GetMoveInput();

	//カメラ方向に向ける
	// 今はいらない（おそらくこの先も
	//velocity = TransformNormal(velocity, camera_->GetMainCamera().matWorld_);
	//ｙの量を無視する
	velocity.y = 0.0f;

	//正規化
	velocity.SetNormalize();

	//入力がある場合
	if (velocity != Vector3(0, 0, 0)) {
		//向きを指定
		world_.rotate_.y = GetYRotate({ velocity.x,velocity.z }) + ((float)std::numbers::pi);
	}

	return velocity;
}
