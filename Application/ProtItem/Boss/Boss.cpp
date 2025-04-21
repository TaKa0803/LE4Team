#include "Boss.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"

#pragma region 状態
#include"ProtItem/Boss/Behavior/Idle/BossIdle.h"
#include"ProtItem/Boss/Behavior/AreaAttack/BossAreaAttack.h"
#pragma endregion


Boss::Boss()
{
	//オブジェクト生成
	GameObject::Initialize("Boss");

	behaviors_.resize((size_t)Behavior::Count);
	behaviors_[(size_t)Behavior::Idle] = std::make_unique<BossIdle>();
	behaviors_[(size_t)Behavior::Attack1] = std::make_unique<BossAreaAttack>();
	behaviors_[(size_t)Behavior::Attack2] = std::make_unique<BossAreaAttack>();


	std::unique_ptr<GlobalVariableGroup> gvg = std::make_unique<GlobalVariableGroup>("Boss");
	gvg->SetMonitorValue("共通経過時間", &parameters_.currentSec);
	gvg->SetTreeData(model_->SetDebugParam());
	for (auto& behavior : behaviors_) {
		if (!behavior)continue;
		gvg->SetTreeData(behavior->tree_);
	}
}

void Boss::Init()
{
}

void Boss::Update()
{
	//リクエストがある場合
	if (behaviorRequest_) {
		//リクエストの値を渡す
		behavior_ = behaviorRequest_.value();
		//リクエスト初期化
		behaviorRequest_ = std::nullopt;
		//状態初期化
		behaviors_[(int)behavior_]->Init();
	}

	//状態更新
	behaviors_[(int)behavior_]->Update();

	//行列更新
	GameObject::GameUpdate();


}
