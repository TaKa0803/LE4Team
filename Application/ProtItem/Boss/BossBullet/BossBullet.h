#pragma once
#include"InstancingGameObject/InstancingGameObject.h"
#include"SphereCollider/SphereCollider.h"

struct BossBulletData {
	//座標
	EulerWorldTransform world;
	//ベクトル
	Vector3 velocity;
	//加速度
	Vector3 acceraletion;

	//サイズ
	float radius = 1.0f;
};

//ボスが出す一発の弾
class BossBullet :public InstancingGameObject{

public://**パブリック関数**//

	/// <summary>
	/// コンストラクタと初期化
	/// </summary>
	/// <param name="data"></param>
	BossBullet(const BossBulletData& data);
	~BossBullet()=default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 当たった時の処理
	/// </summary>
	void OnCollision();

	/// <summary>
	/// 死亡フラグ
	/// </summary>
	bool GetDead() { return isDead_; };

private://**プライベート変数**//

	//コライダー
	std::unique_ptr<SphereCollider>colldier_;

private://**パラメータ変数**//
	//速度
	Vector3 velocity_;
	//加速度
	Vector3 acceleration_;
	
	//死亡フラグ
	bool isDead_ = false;
};