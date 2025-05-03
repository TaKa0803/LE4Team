#pragma once
#include"ProtItem/Player/behavior/IProtBehavior.h"

class PlayerAttackManager : public IProtBehavior {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerAttackManager();
	~PlayerAttackManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
};