#include "BossWeaponRollAttack.h"
#include"ProtItem/Boss/Boss.h"

BossWeaponRollAttack::BossWeaponRollAttack()
{
	tree_.name_ = "武器回転攻撃";
	tree_.SetValue("待機時間", &waitTime_);
	tree_.SetValue("攻撃時間", &attackTime_);
	tree_.SetValue("硬直時間", &endTime_);

}

void BossWeaponRollAttack::InitBehavior0()
{
}

void BossWeaponRollAttack::InitBehavior1()
{
}

void BossWeaponRollAttack::InitBehavior2()
{
}

void BossWeaponRollAttack::UpdateBehavior0()
{
	if (boss_->parameters_.currentSec >= waitTime_) {
		countRequest_ = 1;
	}
}

void BossWeaponRollAttack::UpdateBehavior1()
{
	if (boss_->parameters_.currentSec >= attackTime_) {
		countRequest_ = 2;
	}
}

void BossWeaponRollAttack::UpdateBehavior2()
{
	if (boss_->parameters_.currentSec >= endTime_) {
		boss_->behaviorRequest_ = Boss::Behavior::Idle;
	}
}
