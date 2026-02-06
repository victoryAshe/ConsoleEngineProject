#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

class SokobanPlayer : public Actor
{
	RTTI_DECLARATIONS(SokobanPlayer, Actor)

public:
	SokobanPlayer(const Vector2& position);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;
};

