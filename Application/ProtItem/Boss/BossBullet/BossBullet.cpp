#include "BossBullet.h"
#include"DeltaTimer/DeltaTimer.h"

BossBullet::BossBullet(const BossAmmoData& data)
{
	InstancingGameObject::Initialize("sphere");

	//弾判定初期化
	world_ = data.world;
	velocity_ = data.velocity;
	acceleration_ = data.acceraletion;

	//コライダー初期化
	colldier_ = std::make_unique<SphereCollider>();
	colldier_->Initialize("敵弾",world_);
}

void BossBullet::Update()
{
	//加速処理
	velocity_ += acceleration_ * (float)DeltaTimer::deltaTime_;
	//移動処理
	world_.translate_ += velocity_ * (float)DeltaTimer::deltaTime_;
	//更新
	world_.UpdateMatrix();

	//コライダー更新
	colldier_->Update();
}

