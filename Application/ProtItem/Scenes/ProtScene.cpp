#include "ProtScene.h"

ProtScene::ProtScene()
{
	//プレイヤーの生成
	player_ = std::make_unique<ProtPlayer>();

	//ボスの生成
	boss_ = std::make_unique<Boss>();

	//地面クラスの生成
	field_ = std::make_unique<Field>();
}

void ProtScene::Initialize()
{

	//ボスの初期化
	boss_->Init();

	field_->Initialize();
}

void ProtScene::Update()
{
	//プレイヤーの更新
	player_->Update();

	//ボスの更新
	boss_->Update();

	//地面の更新
	field_->Update();
}

void ProtScene::Draw()
{
	//地面の描画
	field_->Draw();

	//ボスの描画
	boss_->Draw();

	//プレイヤーの描画
	player_->Draw();

	//セットされたインスタンシングモデルをすべて描画
	InstancingModelManager::GetInstance()->DrawAllModel();

}
