#include "ProtPlayerRoll.h"
#include"ProtItem/Player/ProtPlayer.h"
#include"DeltaTimer/DeltaTimer.h"

ProtPlayerRoll::ProtPlayerRoll()
{
	tree_.name_ = "回避";
	tree_.SetMonitorValue("経過時間", &currentSec_);
	tree_.SetMonitorValue("回避時の移動方向", &velo_);

	tree_.SetValue("時間", &rollSec_);
	tree_.SetValue("速度", &rollSpeed_);
	tree_.SetValue("回避後のクールタイム", &cooldowmSec_);
}

void ProtPlayerRoll::Init()
{
	//入力方向チェック
	velo_ = player_->SetBody2Input();

	//もし入力がない場合
	if (velo_ == Vector3{0, 0, 0}) {
		//前に進む
		velo_ = {0,0,1};
	}

	//時間リセット
	currentSec_ = 0;
}

void ProtPlayerRoll::Update()
{

	//時間カウント
	currentSec_ += (float)DeltaTimer::deltaTime_;

	//時間外なら終了
	if (currentSec_ >= rollSec_) {
		//通常状態にリクエスト
		player_->behaviorRequest_ = ProtPlayer::Behavior::Move;
		//回避クールタイムを再セット
		player_->parameters_.currentRollSec = cooldowmSec_;
		
		//処理を終える
		return;
	}

	//時間内なので処理

	//移動する
	player_->world_.translate_ += velo_ * rollSpeed_ * (float)DeltaTimer::deltaTime_;
}
