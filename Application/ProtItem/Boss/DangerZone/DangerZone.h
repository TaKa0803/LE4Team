#pragma once
#include"InstancingGameObject/InstancingGameObject.h"

struct DangerZoneParameters {
	Vector3 centerPos = { 0,0,0 };
	float maxRadius = 1.0f;
	float maxDeadSec = 0.5f;
};

class DangerZone:public InstancingGameObject{

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DangerZone(const DangerZoneParameters& param);
	~DangerZone() = default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 死亡フラグ取得
	/// </summary>
	/// <returns></returns>
	bool GetDead() const { return isDead_; }

private://**プライベート変数**//

	//パラメータ
	DangerZoneParameters parameters_;

	//時間
	float sec_ = 0;

	//死亡フラグ
	bool isDead_ = false;
};