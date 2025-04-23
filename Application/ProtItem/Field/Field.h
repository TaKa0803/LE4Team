/**
 * @file Field.h
 * @brief ステージ全般の機能を管理
 * @author KATO
 * @date 2025/04/18
 */

#pragma once
#include "InstancingModelManager/InstancingModelManager.h"
#include "WorldTransform/WorldTransform.h"
#include "Camera/Camera.h"

#include <list>
#include <cassert>


#include"Input/Input.h"

struct Block {
	EulerWorldTransform world;
	Vector4 color;
	Vector2 massLocation;
};

class Field {
public:

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	//ステージの生成
	void CreateStage();

	//ステージの状態リセット
	void ResetStage();

	//ステージの削除
	void DeleteStage();

	//各ブロックの高さの限界値を設定
	void SetBlockHeightLimit(float heightLimit);

	//現在位置のブロックを基にプレイヤー座標Yを計算して返す
	float GetMassLocationPosY(Vector3 translate);

private:
	//各ブロックの生成
	void CreateBlocks(const int x, const int z);

	//座標を基にブロック位置を取得する
	Vector2 GetBlockAt(float x, float z);

	/// <summary>
	/// 指定ブロックを中心に周囲の高さを変える(減衰なし)
	/// 第一引数:中心地点
	/// 第二引数:下がる範囲
	/// 第三引数:中心地点の下がる量
	/// </summary>
	void RaiseBlocksAround(const Vector2& center, float radius, float deltaY);

	/// <summary>
	/// 指定ブロックを中心に周囲の高さを変える(減衰あり)
	/// 第一引数:中心地点
	/// 第二引数:下がる範囲
	/// 第三引数:中心地点の下がる量
	/// </summary>
	void RaiseBlocksAroundWithAttenuation(const Vector2& center, float radius, float deltaY);

	//各ブロックの高さを限界値内に修正
	void FixedHeightCorrection();

	/// <summary>
	/// 各ブロックの高さで色を変える
	/// 第一引数:Yが高い時の色の最大値
	/// 第二引数:Yが低い時の色の最大値
	/// 第三引数:中心Y座標
	/// 第四引数:+-範囲を色が最大となる値
	/// </summary>
	void ColorAdjustmentByHeight(const Vector4& highColor, const Vector4& lowColor, float centerY, float rangeY);

private:
	EulerWorldTransform worldTransform_;

	Input* input_ = nullptr;

	std::string tag_ = "Block";

	InstancingModelManager* IMM_ = nullptr;

	Vector3 centerBlockPos_ = { 0.0f,0.0f,0.0f };//基準となる中心ブロックの座標
	float blockSize_ = 1.0f;//ブロックのサイズ

	std::list<Block> blocks_;//各種ブロック
	static const int verticalSize_ = 20;//縦のブロック数
	static const int horizontalSize_ = 20;//横のブロック数
	float blockWidth_ = 2.0f;//ブロック間隔
	float prevBlockWidth_ = blockWidth_;//前フレームのブロック間隔

	//テスト用変数
	Vector2 nowPos_ = { 0.0f,0.0f };//現在の指定位置
	float radius_ = 1.0f;//中心からの範囲
	float deltaY_ = -0.5f;//一度に下がる値
	float heightLimit_ = 2.5f;//ブロックの高さ限界値+heightLimit ~ -heightLimitの範囲
	Vector4 highColor_ = { 1.0f, 0.0f, 0.0f, 1.0f };//Yが高い時の色
	Vector4 lowColor_ = { 0.0f, 0.0f, 1.0f, 1.0f };//Yが低い時の色
};