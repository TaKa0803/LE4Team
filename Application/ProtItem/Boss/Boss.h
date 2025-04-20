#pragma once
#include"SingleGameObjects/GameObject.h"
#include"ProtItem/Boss/Behavior/IBossBehavior.h"
#include<optional>

//ボスクラス
class Boss :public GameObject {

public://**パブリック変数**//

	//パラメータ
	struct Parameters {
		//カウント時間
		float currentSec = 0;
	}parameters_;


public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Boss();
	~Boss()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public://**状態

	//状態
	enum Behavior {
		Idle,
		Attack1,
		Attack2,
		Count
	}behavior_;

	//状態リクエスト
	std::optional<Behavior>behaviorRequest_ = Behavior::Idle;

	//状態
	std::vector<std::unique_ptr<IBossBehavior>>behaviors_;

};