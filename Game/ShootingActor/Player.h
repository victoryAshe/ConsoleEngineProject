#pragma once

#include "Actor/Actor.h"
#include "Util/Timer.h"

using namespace Wanted;

class Player : public Actor
{
	// 발사 모드.
	enum class FireMode
	{
		None = -1,
		OneShot,
		Repeat
	};

	// Timer 변수.
	Timer timer;

	RTTI_DECLARATIONS(Player, Actor)

public:
	Player();
	~Player();

private:
	virtual void Tick(float deltaTime) override;

	// 왼쪽으로 이동하는 함수.
	void MoveLeft();

	// 오른쪽으로 이동하는 함수.
	void MoveRight();

	// 탄약 발사 함수.
	void Fire();

	// 연속 발사 함수.
	void FireInterval();

	// 발사 가능 여부 확인 함수.
	bool CanShoot() const;

private:
	// 발사 모드.
	FireMode fireMode = FireMode::None;

	// 연사 시간 간격.
	float fireInterval = 0.2f;
};

