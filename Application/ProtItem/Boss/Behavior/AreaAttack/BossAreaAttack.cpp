#include "BossAreaAttack.h"
#include"ProtItem/Boss/Boss.h"

BossAreaAttack::BossAreaAttack()
{
	tree_.name_ = "落下物攻撃";
	tree_.SetValue("待機時間", &waitTime_);
	tree_.SetValue("実行時間", &attackTime_);
	tree_.SetValue("個数", &numbers_);
}

void BossAreaAttack::InitBehavior0()
{
	//予備動作初期化
}

void BossAreaAttack::InitBehavior1()
{
	//実際の行動初期化
	currentNum_ = 0;
}

void BossAreaAttack::UpdateBehavior0()
{
	//時間外で次
	if (boss_->parameters_.currentSec > waitTime_) {
		countRequest_ = 1;
	}
}

void BossAreaAttack::UpdateBehavior1()
{	
	//経過時間で終了
	if (boss_->parameters_.currentSec >= attackTime_) {
		//通常状態に移行
		boss_->behaviorRequest_ = Boss::Behavior::Idle;
	}

	//タイム以上で発生
	if (boss_->parameters_.currentSec >= (attackTime_ / numbers_) * currentNum_) {
		//カウント増加
		currentNum_++;

		//生成
		boss_->SpawnDangerZone();
	}
}


