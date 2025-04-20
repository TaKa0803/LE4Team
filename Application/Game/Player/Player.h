#pragma once
#include"SingleGameObjects/GameObject.h"
#include"Camera/Camera.h"
#include"SphereCollider/SphereCollider.h"
#include"Game/Player/Behavior/PlayerBaseBehavior.h"
#include"Game/Effect/Impact/Impact.h"
#include"Game/Effect/EffectMove/EffectMove.h"

#include"Game/Player/Input/PlayerInputManager.h"
#include"Game/Player/Animation/PlayerAnimationManager.h"
#include"Game/CircleShadow/CircleShadow.h"
#include<vector>

//プレイヤークラス
class Player :public GameObject {

public://**パブリック変数**//

	//基本パラメータ
	PlayerParameter baseParameter_;

	//入力パラメータ
	PlayerInputParameter inputParameter_;

public://**パブリック変数**//

	Player();
	~Player() = default;

	void Initialize();

	/// <summary>
	/// ゲーム関係の更新
	/// </summary>
	void GameUpdate();

	/// <summary>
	/// モデルの更新
	/// </summary>
	void ObjectUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// パーティクル描画
	/// </summary>
	void DrawParticle();

	/// <summary>
	/// UIの描画
	/// </summary>
	void DrawUI();

	//押し戻し
	void OnCollisionBack(const Vector3& backV);

	//コライダーを取得
	SphereCollider* GetCollider() { return collider_.get(); }

	//攻撃状態か取得
	bool IsPlayerATK() {
		if (behavior_ == State::ATK) { return true; }
		return false;
	}

	/// <summary>
	/// 移動エフェクト出現
	/// </summary>
	void SpawnMoveEffect();

	/// <summary>
	/// 攻撃エフェクト出現
	/// </summary>
	void SpawnImpactEffect() { impactE_->Spawn(world_); };

public://**セッター**//

	/// <summary>
	/// アニメーションのセット
	/// </summary>
	/// <param name="type">アニメーションタイプ</param>
	void SetAnimation(PlayerAnimationManager::Animation type) { animationManager_->SetAnimation(type); }

	/// <summary>
	/// 入力方向を向く
	/// </summary>
	/// <returns>向きベクトルを返却</returns>
	Vector3 SetBody4Input();

public://**ゲッター**//

	/// <summary>
	/// プレイヤー入力管理取得
	/// </summary>
	/// <returns></returns>
	PlayerInputManager* GetInput() { return input_.get(); }

#pragma region 状態管理とメンバ関数ポインタテーブル

	//プレイヤーの状態
	State behavior_ = State::Move;

	//状態群
	std::vector<std::unique_ptr<PlayerBaseBehavior>>behaviors_;

#pragma endregion

private://**プライベート変数**//

	//入力管理
	std::unique_ptr<PlayerInputManager>input_;

	//カメラ
	const Camera* camera_ = nullptr;

	//当たり判定
	std::unique_ptr<SphereCollider> collider_;

	//移動エフェクト
	std::unique_ptr<EffectMove>effectMove_;

	//攻撃エフェクト
	std::unique_ptr<EffectImpact>impactE_;

	//円の影
	std::unique_ptr<CircleShadow>circleShadow_;

	//アニメーション管理
	std::unique_ptr<PlayerAnimationManager>animationManager_;

	//落下速度
	float fallSpd_ = 0.1f;

	//加算式落下加速度
	float addFallSpd_ = 0;
};