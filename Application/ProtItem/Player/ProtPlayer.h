#pragma once
#include"SingleGameObjects/GameObject.h"
#include"ProtItem/Player/Input/ProtPlayerInput.h"
#include"ProtItem/Player/behavior/IProtBehavior.h"
#include"Collider/SphereCollider/SphereCollider.h"
#include<optional>


struct ProtPlayerParameters {
	//体力
	int hp = 10;

	//ヒットフラグ
	bool isHit = true;

	//回避クールタイムカウント
	float currentRollSec = 0;

};


class ProtPlayer :public GameObject {

public://**パブリック変数**//

	//基本パラメータ
	ProtPlayerParameters parameters_;

public://**パブリック変数**//

	//コンストラクタ
	ProtPlayer();
	//デストラクタ
	~ProtPlayer() = default;

	//更新
	void Update();

	//描画
	void Draw();

	/// <summary>
	/// 座標をセットして更新
	/// </summary>
	/// <param name="translate">セットする座標</param>
	void SetWorldTranslate(const Vector3& translate);

public://**ゲッター**//

	/// <summary>
	/// 入力取得
	/// </summary>
	/// <returns></returns>
	ProtPlayerInput* GetInput() { return input_.get(); };

public://**セッター

	/// <summary>
	/// 入力方向に体を向ける
	/// </summary>
	/// <returns>向きベクトル</returns>
	Vector3 SetBody2Input();

public://**状態**//

	//状態
	enum class Behavior{
		Move,	//移動
		Roll,	//回避
		Count	//状態の数
	}behaviorName_=Behavior::Move;

	//状態リクエスト
	std::optional<Behavior>behaviorRequest_ = Behavior::Move;

	//状態処理群
	std::vector<std::unique_ptr<IProtBehavior>>behaviors_;

private://**プライベート変数**//

	//プレイヤー入力クラス
	std::unique_ptr<ProtPlayerInput>input_;

	//当たり判定
	std::unique_ptr<SphereCollider>collider_;

};