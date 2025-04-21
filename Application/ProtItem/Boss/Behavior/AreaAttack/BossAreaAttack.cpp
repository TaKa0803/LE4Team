#include "BossAreaAttack.h"

BossAreaAttack::BossAreaAttack()
{

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
	if (sec_ > waitTime_) {
		countRequest_ = 1;
	}
}

void BossAreaAttack::UpdateBehavior1()
{	
	//タイム以上で発生
	if (sec_ >= (attackTime_ / numbers_) * currentNum_) {
		//カウント増加
		currentNum_++;


	}
}


