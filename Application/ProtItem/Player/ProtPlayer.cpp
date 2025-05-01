#include "ProtPlayer.h"
#include"DeltaTimer/DeltaTimer.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"

#pragma region 状態クラス
#include"ProtItem/Player/behavior/Move/ProtPlayerMove.h"
#include"ProtItem/Player/behavior/Roll/ProtPlayerRoll.h"
#pragma endregion

#include<numbers>

#include"InstancingModelManager/InstancingModelManager.h"

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
	//コライダーツリーセット
	gvg->SetTreeData(collider_->GetDebugTree());

	//全ての状態のツリーをセット
	for (auto& behavior : behaviors_) {
		if (behavior) {
			gvg->SetTreeData(behavior->tree_);
		}
	}

	//ヒット時の処理パラメータ設定
	GvariTree hitTree;
	hitTree.name_ = "ヒット時";
	hitTree.SetMonitorValue("ヒットフラグON", &parameters_.isHit);
	hitTree.SetValue("無敵時間", &maxHitSec_);
	hitTree.SetValue("点滅回数", &maxTenmetu_);
	gvg->SetTreeData(hitTree);
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

	//点滅更新
	Tenmetu();

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

void ProtPlayer::SetWorldTranslate(const Vector3& translate)
{
	world_.translate_ = translate;
	world_.UpdateMatrix();
}

void ProtPlayer::OnCollison()
{
	//ヒットフラグOFF
	parameters_.isHit = false;
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

void ProtPlayer::Tenmetu()
{
	if (!parameters_.isHit) {

		hitSec_ += (float)DeltaTimer::deltaTime_;

		//時間内での点滅処理
		if (hitSec_ >= (maxHitSec_/maxTenmetu_)*tenmetuCount_) {
			tenmetuCount_++;

			//透明度を変更
			if (model_->materialData_->color.w == 1.0f) {
				model_->materialData_->color.w = 0.1f;
			}
			else {
				model_->materialData_->color.w = 1.0f;
			}
		}
			
		//時間経過で終了
		if (hitSec_ >= maxHitSec_) {
			parameters_.isHit = true;
			model_->materialData_->color.w = 1.0f;

			//カウント初期化
			hitSec_ = 0;
			//点滅回数初期化
			tenmetuCount_ = 0;
		}
	}
}
