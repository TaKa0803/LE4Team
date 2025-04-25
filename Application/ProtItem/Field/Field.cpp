/**
 * @file Field.h
 * @brief ステージ全般の機能を管理
 * @author KATO
 * @date 2025/04/18
 */

#include "Field.h"
#include"ImGuiManager/ImGuiManager.h"

void Field::Initialize() {
	//Input
	input_ = Input::GetInstance();

	//インスタンシングモデルマネージャ生成
	IMM_ = InstancingModelManager::GetInstance();

	worldTransform_.Initialize();

	//ステージを生成
	CreateStage();

	//ステージ開始演出
	StartStage();
}

void Field::Update() {
	ImGui::Begin("TestOperate");
	ImGui::DragFloat("BlockWidth", &blockWidth_);
	ImGui::DragFloat("Radius", &radius_);
	ImGui::DragFloat("DeltaY", &deltaY_);
	ImGui::DragFloat("HeightLimit", &heightLimit_);
	if (ImGui::Button("TestRaiseBlocksAround")) {
		//現在のnowPos_の位置からradius_範囲をdeltaY_分下げる
		RaiseBlocksAround(GetBlockAt(nowPos_.x, nowPos_.y), radius_, deltaY_);
	}
	if (ImGui::Button("TestRaiseBlocksAroundWithAttenuation")) {
		//現在のnowPos_の位置からradius_範囲をdeltaY_分下げる(距離減衰付き)
		RaiseBlocksAroundWithAttenuation(GetBlockAt(nowPos_.x, nowPos_.y), radius_, deltaY_);
	}
	if (ImGui::Button("TestSetBlockHeightLimit")) {
		//各ブロックの高さを限界値で固定
		SetBlockHeightLimit(heightLimit_);
	}
	if (ImGui::Button("StartStage")) {
		//ステージ開始演出
		Finalize();

		CreateStage();

		StartStage();
	}
	if (ImGui::Button("ResetStage")) {
		//ステージ状態をリセット
		ResetStage();
	}
	ImGui::End();

	//ステージ開始/リセット演出
	if (isAnimationReset_ == true) {
		deltaTime_ += deltaPlusTime_;
		PlayStageIntroAnimation(deltaTime_);
	}

	//高さを限界値内に修正
	FixedHeightCorrection();

	//各ブロックの高さに応じて色を変更
	ColorAdjustmentByHeight(highColor_, lowColor_, 0.0f, 2.0f);

#ifdef _DEBUG
	//現在のnowPos_に対応するブロックを赤くする
	Vector2 selected = GetBlockAt(nowPos_.x, nowPos_.y);
	for (Block& block : blocks_) {
		if (block.massLocation.x == selected.x && block.massLocation.y == selected.y) {
			block.color = { 1.0f, 0.0f, 0.0f, 1.0f };//赤
		}
	}
#endif

	//ブロック間隔の更新
	if (blockWidth_ != prevBlockWidth_) {
		for (Block& block : blocks_) {
			//ステージの中央からのオフセットを計算
			float offsetX = (horizontalSize_ - 1) * 0.5f * blockWidth_;
			float offsetZ = (verticalSize_ - 1) * 0.5f * blockWidth_;

			//ブロックの配置座標を計算
			block.world.translate_ = {
				centerBlockPos_.x + (block.massLocation.x * blockWidth_) - offsetX,
				centerBlockPos_.y,
				centerBlockPos_.z + (block.massLocation.y * blockWidth_) - offsetZ
			};
		}

		//更新
		prevBlockWidth_ = blockWidth_;
	}

	//各ブロックの行列更新
	for (Block& block : blocks_) {
		block.world.UpdateMatrix();
	}
}

void Field::Draw() {
	for (Block& block : blocks_) {
		IMM_->SetData(tag_, block.world, block.color);
	}
}

void Field::Finalize() {
	blocks_.clear();
}

void Field::StartStage() {
	isAnimationReset_ = true;
	deltaTime_ = 0.0f;
	elapsedTime_ = 0.0f;
	deltaPlusTime_ = 1.0f / 2400.0f;
}

