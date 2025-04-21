#pragma once
#include"ProtItem/Boss/Behavior/IBossAttack/IBossAttack.h"

//プレイヤーに弾を落とす攻撃を行う
class BossAreaAttack : public IBossAttack {

public:

	BossAreaAttack();
	~BossAreaAttack() = default;

public://**状態**//

	void InitBehavior1()override;
	void InitBehavior2()override;
	void InitBehavior3()override {};
	void InitBehavior4()override {};
	void InitBehavior5()override {};

	void UpdateBehavior1()override;
	void UpdateBehavior2()override;
	void UpdateBehavior3()override {};
	void UpdateBehavior4()override {};
	void UpdateBehavior5()override {};

private:

	//待機時間
	float waitTime_ = 1.0f;

	//

};