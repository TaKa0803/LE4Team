#include "ProtPlayerMove.h"
#include"ProtItem/Player/ProtPlayer.h"
#include"DeltaTimer/DeltaTimer.h"

ProtPlayerMove::ProtPlayerMove()
{

	tree_.name_ = "移動";
	tree_.SetValue("移動速度",&moveSpped_);

}

void ProtPlayerMove::Init()
{
}

void ProtPlayerMove::Update()
{

	//移動入力取得
	Vector3 move = player_->SetBody2Input();

	//移動する
	player_->world_.translate_ += move * moveSpped_*(float)DeltaTimer::deltaTime_;

	//もし回避入力があった&クールタイムが終わっている場合
	if (player_->GetInput()->GetInput(ProtPlayerInput::Type::Roll)&&player_->parameters_.currentRollSec==0) {
		//状態リクエスト送信
		player_->behaviorRequest_ = ProtPlayer::Behavior::Roll;
	}
}