void Field::ResetStage() {
	isAnimationReset_ = true;
	deltaTime_ = 0.0f;
	elapsedTime_ = 0.0f;
	deltaPlusTime_ = 1.0f / 600.0f;
}

void Field::DeleteStage() {
	//ブロックを削除
	blocks_.clear();
}

void Field::SetBlockHeightLimit(float heightLimit) {
	//高さの限界値を設定
	heightLimit_ = heightLimit;
}

float Field::GetMassLocationPosY(Vector3 translate) {
	//現在のマスを確認する
	Vector2 selected = GetBlockAt(translate.x, translate.z);
	for (Block& block : blocks_) {
		if (block.massLocation.x == selected.x && block.massLocation.y == selected.y) {
			//プレイヤーの位置をnowPos_に記録
			nowPos_ = { translate.x,translate.z };
			//現在のブロック座標Y + ブロックのサイズ(半径)を返す
			return block.world.translate_.y + blockSize_;
		}
	}

	//何もなければそのままを返す
	return translate.y;
}

void Field::CreateBlocks(const int x, const int z) {
	Block block;

	block.world.Initialize();

	//ステージの中央からのオフセットを計算
	float offsetX = (horizontalSize_ - 1) * 0.5f * blockWidth_;
	float offsetZ = (verticalSize_ - 1) * 0.5f * blockWidth_;

	//ブロックの配置座標を計算
	block.world.translate_ = {
		centerBlockPos_.x + (x * blockWidth_) - offsetX,
		centerBlockPos_.y,
		centerBlockPos_.z + (z * blockWidth_) - offsetZ
	};

	block.world.rotate_ = { 0.0f,0.0f,0.0f };
	block.world.scale_ = { 0.0f,0.0f,0.0f };

	block.color = { 1.0f,1.0f,1.0f,1.0f };

	//マス位置xとzを保存
	block.massLocation = { (float)x,(float)z };

	blocks_.push_back(block);
}

void Field::CreateStage() {
	//各ブロックの生成
	for (int i = 0; i < verticalSize_; i++) {
		for (int j = 0; j < horizontalSize_; j++) {
			CreateBlocks(i, j);
		}
	}

	//限界値の設定
	SetBlockHeightLimit(heightLimit_);
}

Vector2 Field::GetBlockAt(float x, float z) {
	for (Block& block : blocks_) {
		const Vector3& blockPos = block.world.translate_;

		//各軸面上でブロックの中心からBlockSize_の範囲か確認
		if (std::abs(blockPos.x - x) <= blockSize_ &&
			std::abs(blockPos.z - z) <= blockSize_) {
			return block.massLocation;
		}
	}

	return Vector2{ -1,-1 };//見つからなければ適当な値
}

void Field::PlayStageIntroAnimation(float deltaTime) {
	if (!isAnimationReset_) return;

	elapsedTime_ += deltaTime;

	bool allFinished = true;

	for (Block& block : blocks_) {
		Vector3& pos = block.world.translate_;
		Vector3& rot = block.world.rotate_;
		Vector3& scale = block.world.scale_;

		//中心ブロックとの距離を使ってディレイを計算
		float dx = pos.x - centerBlockPos_.x;
		float dz = pos.z - centerBlockPos_.z;
		float distanceFromCenter = std::sqrt(dx * dx + dz * dz);

		float startDelay = distanceFromCenter * 0.2f;//距離に応じた遅延
		float localTime = elapsedTime_ - startDelay;

		if (localTime < 0.0f) {
			allFinished = false;//まだ開始してないブロックあり
			continue;
		}

		float animDuration = 1.0f;//各ブロックのアニメ時間
		float t = min(localTime / animDuration, 1.0f);

		//イージング（smoothstep風）
		float easedT = t * t * (3.0f - 2.0f * t);

		//回転：最終的に0に戻す（0 → 最大回転 → 0）
		float angle = std::sin(easedT * 3.14159f) * 3.14159f;//πラジアン（180°）回転して戻る
		rot = { 0.0f, angle, 0.0f };

		//アニメーション適用
		pos.y = -5.0f + easedT * 5.0f;
		scale = { easedT, easedT, easedT };

		if (t < 1.0f) {
			allFinished = false;//このブロックはまだ完了してない
		}
	}

	if (allFinished) {
		isAnimationReset_ = false;//全ブロック完了で演出終了
	}
}

