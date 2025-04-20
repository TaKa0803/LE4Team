#include "IBossAttack.h"

void IBossAttack::Init()
{
	behaviorCount_ = 0;
}


void IBossAttack::Update()
{
}


//初期化関数をまとめる
void (IBossAttack::* IBossAttack::behaviorInit[])() {
	&IBossAttack::InitBehavior1,
	& IBossAttack::InitBehavior2,
	& IBossAttack::InitBehavior3,
	& IBossAttack::InitBehavior4,
	& IBossAttack::InitBehavior5,

};

//更新処理関数をまとめる
void(IBossAttack::* IBossAttack::behaviorUpdate[])() {
	&IBossAttack::UpdateBehavior1,
	& IBossAttack::UpdateBehavior2,
	& IBossAttack::UpdateBehavior3,
	& IBossAttack::UpdateBehavior4,
	& IBossAttack::UpdateBehavior5,

};