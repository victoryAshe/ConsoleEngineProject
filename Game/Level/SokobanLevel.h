#pragma once
#include "Level/Level.h"
#include "Actor/Sokoban/SokobanPlayer.h"
#include "Interface/ICanPlayerMove.h"

class SokobanLevel
	: public Wanted::Level,
	public ICanPlayerMove
{
	RTTI_DECLARATIONS(SokobanLevel, Level)

public:
	SokobanLevel();

	// Event 함수 override.
	virtual void Draw() override;
private:
	// Load Game Map.
	void LoadMap(std::string& mapName);

	// ICanPlayerMove을(를) 통해 상속됨
	virtual bool CanMove(
		const Wanted::Vector2& playerPosition,
		const Wanted::Vector2& nextPosition) override;

	// Game Clear 확인 함수.
	bool CheckGameClear();

private:
	// 획득해야하는 목표 점수.
	int targetScore = 0;

	// Game Clear 여부를 알려주는 변수.
	bool isGameClear = false;
};

