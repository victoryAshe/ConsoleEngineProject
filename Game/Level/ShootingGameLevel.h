#pragma once

#include "Level/Level.h"
#include "Math/Vector2.h"

using namespace Wanted;

class ShootingGameLevel: public Level
{
	RTTI_DECLARATIONS(ShootingGameLevel, Level)

public:
	ShootingGameLevel();
	~ShootingGameLevel();

private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	// 충돌 판정 처리 함수.
	void ProcessCollisionPlayerBulletAndEnemy();
	void ProcessCollisionPlayerAndEnemyBullet();

	void ShowScore();

private:
	// 점수 변수.
	int score = 0;

	// Player가 죽었는지 확인.
	bool isPlayerDead = false;

	// Plaryer가 죽은 위치 (Draw에서 처리하기 위해 Tick에서 저장).
	Vector2 playerDeadPosition;

	// 점수 문자열.
	char scoreString[128] = {};
};

