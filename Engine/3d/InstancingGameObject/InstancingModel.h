#pragma once

#include<wrl.h>
#include<string>
#include<vector>

#include"DirectXFunc/DirectXFunc.h"
#include"InstancingGameObject/InstancingPSO.h"
#include"ComputeShaders/SkinningCS.h"

#include"Math/Vector2.h"
#include"Math/Matrix.h"
#include"struct.h"

#include"LightManager/LightManager.h"
#include"GlobalVariable/Tree/GlobalVariableTree.h"

//モデルマネージャの前方宣言
class InstancingModelManager;

//インスタンシングモデルクラス
class InstancingModel {

public://**パブリック変数**//

	//UVのワールド
	EulerWorldTransform uvWorld_;

	//blendMode1
	BlendMode blendMode_ = BlendMode::kNormal;

	//fillMode
	FillMode fillMode_ = FillMode::kSolid;



public:	//**パブリック関数**//

	/// <summary>
	/// デストラクタ
	/// </summary>
	~InstancingModel();

	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

	/// <summary>
	/// OBJ作成
	/// </summary>
	/// <param name="filePath">resources以降のフォルダ</param>
	/// <returns><モデルデータ/returns>
	static InstancingModel* CreateFromOBJ(const std::string& directory, const std::string& filePath, int instancingNum);

	/// <summary>
	/// インスタンシング描画での非共通データを追加する
	/// </summary>
	/// <param name="world">world座標</param>
	/// <param name="color">色</param>
	void AddInstancingData(const EulerWorldTransform& world, const Vector4& color = { 1,1,1,1 });

	/// <summary>
	/// アニメーションのカウント処理
	/// </summary>
	void UpdateAnimationCount();

	/// <summary>
	/// アニメーション更新処理
	/// </summary>
	void UpdateAnimationBone(const std::string& animeName);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="WVP"></param>
	/// <param name="texture"></param>
	void Draw(int texture = -1);




	/// <summary>
	/// 画像を使用するか
	/// </summary>
	/// <param name="ans">画像を使うか</param>
	//void IsEnableTexture(bool ans) { materialData_->enableTexture = ans; }
	void Debug(const char* name);

public:	//**セッター**//

	/// <summary>
	/// シェーダー処理の切り替え
	/// </summary>
	/// <param name="ans">影をつけるか</param>
	void SetEnableShader(bool ans) { materialData_->enableLighting = ans; }

	void SetUV(Matrix4x4 uv) { materialData_->uvTransform = uv; }

	void SetUVTranslate(Vector2 pos) { uvWorld_.translate_.x = pos.x; uvWorld_.translate_.y = pos.y; }

	void SetUVScale(Vector3 scale) { uvWorld_.scale_ = scale; }

	/// <summary>
	/// 色の変更
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(Vector4 color) { materialData_->color = color; }

	void SetAlpha(float alpha) { materialData_->color.w = alpha; }

	void SetBlendMode(BlendMode blend) { blendMode_ = blend; }

	void SetFillMode(FillMode fillmode) { fillMode_ = fillmode; }

	void SetTag(const std::string& tag) { tag_ = tag; }

	void SetTexture(int tex) { setTexture_ = tex; }

	void SetEnableTexture(bool isEnable) { materialData_->enableTexture = isEnable; }

	void SetEnableEnviomentMap(bool enable, float num) { materialData_->enableEnvironmentMap = enable; materialData_->enviromentCoefficient = num; }

	void SetAnimationSecond(float sec, bool loop) { animationRoopSecond_ = sec; isAnimeRoop_ = loop; }

	void SetAnimeName(const std::string& name) { nowAnimeName_ = name; }

public:	//**ゲッター**//

	/// <summary>
	/// 色の取得
	/// </summary>
	/// <returns>色</returns>
	const Vector4 GetColor() { return materialData_->color; }

	const Material* GetMaterialData() { return materialData_; }

	const Vector3 GetUVScale()const { return uvWorld_.scale_; }

	const std::string GetTag() { return tag_; }

	/// <summary>
	/// worldの数取得
	/// </summary>
	const float GetWorldNum();

	/// <summary>
	/// ツリーを作成して取得
	/// </summary>
	/// <param name="tree">ツリー名</param>
	/// <returns></returns>
	GlobalVariableTree& CreateAndGetTree(const std::string& tree);

private: //**プライベート関数**//

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="modelData">モデルデータ</param>
	/// <param name="name"></param>
	/// <param name="point">頂点数</param>
	/// <param name="instancingNum">インスタンシング数</param>
	/// <param name=""></param>
	void Initialize(
		ModelAllData modelData,
		std::string name,
		int point,
		int instancingNum
	);

	//アニメーションの処理
	void ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime);

private: //**プライベート変数**//

	//DirectXFuncのポインタ
	DirectXFunc* DXF_ = nullptr;

	//モデルデータ
	ModelAllData modelData_;

	//スキニング処理
	std::unique_ptr<SkinningCS>skinningCS_;

	//ジョイントの描画データ
	InstancingModelManager* IMM_;
	std::string jointMtag_;
	bool drawJoint_ = false;
	bool drawModel_ = true;

	//タグ
	std::string tag_;

	//画像ハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE texture_;

	//インスタンシングようハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE instancingHandle_;

	//インスタンシングの数
	int instancingNum_;

	//頂点数
	int point_;

	//セットした画像番号
	int setTexture_ = -1;

	//vertexリソースとビュー
	ID3D12Resource* vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	//indexリソースとビュー
	ID3D12Resource* indexResource_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

#pragma region 各追加送信データ
	//wvpリソースとデータ
	ID3D12Resource* wvpResource_;
	WorldTransformation* wvpData_ = nullptr;

	//マテリアルリソースとデータ
	ID3D12Resource* materialResource_;
	Material* materialData_ = nullptr;

	//ディレクショナルライト
	ID3D12Resource* directionalLightResource_;
	DirectionalLight* directionalLightData_ = nullptr;

	//カメラ
	ID3D12Resource* cameraResource_;
	Camera4GPU* cameraData_ = nullptr;

	//ポイントライト
	ID3D12Resource* pointlightResource_;
	PointLight* pointLightData_;
#pragma endregion

#pragma region アニメーション関係
	//アニメーションフラグ
	bool isAnimationActive_ = true;
	float animationTime_ = 0;
	//animationの一周までの秒数
	float animationRoopSecond_ = 1.0f;
	//アニメループフラグ
	bool isAnimeRoop_ = true;

	//現在のアニメの名前
	std::string nowAnimeName_ = "";

	//ボーンのないanimationキューブ用のlocalMatrix
	Matrix4x4 localM_;
#pragma endregion

#pragma region インスタンシング特有のデータ
	//インスタンシングで利用するデータ
	struct InstancingData {
		EulerWorldTransform world;
		Vector4 color;
	};

	//アニメーションデータごとのデータ
	std::vector<InstancingData>instancingDatas_;
#pragma endregion

	//モデルタイプ
	ModelDataType modelType_;

private:
	//ツリーデータ
	GlobalVariableTree tree_ = GlobalVariableTree("");
};
