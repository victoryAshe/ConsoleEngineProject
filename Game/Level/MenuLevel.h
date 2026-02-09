#pragma once

#include "Level/Level.h"
#include "Math/Color.h"
#include "Render/Renderer.h"
#include <vector>
#include <string>


using namespace Wanted;

// Menu Item Struct.
struct MenuItem
{
	// Menu 선택됐을 때 실행될 함수의 type.
	using OnSelected = void (*)();

	MenuItem(const char* text, OnSelected onSelected)
		: onSelected(onSelected)
	{
		// Copy text.
		size_t length = strlen(text) + 1;
		this->text = new char[length];
		strcpy_s(this->text, length, text);
	}

	~MenuItem()
	{
		if (text)
		{
			delete[] text;
			text = nullptr;
		}
	}

	// Menu Text.
	char* text = nullptr;

	// Menu 선택됐을 때 실행될 Logic?
	OnSelected onSelected = nullptr;
};


class MenuLevel :public Level
{
	RTTI_DECLARATIONS(MenuLevel, Level)

		// Event 함수 overrdie.
		virtual void Tick(float deltaTime) override;
	virtual void Draw() override;


public:
	MenuLevel();
	~MenuLevel();

private:
	// 현재 활성된 menu item index.
	int currentIndex = 0;


	// 섵개된 item의 색상.
	Color selectedColor = Color::Green;

	// 선택되지 않은 Item의 색상.
	Color unselectedColor = Color::White;

	// Menu Item 배열.
	std::vector<MenuItem*> items;

	// Title용.
	const std::string titleFileName = "Title";
	//shared_ptr<const AsciiArt>
};