void Field::RaiseBlocksAround(const Vector2& center, float radius, float deltaY) {
	//基準となる中心ブロックのワールド座標を取得
	Vector3 centerPos{};
	bool found = false;

	for (const Block& block : blocks_) {
		if (block.massLocation.x == center.x && block.massLocation.y == center.y) {
			centerPos = block.world.translate_;
			found = true;
			break;
		}
	}

	//中心ブロックが見つからない場合は何もしない
	if (!found) {
		return;
	}

	//半径範囲内にあるブロックを下げる
	for (Block& block : blocks_) {
		Vector3 pos = block.world.translate_;

		//中心座標からの距離を計算（XZ平面）
		float dx = pos.x - centerPos.x;
		float dz = pos.z - centerPos.z;
		float distance = std::sqrt(dx * dx + dz * dz);

		if (distance <= radius) {
			block.world.translate_.y += deltaY;
		}
	}
}

void Field::RaiseBlocksAroundWithAttenuation(const Vector2& center, float radius, float deltaY) {
	//基準となる中心ブロックのワールド座標を取得
	Vector3 centerPos{};
	bool found = false;

	for (const Block& block : blocks_) {
		if (block.massLocation.x == center.x && block.massLocation.y == center.y) {
			centerPos = block.world.translate_;
			found = true;
			break;
		}
	}

	//中心ブロックが見つからない場合は何もしない
	if (!found) {
		return;
	}

	//半径範囲内にあるブロックを下げる（中心に近いほど多く動かす）
	for (Block& block : blocks_) {
		Vector3 pos = block.world.translate_;

		// 中心座標からの距離を計算（XZ平面）
		float dx = pos.x - centerPos.x;
		float dz = pos.z - centerPos.z;
		float distance = std::sqrt(dx * dx + dz * dz);

		if (distance <= radius) {
			//線形減衰（距離が大きいほど減少量は少なくなる）
			float attenuation = 1.0f - (distance / radius);//1.0〜0.0f
			float adjustedDeltaY = deltaY * attenuation;
			block.world.translate_.y += adjustedDeltaY;
		}
	}
}

void Field::FixedHeightCorrection() {
	for (Block& block : blocks_) {
		if (block.world.translate_.y >= heightLimit_) {//+heightlimit_より大きければ
			block.world.translate_.y = heightLimit_;
		}
		else if (block.world.translate_.y <= -heightLimit_) {//-heightlimit_より小さければ
			block.world.translate_.y = -heightLimit_;

		}
		else {//範囲内なら

		}
	}
}

void Field::ColorAdjustmentByHeight(const Vector4& highColor, const Vector4& lowColor, float centerY, float rangeY) {
	for (Block& block : blocks_) {
		float y = block.world.translate_.y;

		//上方向
		if (y > centerY + rangeY) {
			block.color = highColor;
		}
		else if (y > centerY) {
			float t = (y - centerY) / rangeY;//0.0～1.0
			block.color = {
				1.0f * (1.0f - t) + highColor.x * t,
				1.0f * (1.0f - t) + highColor.y * t,
				1.0f * (1.0f - t) + highColor.z * t,
				1.0f * (1.0f - t) + highColor.w * t
			};
		}

		//下方向
		else if (y < centerY - rangeY) {
			block.color = lowColor;
		}
		else if (y < centerY) {
			float t = (centerY - y) / rangeY;//0.0～1.0
			block.color = {
				1.0f * (1.0f - t) + lowColor.x * t,
				1.0f * (1.0f - t) + lowColor.y * t,
				1.0f * (1.0f - t) + lowColor.z * t,
				1.0f * (1.0f - t) + lowColor.w * t
			};
		}

		//中心値：白
		else {
			block.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
	}
}