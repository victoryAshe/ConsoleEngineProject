#include "MenuLevel.h"
#include "GameManager/SokobanGame.h"
#include "Core/Input.h"

MenuLevel::MenuLevel()
{
	// Create Menu Item.
	items.emplace_back(new MenuItem(
		"Resume Game",
		[]()
		{
			// Menu Toggle 함수 호출.
			SokobanGame::Get().ToggleMenu();
		}
	));

	items.emplace_back(new MenuItem(
		"Quit Game",
		[]()
		{
			SokobanGame::Get().QuitEngine();
		}
	));

}

MenuLevel::~MenuLevel()
{
	for (MenuItem*& item : items)
	{
		delete item;
		item = nullptr;
	}

	// 배열 초기화.
	items.clear();
}


void MenuLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 입력 처리 (방향키 위/아래, 엔터, ESC).
	// 배열 길이.
	static int length = static_cast<int>(items.size());
	if (Input::Get().GetKeyDown(VK_UP))
	{
		// index 돌리기.
		currentIndex = (currentIndex - 1 + length) % length;
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		// index 돌리기.
		currentIndex = (currentIndex + 1) % length;
	}

	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		// MenuItem이 저장한 함수 포인터 호출.
		items[currentIndex]->onSelected();
	}

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// Menu Toggle.
		SokobanGame::Get().ToggleMenu();

		// index 초기화.
		currentIndex = 0;
	}
}

void MenuLevel::Draw()
{
	// 메뉴 제목 출력.
	//Util::SetConsolePosition(Vector2::Zero);
	//Util::SetConsoleTextColor(Color::White);
	//
	//// Print Text.
	//std::cout << "Sokoban Game\n\n";

	Renderer::Get().Submit("Sokoban Game", Vector2::Zero);

	// Print Menu Item.
	for (int ix = 0; ix < static_cast<int>(items.size()); ++ix)
	{
		// 아이템 색상 확인 (선택됐는지 여부).
		Color textColor =
			(ix == currentIndex) ? selectedColor : unselectedColor;

		Renderer::Get().Submit(
			items[ix]->text,
			Vector2(0, 2 + ix),
			textColor
		);

		// 색상 설정.
		//Util::SetConsoleTextColor(textColor);

		// Print Text.
		//std::cout << items[ix]->text<<"\n";
	}
}
