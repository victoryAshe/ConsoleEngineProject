#include "SokobanGame.h"
#include "Level/SokobanLevel.h"
#include "Level/MenuLevel.h"

#include <iostream>

// 정적 변수 초기화.
SokobanGame* SokobanGame::instance = nullptr;

SokobanGame::SokobanGame()
{
	instance = this;

	// 두 Level 생성 및 배열에 추가.
	levels.emplace_back(new SokobanLevel());
	levels.emplace_back(new MenuLevel());

	// 시작 상태(Level) 설정.
	state = State::GamePlay;

	// Game 시작 시 활성화할 레벨 결정.
	mainLevel = levels[0];
}

SokobanGame::~SokobanGame()
{
	// 중복 제거 방지. (부모 class인 Engine에서.)
	mainLevel = nullptr;

	// 모든 Level 삭제.
	for (Level*& level : levels)
	{
		if (&level)
		{
			delete level;
			level = nullptr;
		}
	}

	// 배열 정리.
	levels.clear();

}

void SokobanGame::ToggleMenu()
{
	// 화면 지우기.
	// system: Console 명령어 실행 함수.
	// cls -> clear screen.
	system("cls");

	// 변경할 index 계산.
	// 현재 활성 레벨 index가 1이면 -> 0으로.
	// 현재 활성 레벨 index가 0이면 -> 1으로.
	int stateIndex = (int)state;
	int nextState = 1 - stateIndex; // one-x.
	state = (State)nextState;		//static_cast.

	// Main Level 변경.
	mainLevel = levels[static_cast<int>(state)];
}

SokobanGame& SokobanGame::Get()
{
	// Exception handling.
	if (!instance)
	{
		std::cerr << "Game::Get() - instance is null.\n";
		__debugbreak();
	}

	// 정적 변수 반환.
	return *instance;
}