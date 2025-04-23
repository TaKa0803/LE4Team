#pragma once
#include"InstancingGameObject/InstancingGameObject.h"
#include"SphereCollider/SphereCollider.h"

struct BossAmmoData {
	//座標
	EulerWorldTransform world;
	//ベクトル
	Vector3 velocity;
	//加速度
	Vector3 acceraletion;
};

//ボスが出す一発の弾
class BossAmmo :public InstancingGameObject{

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタと初期化
	/// </summary>
	/// <param name="data"></param>
	BossAmmo(const BossAmmoData& data);
	~BossAmmo()=default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private://**プライベート変数**//

	//コライダー
	std::unique_ptr<SphereCollider>colldier_;

private://**パラメータ変数**//
	//速度
	Vector3 velocity_;
	//加速度
	Vector3 acceleration_;
	
};