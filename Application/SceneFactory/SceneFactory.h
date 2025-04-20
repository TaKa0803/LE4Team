#pragma once
#include"IScene/IScene.h"

#pragma region 各シーンクラス
#include"Debug/DebugScene.h"
#include"ProtItem/Scenes/ProtScene.h"
#pragma endregion

#include<iostream>

//**現在未使用。エンジンの調整後利用する。

//番号に合ったシーンの生成処理
class SceneFactory {

public://**パブリック関数**//
	
	/// <summary>
	/// 指定したシーンクラスを読み込む
	/// </summary>
	/// <param name="scene">シーン名</param>
	/// <returns>シーンのユニークポインタ</returns>
	static std::unique_ptr<IScene>CreateScene(SCENE scene) {
		switch (scene)
		{
		case SCENE::Debug://デバッグシーンを返す処理
			return std::make_unique<DebugScnene>();

		case SCENE::Prot://デバッグシーンを返す処理
			return std::make_unique<ProtScene>();


		case SCENE::SceneCount:
			return nullptr;

		default:
			return nullptr;
		}
	}

};