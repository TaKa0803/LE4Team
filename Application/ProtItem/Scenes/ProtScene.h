#pragma once
#include "IScene/IScene.h"
#include"ProtItem/Player/ProtPlayer.h"
#include"Game/Plane/Plane.h"

class ProtScene : public IScene {

public://**パブリック関数**//
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ProtScene();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ProtScene()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:

	//プレイヤー
	std::unique_ptr<ProtPlayer> player_ = nullptr;

	//地面
	std::unique_ptr<Plane>plane_ = nullptr;
};