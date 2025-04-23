#include "BossIdle.h"
#include"DeltaTimer/DeltaTimer.h"
#include"ProtItem/Boss/Boss.h"
#include<memory>

BossIdle::BossIdle()
{
	tree_.name_ = "待機";
	tree_.SetValue("時間", &sec_);
}

void BossIdle::Init()
{
}

void BossIdle::Update()
{
	boss_->parameters_.currentSec += (float)DeltaTimer::deltaTime_;
	//時間経過で次の状態に
	if (boss_->parameters_.currentSec >= sec_) {
		boss_->behaviorRequest_ = Boss::Attack1;
	}

}
