#include "SokobanLevel.h"
#include "Actor/Sokoban/SokobanPlayer.h"
#include "Actor/Sokoban/Wall.h"
#include "Actor/Sokoban/Ground.h"
#include "Actor/Sokoban/Box.h"
#include "Actor/Sokoban/Target.h"
#include "NPC/NPC.h"
#include "Util/Util.h"
#include "Util/PathUtil.h"
#include "Common/RTTI.h"

#include <iostream>


SokobanLevel::SokobanLevel()
{
	std::string mapName = "Stage1";
	LoadMap(mapName);
}

void SokobanLevel::Draw()
{
	super::Draw();

	// Game Clear인 경우, Message 출력.
	if (isGameClear)
	{
		// Set Console Position && Color.
		Util::SetConsolePosition(Vector2(30, 0));
		Util::SetConsoleTextColor(Color::White);

		// Print Game Clear message.
		std::cout << "Game Clear!";
	}
}

void SokobanLevel::LoadMap(std::string& mapName)
{
	// Load File.
	// 최종 경로 만들기.
	std::string exePath = PathUtil::GetExeDir();
	std::string path = exePath;
	path = PathUtil::JoinPath(path, "Stage");
	path = PathUtil::JoinPath(path, mapName + ".txt");

	std::string dialoguePath = exePath;
	dialoguePath = PathUtil::JoinPath(dialoguePath, "Dialogue");
	dialoguePath = PathUtil::JoinPath(dialoguePath, mapName+".csv");

	// Open File.
	FILE* file = nullptr;
	fopen_s(&file, path.c_str(), "rt");

	// null check
	if (!file)
	{
		// use std error console 
		std::cerr << "Failed to open map file.\n";

		// stop int debugMode if a error occurs
		__debugbreak();
	}

	// Read map.
	// read map size
	fseek(file, 0, SEEK_END);

	// 마지막 위치에 대한 사이즈 읽기.
	size_t fileSize = ftell(file);

	// File Position 되돌리기.
	rewind(file);

	// Create buffer to read data from file.
	char* data = new char[fileSize + 1];

	//Read data
	fread(data, sizeof(char), fileSize, file);

	// 읽어온 문자열을 분석(Parsing)해서 출력
	// index를 사용해 한 문자씩 읽기.
	int index = 0;

	// 객체를  생성할 위치 값.
	Wanted::Vector2 position(0, 0);

	while (index < fileSize)
	{
		// Read char.
		char mapCharacter = data[index];
		++index;

		if (mapCharacter == '\n')
		{
			//std::cout << "\n";
			// 개행: y좌표는 하나 늘리고, x좌표 초기화.
			++position.y;
			position.x = 0;
			continue;
		}

		/*
		* [6 kinds of actor]
		#: Wall
		.: Ground
		p: Player
		b: Box
		t: Target
		n: NPC
		*/
		// 한 문자씩 처리.
		switch (mapCharacter)
		{
		case '#':
		case '1':
			AddNewActor(new Wall(position));
			break;

		case '.':
			AddNewActor(new Ground(position));
			break;

		case 'p':
			// Player: Movable
			AddNewActor(new SokobanPlayer(position));
			AddNewActor(new Ground(position));
			break;

		case 'b':
			// Box: Movable
			// 박스가 옮겨졌을 때, 그 밑에 땅이 있어야 함.
			AddNewActor(new Box(position));
			AddNewActor(new Ground(position));
			break;

		case 't':
			AddNewActor(new Target(position));
			++targetScore;
			break;

		case 'S':
			// NPC: Deletable
			// NPC가 사라졌을 때, 그 밑에 땅이 있어야 함!
			// TODO: case에 따라 NPC 설정파일 Load해서 가져오는 것으로 변경.
			AddNewActor(new NPC(position, "스피드웨건", dialoguePath.c_str(), 1, 2));
			AddNewActor(new Ground(position));
			break;
		}

		// x 좌표 증가 처리.
		++position.x;
	}

	// Free buffer already have been used
	delete[] data;

	// Close if successfuly opened.
	fclose(file);
}

