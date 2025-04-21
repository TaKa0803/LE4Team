#include "IBossAttack.h"
#include"DeltaTimer/DeltaTimer.h"

IBossAttack::IBossAttack()
{
	tree_.SetMonitorValue("現在の状態番号", &behaviorCount_);
	tree_.SetMonitorValue("状態の経過時間", &sec_);

}

void IBossAttack::Init()
{
	behaviorCount_ = 0;
	countRequest_ = 0;
	sec_ = 0;
}


void IBossAttack::Update()
{
	if (countRequest_) {
		behaviorCount_ = countRequest_.value();
		countRequest_ = std::nullopt;

		//初期化
		(this->*behaviorInit[behaviorCount_])();
	}

	//時間経過
	sec_ += (float)DeltaTimer::deltaTime_;

	//更新
	(this->*behaviorUpdate[behaviorCount_])();

}


//初期化関数をまとめる
void (IBossAttack::* IBossAttack::behaviorInit[])() {
	&IBossAttack::InitBehavior0,
	& IBossAttack::InitBehavior1,
	& IBossAttack::InitBehavior2,
	& IBossAttack::InitBehavior3,
	& IBossAttack::InitBehavior4,

};

//更新処理関数をまとめる
void(IBossAttack::* IBossAttack::behaviorUpdate[])() {
	&IBossAttack::UpdateBehavior0,
	& IBossAttack::UpdateBehavior1,
	& IBossAttack::UpdateBehavior2,
	& IBossAttack::UpdateBehavior3,
	& IBossAttack::UpdateBehavior4,

};