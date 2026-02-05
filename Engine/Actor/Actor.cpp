#include "Actor.h"
#include "Util/Util.h"
#include "Render/Renderer.h"
#include "Engine/Engine.h"

#include <iostream>
#include <Windows.h>

namespace Wanted
{
	Actor::Actor(const char* image, 
		const Vector2& position,
		Color color)
		: position(position), color(color)
	{
		// copy string.
		width = static_cast<int>(strlen(image));
		this->image = new char[width + 1];
		strcpy_s(this->image, width + 1, image);
	}

	Actor::~Actor()
	{
		// Free memory.
		SafeDeleteArray(image);
	}

	void Actor::BeginPlay() 
	{
		// Event 받은 후에는 flag 설정.
		hasBeganPlay = true;
	}

	void Actor:: Tick(float deltaTime)
	{
	}

	void Actor:: Draw()
	{
		// Renderer에 그리기 요청.
		//Renderer::Draw(position, color, image);

		// Renderer에 Data 제출.
		Renderer::Get().Submit(image, position, color, sortingOrder);
	}


	void Actor::Destroy()
	{
		// 삭제 flag 설정.
		destroyRequested = true;

		// 삭제 event 호출.
		OnDestroy();
	}

	void Actor::OnDestroy()
	{ }

	void Actor::QuitGame()
	{
		// Engine 종료 요청.
		Engine::Get().QuitEngine();
	}

	bool Actor::TestIntersect(const Actor* const other)
	{
		// AABB (Axis Aligned Bounding Box).
		// 원래는 시작점하고 끝점해서 2차원~3차원 계산함
		// 여기서는: x 좌표만 고려하면 됨! y는 크기가 1이기 때문.

		// 자기 자신의 x좌표 정보.
		int xMin = position.x;
		int xMax = position.x + width - 1;

		// 충돌을 비교할 다른 Actor의 x좌표 정보.
		int otherXMin = other->GetPosition().x;
		int ohterXMax = other->position.x 
			+ other->width - 1;

		// 안 겹치는 조건 확인.
		// 내 오른쪽 좌표보다 더 오른쪽에 있는 경우.
		if (otherXMin > xMax)
		{
			return false;
		}

		// 내 왼쪽 좌표보다 더 왼쪽에 있는 경우.
		if (ohterXMax < xMin)
		{
			return false;
		}

		// y는 크기가 1이므로, 좌표가 같은지 여부만 확인.
		return position.y == other->position.y;
	}

	void Actor::ChangeImage(const char* newImage)
	{
		// 기존 memory 해제.
		SafeDeleteArray(image);

		// 새로운 문자열 복사.
		width = static_cast<int>(strlen(newImage));
		image = new char[width + 1];
		strcpy_s(image, width+1, newImage);
	}

	void Actor::SetPosition(const Vector2& newPosition)
	{
		// Renderer에 빈칸 그리기 요청.
		//Renderer::Draw(position, ' ');
		
		// 변경하려는 위치가 현재 위치와 같으면 건너뜀.
		if(position == newPosition)
		{
			return;
		}

		// 새로운 위치 설정.
		position = newPosition;
	}
}