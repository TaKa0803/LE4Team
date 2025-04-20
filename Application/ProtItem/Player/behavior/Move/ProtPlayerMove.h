#pragma once
#include"ProtItem/Player/behavior/IProtBehavior.h"

//プレイヤーの移動関係
class ProtPlayerMove : public IProtBehavior {
public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ProtPlayerMove();
	~ProtPlayerMove()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

private://**プライベート変数**//

	//移動速度
	float moveSpped_ = 1.0f;

};