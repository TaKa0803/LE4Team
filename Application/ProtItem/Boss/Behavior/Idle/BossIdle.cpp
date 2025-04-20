#include "BossIdle.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"
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
}
