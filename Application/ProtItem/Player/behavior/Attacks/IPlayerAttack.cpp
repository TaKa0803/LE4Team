#include "IPlayerAttack.h"
#include"DeltaTimer/DeltaTimer.h"
#include"ProtItem/Player/ProtPlayer.h"

IPlayerAttack::IPlayerAttack()
{

	input_ = std::make_unique<ProtPlayerInput>();

	std::vector<std::string> behaviorName_ = {
		"予備動作",
		"攻撃",
		"硬直"
	};

	tree_.SetMonitorValue("経過時間", &sec_);
	tree_.SetMonitorCombo("状態",&behavior_,behaviorName_);
	//各最大時間パラメータを設定
	for (int i = 0; i < Count; i++) {
		tree_.SetValue(behaviorName_[i], &maxSeconds_[i]);
	}
}

void IPlayerAttack::Init()
{
	behavior_ = 0;
	behaviorRequest_ = 0;
	isAttack_ = false;
	isEnd_ = false;
}

void IPlayerAttack::Update()
{
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		behaviorRequest_ = std::nullopt;
		sec_ = 0;
		nowMaxSecond_ = maxSeconds_[behavior_];
		//初期化
		(this->*behaviorInit[behavior_])();		
	}

	//更新
	(this->*behaviorUpdate[behavior_])();

	//時間更新
	sec_ += (float)DeltaTimer::deltaTime_;

	//時間経過で次の状態に移行
	if (sec_ >= nowMaxSecond_) {
		//次の状態に移行
		if (behavior_ < Count - 1) {
			behaviorRequest_ = behavior_ + 1;
		}
		else {
			isEnd_ = true;
		}
	}

	//もし次の攻撃入力があったら
	if (input_->GetInput(ProtPlayerInput::Type::Attack)) {
		//次の攻撃フラグを立てる
		isAttack_ = true;
	}

}

//初期化関数をまとめる
void (IPlayerAttack::* IPlayerAttack::behaviorInit[])() {
	& IPlayerAttack::InitPreliminaryAction,
	& IPlayerAttack::InitAction,
	& IPlayerAttack::InitRigorAction
};

//更新処理関数をまとめる
void(IPlayerAttack::* IPlayerAttack::behaviorUpdate[])() {
	& IPlayerAttack::UpdatePreliminaryAction,
	& IPlayerAttack::UpdateAction,
	& IPlayerAttack::UpdateRigorAction,
};