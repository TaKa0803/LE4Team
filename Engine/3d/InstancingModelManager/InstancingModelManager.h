#pragma once

#include<string>
#include<vector>
#include<map>
#include<iostream>

#include"InstancingGameObject/InstancingPSO.h"
#include"GlobalVariable/Tree/GlobalVariableTree.h"

//前方宣言
class InstancingModel;

//インスタンシングモデル管理マネージャ
class InstancingModelManager {

public://**シングルトンパターン**//
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static InstancingModelManager* GetInstance();
private:
	InstancingModelManager() = default;
	~InstancingModelManager() = default;
	InstancingModelManager(const InstancingModelManager& o) = delete;
	const InstancingModelManager& operator=(const InstancingModelManager& o) = delete;

public://**パブリック関数**//

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="loadAllModel"></param>
	void Initialize(bool loadAllModel=true);

	/// <summary>
	/// すべてのモデルのロード
	/// </summary>
	void LoadAllModel();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// すべてのモデルの描画
	/// </summary>
	void DrawAllModel();


	/// <summary>
	/// タグの検索
	/// </summary>
	/// <param name="tag"></param>
	bool SerchTag(const std::string& tag);

	/// <summary>
	/// デバッグ表示
	/// </summary>
	/// <param name="tag">タグのモデル</param>
	/// <param name="name">UIの名前</param>
	void Debug(const std::string& tag,  const char* name);

public://**ゲッター**//

	/// <summary>
	/// タグのモデルのα値取得
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	float GetAlpha(const std::string& tag);

	/// <summary>
	/// PSOの取得
	/// </summary>
	/// <returns></returns>
	InstancingPSO* GetPSO() { return pso_; };

	/// <summary>
	/// ツリーの作成と取得
	/// </summary>
	/// <param name="tag">モデルタグ名</param>
	/// <param name="name">ツリー名</param>
	/// <returns></returns>
	GlobalVariableTree& CreateAndGetTree(const std::string& tag, const std::string& name);

public://**セッター**//

	/// <summary>
	/// タグのモデルにワールド追加
	/// </summary>
	/// <param name="tag">モデルタグ名</param>
	/// <param name="world">ワールドトランスフォーム</param>
	void SetData(const std::string& tag, const EulerWorldTransform& world,const Vector4&color={1,1,1,1});

	/// <summary>
	/// タグのモデルに画像を設定
	/// </summary>
	/// <param name="tag">モデルタグ名</param>
	/// <param name="texture">画像の番号</param>
	void SetTexture(const std::string& tag, int texture);

	/// <summary>
	/// ブレンドモードの設定
	/// </summary>
	/// <param name="tag"></param>
	/// <param name="blend">kFillMode</param>
	void SetBlendMode(const std::string& tag, BlendMode blend);

	/// <summary>
	/// 埋めるモード設定
	/// </summary>
	/// <param name="tag">モデルタグ名</param>
	/// <param name="fillMode">kBlend</param>
	void SetFillMode(const std::string& tag, FillMode fillMode);

	/// <summary>
	/// α値設定
	/// </summary>
	/// <param name="tag"></param>
	/// <param name="alpha"></param>
	void SetAlpha(const std::string& tag, float alpha);

	/// <summary>
	/// 画像を有効にするか否か
	/// </summary>
	/// <param name="tag"></param>
	/// <param name="isEnable"></param>
	void SetEnableTexture(const std::string& tag, bool isEnable);

	/// <summary>
	/// シェーダーのONOFF
	/// </summary>
	/// <param name="tag">モデルタグ</param>
	/// <param name="isEnable">正否</param>
	void SetEnableShader(const std::string& tag, bool isEnable);

	/// <summary>
	/// 環境マップの有無
	/// </summary>
	/// <param name="tag"></param>
	/// <param name="isEnable"></param>
	/// <param name="num"></param>
	void SetEnableEnviomentMap(const std::string& tag, bool isEnable, float num = 1.0f);

	/// <summary>
	/// アニメーションのループフレーム変更
	/// </summary>
	/// <param name="tag"></param>
	/// <param name="sec"></param>
	/// <param name="animeLoop"></param>
	void SetAnimationRoopFrame(const std::string& tag,float sec,bool animeLoop=true);

	/// <summary>
	/// アニメーション変更
	/// </summary>
	/// <param name="tag">モデルタグ</param>
	/// <param name="animeName">アニメーション名</param>
	void SetAnimeName(const std::string& tag, const std::string& animeName);

private://**プライベート変数**//

	//PSO
	InstancingPSO* pso_=nullptr;

	//jsonのパス
	const std::string modelPathFile_ = "resources/jsonfile/modelPathFile.json";

	//グループ名
	const std::string groupName_ = "InstancingmodelPathFile";

	//モデルデータ置き場所の必須位置
	const std::string baseFolda_ = "resources/Models/";
 
	//モデルのデータ群
	std::map<std::string, InstancingModel*>modelDatas_;

	//データの最大読み込み量
	const int maxModelNum = 256;

	//モデルの読み込んだ数
	int modelNum = 0;

};