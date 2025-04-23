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

	/// <summary>
	/// 弾の発射処理
	/// </summary>
	void SpawnBullet();

public://**セッター**//

	/// <summary>
	/// プレイヤーのワールド座標取得
	/// </summary>
	/// <param name="world"></param>
	void SetPlayerWorld(const EulerWorldTransform* world) { playerWorld_ = world; };

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

private://**プライベート変数**//

	//状態
	std::vector<std::unique_ptr<IBossBehavior>>behaviors_;

	//弾データ群
	std::list<std::unique_ptr<BossBullet>>bullets_;

	//プレイヤー座標
	const EulerWorldTransform* playerWorld_=nullptr;

private://**パラメータ変数**//

	//弾の初期高度
	float bulletStartHeight_ = 10;

	//落下速度
	float fallSpeed_ = 10.0f;
};