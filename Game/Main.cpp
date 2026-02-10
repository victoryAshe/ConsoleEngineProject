#include <iostream>
#include "Engine/Engine.h"

// Shooting Game.
#include "Level/ShootingGameLevel.h"

// Sokoban Game.
#include "GameManager/SokobanGame.h"

using namespace Wanted;

// Check Memory Leak in DebugMode.
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	// 제목 설정. 나중에 엔진으로 넘겨주기.
	SetConsoleTitle(L"ConsoleGame");

	//Engine engine;
	//engine.SetNewLevel(new ShootingGameLevel());
	//engine.Run();

	SokobanGame game;
	game.Run();
	
	return 0;
}