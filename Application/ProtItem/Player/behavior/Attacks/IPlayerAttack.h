#pragma once
#include"ProtItem/Player/behavior/IProtBehavior.h"
#include<optional>

class IPlayerAttack :private IProtBehavior {

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IPlayerAttack();
	~IPlayerAttack() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;


public://**状態**//

	/// <summary>
	/// 各状態の初期化関数
	/// </summary>
	virtual void InitPreliminaryAction() = 0;
	virtual void InitAction() = 0;
	virtual void InitRigorAction() = 0;

	/// <summary>
	/// 各状態の更新関数
	/// </summary>
	virtual void UpdatePreliminaryAction() = 0;
	virtual void UpdateAction() = 0;
	virtual void UpdateRigorAction() = 0;

	//関数テーブル
	static void (IPlayerAttack::* behaviorInit[])();
	static void (IPlayerAttack::* behaviorUpdate[])();

	//状態リクエスト
	std::optional<int>behaviorRequest_ = std::nullopt;

private:

	//時間
	float sec_ = 0;

	//攻撃前時間
	float waitSec_ = 0;
	//攻撃時間
	float actionSec_ = 0;
	//攻撃後時間
	float afterSec_ = 0;

	//状態番号
	int behavior_=0;

};