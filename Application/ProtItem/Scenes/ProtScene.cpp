#include "ProtScene.h"
#include"Camera/Camera.h"

ProtScene::ProtScene()
{
	//プレイヤーの生成
	player_ = std::make_unique<ProtPlayer>();
	
	//カメラ制御のインスタンス生成
	camera_ = std::make_unique<FlowCamera>(&player_->world_);

	//ボスの生成
	boss_ = std::make_unique<Boss>();
	//プレイヤーワールドポインタ生成
	boss_->SetPlayerWorld(&player_->world_);

	//地面クラスの生成
	field_ = std::make_unique<Field>();
}

void ProtScene::Initialize()
{
	//カメラ処理初期化
	camera_->Init();

	//ボスの初期化
	boss_->Init();

	//フィールド初期化
	field_->Initialize();
}

void ProtScene::Update()
{

	//カメラ更新
	camera_->Update();

	//プレイヤーの更新
	player_->Update();

	//ボスの更新
	boss_->Update();

	//ボスの弾の座標取得
	for (auto& bullet : boss_->GetBullets()) {
		//座標
		Vector3 pos = bullet->world_.GetWorldTranslate();
		//半径
		float wide = bullet->GetRadius();

		pos; wide;
	}

	//地面の更新
	field_->Update();
	player_->world_.translate_.y = field_->GetMassLocationPosY(player_->world_.translate_);

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
