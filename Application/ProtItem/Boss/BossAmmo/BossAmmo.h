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
	//円判定
	std::unique_ptr<SphereCollider> collider;

};

//ボスが出す一発の弾
class BossAmmo :public InstancingGameObject{

public://**パブリック関数**//

	BossAmmo(const BossAmmo& ammo);
	~BossAmmo()=default;

	void Update();

	void Collison();

private:
	
	//データ
	BossAmmoData data_;
};