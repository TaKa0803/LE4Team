#include "DangerZone.h"
#include"DeltaTimer/DeltaTimer.h"

DangerZone::DangerZone(const DangerZoneParameters& param)
{
	parameters_ = param;

	//座標設定
	world_.translate_ = param.centerPos;

	//オブジェクト生成
	InstancingGameObject::Initialize("DangerArea");
}

void DangerZone::Update()
{
	//時間加算
	sec_ += (float)DeltaTimer::deltaTime_;

	float t = sec_ / parameters_.maxDeadSec;
	//イージング
	float wide = Lerp(0.0f, parameters_.maxRadius, t);

	//サイズ設定
	world_.scale_ = { wide,1,wide };
	//ワールド座標更新
	world_.UpdateMatrix();

	//時間経過でフラグON
	if (sec_ >= parameters_.maxDeadSec)
	{
		isDead_ = true;
	}

}
