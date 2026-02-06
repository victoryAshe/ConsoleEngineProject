#pragma once
#include "Engine/Engine.h"

#include <vector>

using namespace Wanted;

// Enum for Level Management.
enum class State
{
	//None = -1,
	GamePlay = 0,
	Menu = 1,
	Length
};

class SokobanGame : public Engine
{
public:
	SokobanGame();
	~SokobanGame();

	//  Menu/Game Level을 전환하는 함수.
	void ToggleMenu();

	static SokobanGame& Get();

private:

	//Game에서 관리하는 Level을 저장할 배열.
	std::vector<Level*> levels;

	// 현재 활성화된 Level을 나타내는 상태 변수
	State state = State::GamePlay;

	// Singleton 구현을 위한 정적 변수.
	static SokobanGame* instance;


};

