#include "Boss.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"
#include"DeltaTimer/DeltaTimer.h"

#pragma region 状態
#include"ProtItem/Boss/Behavior/Idle/BossIdle.h"
#include"ProtItem/Boss/Behavior/AreaAttack/BossAreaAttack.h"
#include"ProtItem/Boss/Behavior/WeaponRollAttack/BossWeaponRollAttack.h"
#pragma endregion


Boss::Boss()
{
	//オブジェクト生成
	GameObject::Initialize("Boss");

	IBossBehavior::SetBoss(this);

	behaviors_.resize((size_t)Behavior::Count);
	behaviors_[(size_t)Behavior::Idle] = std::make_unique<BossIdle>();
	behaviors_[(size_t)Behavior::Attack1] = std::make_unique<BossAreaAttack>();
	behaviors_[(size_t)Behavior::Attack2] = std::make_unique<BossWeaponRollAttack>();


	std::unique_ptr<GlobalVariableGroup> gvg = std::make_unique<GlobalVariableGroup>("Boss");
	gvg->SetMonitorValue("共通経過時間", &parameters_.currentSec);

	//デバッグ用指定
	gvg->SetMonitorCombo("行動指定", &debugBehavior_,behaviorNames_);


	gvg->SetTreeData(model_->SetDebugParam());
	for (auto& behavior : behaviors_) {
		if (!behavior)continue;
		gvg->SetTreeData(behavior->tree_);
	}

	GvariTree bulletTree;
	bulletTree.name_ = "落下弾関係";
	bulletTree.SetValue("生成高度", &bulletStartHeight_);
	bulletTree.SetValue("落下速度", &fallSpeed_);
	bulletTree.SetValue("半径", &radius_);
	gvg->SetTreeData(bulletTree);
}

void Boss::Init()
{
}

void Boss::Update()
{



	//リクエストがある場合
	if (behaviorRequest_) {

#ifdef _DEBUG
		//デバッグ時の攻撃指定
		if (debugBehavior_ == behaviorNames_[0]) {
			//何もなし
		}
		else if (debugBehavior_ == behaviorNames_[1]) {
			//待機
			behaviorRequest_ = Behavior::Idle;
		}
		else if (debugBehavior_ == behaviorNames_[2]) {
			//攻撃1
			behaviorRequest_ = Behavior::Attack1;
		}
		else {
			//攻撃2
			behaviorRequest_ = Behavior::Attack2;
		}
#endif // _DEBUG

		//リクエストの値を渡す
		behavior_ = behaviorRequest_.value();
		//リクエスト初期化
		behaviorRequest_ = std::nullopt;
		parameters_.currentSec = 0;
		//状態初期化
		behaviors_[(int)behavior_]->Init();
	}

	parameters_.currentSec += (float)DeltaTimer::deltaTime_;

	//状態更新
	behaviors_[(int)behavior_]->Update();

	//行列更新
	GameObject::GameUpdate();

	//弾の更新
	for (auto& bullet : bullets_) {
		bullet->Update();
	}

	//不要な球の削除
	bullets_.remove_if([](auto& bullet) {
		if (bullet->GetDead()) {
			return true;
		}
		else {
			return false;
		}});

}

void Boss::Draw()
{
	//弾描画
	for (auto& bullet : bullets_) {
		bullet->Draw();
	}

	//本体描画
	GameObject::Draw();
}

void Boss::SpawnBullet()
{
	//プレイヤー座標取得
	Vector3 pos = playerWorld_->GetWorldTranslate();

	//指定値高くする
	pos.y = bulletStartHeight_;

	//渡すパラメータ設定
	BossBulletData data;
	data.velocity = Vector3{ 0,-1.0f,0 }*fallSpeed_;
	data.radius = radius_;

	//座標設定
	data.world.translate_ = pos;
	//生成
	std::unique_ptr<BossBullet>bullet = std::make_unique<BossBullet>(data);

	//配列に追加
	bullets_.emplace_back(std::move(bullet));
}
