#pragma once

#include "Actor/Actor.h"
#include "Util/Timer.h"

using namespace Wanted;

class EnemySpawner:public Actor
{
	RTTI_DECLARATIONS(EnemySpawner, Actor)

public:
	EnemySpawner();

private:
	virtual void Tick(float deltaTime) override;

	void SpawnEnemy(float deltaTime);

private:
	// »ý¼º Timer.
	Timer timer;
};

