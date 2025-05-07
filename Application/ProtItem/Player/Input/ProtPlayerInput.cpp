#include "ProtPlayerInput.h"

ProtPlayerInput::ProtPlayerInput()
{
	//入力のインスタンス取得
	input_ = Input::GetInstance();
}

Vector3 ProtPlayerInput::GetMoveInput()
{
	//回答する変数生成
	Vector3 ans;

	//矢印キーの入力チェック
	ans = input_->GetAllArrowKey();

	//コントローラー入力取得
	ans += input_->GetjoyStickLVec3();

	//返却
	return ans.GetNormalizeNum();
}

bool ProtPlayerInput::GetInput(Type type)
{
	switch (type)
	{
	case ProtPlayerInput::Roll:
		return RollInput();
	case ProtPlayerInput::Attack:
		return AttackInput();
	case ProtPlayerInput::Count:
		break;
	default:
		break;
	}

	//
	return false;
}

bool ProtPlayerInput::RollInput()
{
	//入力チェック変数生成と初期化
	int ans=0;

	//キー入力チェック
	ans = input_->TriggerKey(DIK_LSHIFT);

	//コントローラー入力チェック
	ans += input_->IsTriggerButton(kButtonB);

	//返却
	return (bool)ans;
}

bool ProtPlayerInput::AttackInput()
{
	//入力チェック変数生成と初期化
	int ans = 0;

	//キー入力チェック
	ans = input_->TriggerKey(DIK_SPACE);

	//コントローラー入力チェック
	ans += input_->IsTriggerButton(kButtonX);

	//返却
	return (bool)ans;
}
