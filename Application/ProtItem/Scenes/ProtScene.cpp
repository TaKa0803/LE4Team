#include "ProtScene.h"

ProtScene::ProtScene()
{
	//プレイヤーの生成
	player_ = std::make_unique<ProtPlayer>();

	//ボスの生成
	boss_ = std::make_unique<Boss>();

	//地面クラスの生成
	plane_ = std::make_unique<Plane>();
}

void ProtScene::Initialize()
{

	//ボスの初期化
	boss_->Init();

}

void ProtScene::Update()
{
	//プレイヤーの更新
	player_->Update();

	//ボスの更新
	boss_->Update();
}

void ProtScene::Draw()
{
	//地面の描画
	plane_->Draw();

	//ボスの描画
	boss_->Draw();

	//プレイヤーの描画
	player_->Draw();

}
