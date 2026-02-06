#pragma once

#include "Math/Vector2.h"

class ICanPlayerMove
{
public:

	// Player가 이동 가능한지 문의할 때 사용할 함수.
	// playerPosition: Player의 현재 위치.
	// nextPosition: Player가 이동하려는 위치.
	virtual bool CanMove(
		const Wanted::Vector2& playerPosition,
		const Wanted::Vector2& nextPosition
	) = 0;
};