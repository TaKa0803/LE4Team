#include "MainSystem.h"

#include"RandomNum/RandomNum.h"
#include"AudioManager/AudioManager.h"
#include"RTVManager/RTVManager.h"
#include"DSVManager/DSVManager.h"
#include"SingleGameObjects/ObjectPSO.h"
#include"DXC/DXCManager.h"
#include"AppScene/AppScene.h"
#include"SpriteManager/SpriteManager.h"
#include"PostEffect/PostEffectManager/PostEffectManager.h"
#include"UAVManager/UAVManager.h"
#include"GlobalVariable/Manager/GlobalVaribleManager.h"



MainSystem* MainSystem::GetInstance() {
	//インスタンス取得
	static MainSystem instance;
	return &instance;
}

void MainSystem::Run() {
	HRESULT hr;
	hr= CoInitializeEx(0, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));
	//エンジン初期化処理
	Initializes();

	//更新処理
	MainRoop();

	//終了処理
	Finalize();

	//終わり
	CoUninitialize();
}

void MainSystem::Initializes() {
	//windowsアプリケーション
	winApp_ = WindowApp::GetInstance();
	winApp_->Initialize(L"ウイルス討伐作戦！",1280,720);

	//DXCマネージャ
	DXCManager* DXC = DXCManager::GetInstance();
	DXC->Initialize();

	//DirectXマネージャ初期化
	DXF_ = DirectXFunc::GetInstance();
	DXF_->Initialize(winApp_);

	//SRVインスタンス取得
	SRVM_ = SRVManager::GetInstance();
	UAVManager::GetInstance()->Initialize();

	//RTVManager::GetInstance()->Initialize();

	//画像関係
	textureManager_= TextureManager::GetInstance();
	textureManager_->Initialize(DXF_);

	//ポストエフェクトマネージャの初期化
	PostEffectManager::GetInstance()->Initialize();

	//imgui
	imguiManager_ = ImGuiManager::GetInstance();
	imguiManager_->Initialize(winApp_, DXF_);

	//入力
	input_ = Input::GetInstance();
	input_->Initialize(winApp_);
	
	//インスタンシングモデル
	instancingMM_ = InstancingModelManager::GetInstance();

	//乱数クラス
	randomNumClass_ = RandomNumber::GetInstance();
	randomNumClass_->RandomNumberProcessInitialize();
	
	//デルタタイム
	deitaTimer_ = DeltaTimer::GetInstance();
	deitaTimer_->Initialize();

	//音声マネージャ
	AudioManager *audioManager = AudioManager::GetInstance();
	audioManager->Initialize();
	audioManager->LoadAllSoundData();

	//モデルマネージャ初期化
	ModelManager::GetInstance()->Initialize();
	//スプライトマネージャ初期化
	SpriteManager::GetInstance()->Initialize();


}

void MainSystem::MainRoop() {


	//モデルデータ関係読み込み
	ModelManager::GetInstance()->LoadAllModels();
	instancingMM_->Initialize(true);

	//ゲームシーン初期化
	std::unique_ptr<AppScene> gameScene_;
	gameScene_ = std::make_unique<AppScene>();
	gameScene_->Initialize();

	//メインループ
	while (winApp_->ProcessMessage()) {

		//デルタタイマー更新
		deitaTimer_->Update();

#pragma region 状態更新
		///更新前処理
		//ImGui
		imguiManager_->PreUpdate();

		//キー入力
		input_->Update();
		///=以下更新=//

		//Gvariの更新
		GlobalVariableManager::GetInstance()->Update();

		//ゲームシーン更新
		gameScene_->Update();


		//==更新終わり==//

		//更新終わり描画前処理
		imguiManager_->PostUpdate();
#pragma endregion
#pragma region 描画		
		///描画前処理
		//DirectX
		DXF_->PreDraw();	
		//ImGui
		imguiManager_->PreDraw();

		//==以下描画==//	
		gameScene_->Draw();
		//==描画終わり==//

		///描画あと処理
		//imGui
		imguiManager_->PostDraw();
		//DirectX
		DXF_->PostDraw();

		//読み込んだ画像をGPUに送信して削除
		SRVM_->PostInitialize();
#pragma endregion
		
		//シーン終了フラグがtrueの時
		if (IScene::GetLeaveScene()) {
			//シーンを終わる
			break;
		}
	}

}


void MainSystem::Finalize() {

	//解放処理
	SpriteManager::GetInstance()->Finalize();
	ModelManager::GetInstance()->Finalize();
	///開放処理
	instancingMM_->Finalize();
	SRVM_->Finalize();
	UAVManager::GetInstance()->Finalize();
	RTVManager::GetInstance()->Finalize();
	DSVManager::GetInstance()->Finalize();
	PostEffectManager::GetInstance()->Finalize();

	textureManager_->Finalize();
	imguiManager_->Finalize();
	DXF_->Finalize();
	winApp_->Finalize();

}


