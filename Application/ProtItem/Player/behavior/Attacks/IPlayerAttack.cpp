#include "IPlayerAttack.h"

IPlayerAttack::IPlayerAttack()
{
	
}

void IPlayerAttack::Init()
{
	behavior_ = 0;
	behaviorRequest_ = 0;
}

void IPlayerAttack::Update()
{
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		behaviorRequest_ = std::nullopt;
		//player_->parameters_.currentSec = 0;
		//初期化
		(this->*behaviorInit[behavior_])();
	}

	//更新
	(this->*behaviorUpdate[behavior_])();
}

//初期化関数をまとめる
void (IPlayerAttack::* IPlayerAttack::behaviorInit[])() {
	&IPlayerAttack::InitPreliminaryAction,
	& IPlayerAttack::InitAction,
	& IPlayerAttack::InitRigorAction
};

//更新処理関数をまとめる
void(IPlayerAttack::* IPlayerAttack::behaviorUpdate[])() {
	&IPlayerAttack::UpdatePreliminaryAction,
	& IPlayerAttack::UpdateAction,
	& IPlayerAttack::UpdateRigorAction,
};