bool SokobanLevel::CanMove(
	const Wanted::Vector2& playerPosition,
	const Wanted::Vector2& nextPosition)
{

	// Level에 있는 Box Actor 모으기.
	// +) NPC.
	// Box는 Player가 밀기 등 추가적으로 처리해야하기 때문.
	std::vector<Actor*> boxes;
	std::vector<NPC*> npcs;

	// Level에 배치된 전체 Actor를 순회하면서 Box 찾기.
	for (Actor* const actor : actors)
	{
		// Actor가 Box type인지 확인.
		if (actor->IsTypeOf<Box>())
		{
			boxes.emplace_back(actor);
			continue;
		}
		if (actor->IsTypeOf<NPC>())
		{
			npcs.emplace_back(dynamic_cast<NPC*>(actor));
			continue;
		}
	}

	// 이동하려는 위치에 NPC가 있는지 확인.
	for (NPC* const npc : npcs)
	{
		// 위치 비교
		// 새 위치에 NPC가 있으면: 이동 불가.
		if (npc->GetPosition() == nextPosition)
		{
			npc->PlayCollisionEvent();
			return (!npc || npc->DestroyRequested());
		}
	}

	// 이동하려는 위치에 Box가 있는지 확인.
	Actor* boxActor = nullptr;
	for (Actor* const box : boxes)
	{
		// 위치 비교
		if (box->GetPosition() == nextPosition)
		{
			boxActor = box;
			break;
		}
	}

	// 경우의 수 처리.
	// 1. 이동하려는 곳에 Box가 있는 경우.
	if (boxActor)
	{
		// #1. Box를 이동시키려는 위치에 다른 Box가 또 있는지 확인.
		// 두 위치 사이에서 이동 방향 구하기. (Vector의 뺄셈 활용.)
		// 이동 logic에서 두 vector를 더한다는 것은
		// 둘 중 하나는 Position이고, 다른 하나는 방향 vector. 
		//Vector2 nextBoxPosition(nextPosition.x * 2 - playerPosition.x, nextPosition.y * 2 - playerPosition.y);
		// 위는 축약형. 명확히 하기 위해 아래와 같이 기술.
		Vector2 direction = nextPosition - playerPosition;
		Vector2 newPosition = boxActor->GetPosition() + direction;


		// Search Box.
		for (Actor* const ohterBox : boxes)
		{
			// 앞의 박스와 같다면 건너뛰기.
			if (ohterBox == boxActor)
			{
				continue;
			}

			// 다른 박스가 있는지 확인.
			if (ohterBox->GetPosition() == newPosition)
			{
				// 두 박스가 겹쳐진 방향으로는 이동 못함.
				return false;
			}
		}

		// Search.
		for (Actor* const actor : actors)
		{
			if (actor->GetPosition() == newPosition)
			{
				// #2. Wall || NPC이면 이동 불가.
				if (actor->IsTypeOf<Wall>()
					|| actor->IsTypeOf<NPC>())
				{
					return false;
				}

				// #3: Ground || Target.
				if (actor->IsTypeOf<Ground>()
					|| actor->IsTypeOf<Target>())
				{
					// 박스 이동 처리.
					boxActor->SetPosition(newPosition);

					// TODO: Game score 확인.
					isGameClear = CheckGameClear();

					// Player 이동 가능.
					return true;
				}
			}
		}

	}


	// 2. 이동하려는 곳에 Box가 없는 경우.
	// -> 이동하려는 곳에 있는 Actor가 Wall이 아니면 이동 가능.
	for (Actor* const actor : actors)
	{
		// 먼저 이동하려는 위치의 Actor 검색
		if (actor->GetPosition() == nextPosition)
		{
			// 이 Actor가 Wall인지 확인
			if (actor->IsTypeOf<Wall>())
			{
				return false;
			}

			// Ground || Target.
			return true;
		}
	}

	// Other.
	return true;
}

bool SokobanLevel::CheckGameClear()
{
	// Target 위에 있는 Box의 수 검증.
	int currentScore = 0;

	// 배열에 Box 및 Target 저장.
	std::vector<Actor*> boxes;
	std::vector<Actor*> targets;

	// Level에 배치된 배열 순회하면서 두 Actor filtering.
	for (Actor* const actor : actors)
	{
		// Box인 경우 boxes 배열에 추가.
		if (actor->IsTypeOf<Box>())
		{
			boxes.emplace_back(actor);
			continue;
		}

		// Target인 경우 targets 배열에 추가.
		if (actor->IsTypeOf<Target>())
		{
			targets.emplace_back(actor);
		}
	}

	// 점수 확인 (Box의 위치가 Target의 위치와 같은지 비교.)
	for (Actor* const box : boxes)
	{
		for (Actor* const target : targets)
		{
			// 두 Actor의 위치가 같으면 점수 +.
			if (box->GetPosition() == target->GetPosition())
			{
				currentScore += 1;
			}
		}
	}

	// 목표 점수에 도달했는지 확인.
	return currentScore == targetScore;
}
