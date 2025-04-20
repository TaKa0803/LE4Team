#include "ProtScene.h"

ProtScene::ProtScene()
{
	//プレイヤーの生成
	player_ = std::make_unique<ProtPlayer>();

	//地面クラスの生成
	plane_ = std::make_unique<Plane>();
}

void ProtScene::Initialize()
{
}

void ProtScene::Update()
{
	//プレイヤーの更新
	player_->Update();
}

void ProtScene::Draw()
{
	//地面の描画
	plane_->Draw();

	//プレイヤーの描画
	player_->Draw();
}
