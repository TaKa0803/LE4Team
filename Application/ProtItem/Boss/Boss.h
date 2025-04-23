#pragma once
#include"SingleGameObjects/GameObject.h"
#include"ProtItem/Boss/Behavior/IBossBehavior.h"
#include"SphereCollider/SphereCollider.h"
#include"BossBullet/BossBullet.h"
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

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public://**セッター**//

	/// <summary>
	/// 攻撃コライダーのセット
	/// </summary>
	/// <param name="collider">コライダー</param>
	void SetAttackCollider(SphereCollider* collider) { attackColliders_.push_back(collider); };

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

	//弾データ群
	std::vector<std::unique_ptr<BossBullet>>bullets_;
};