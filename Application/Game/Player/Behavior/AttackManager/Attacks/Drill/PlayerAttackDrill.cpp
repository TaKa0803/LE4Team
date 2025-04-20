#include "PlayerAttackDrill.h"
#include"Game/Player/Player.h"

PlayerAttackDrill::PlayerAttackDrill()
{
	tree_.name_ = "ドリル攻撃";
}

void PlayerAttackDrill::InitReserve()
{
	//アニメーションをセット
	player_->SetAnimation(PlayerAnimationManager::ATK_Drill);
}

void PlayerAttackDrill::InitExecution()
{

}

void PlayerAttackDrill::InitRigor()
{
	//アニメーションをセット
	player_->SetAnimation(PlayerAnimationManager::WALK);
}

void PlayerAttackDrill::UpdateReserve()
{
	//入力方向に向く
	player_->SetBody4Input();
	//条件を満たしたら次の状態へ
	if (currentSec_ >= nowMaxSec_) {
		//次の状態へ
		behaviorRequest_ = Behavior::Execution;
	}
}

void PlayerAttackDrill::UpdateExecution()
{
	//入力方向に向いて移動
	SetInput2Move();
	//条件を満たしたら次の状態へ
	if (currentSec_ >= nowMaxSec_) {
		//次の状態へ
		behaviorRequest_ = Behavior::Rigor;
	}
}

void PlayerAttackDrill::UpdateRigor()
{
	//入力方向に向いて移動
	SetInput2Move();
	//条件を満たしたらおわる
	if (currentSec_ >= nowMaxSec_) {
		//おわる
		isEnd_ = true;
	}
}
