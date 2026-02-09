#pragma once

#include "Actor/Actor.h"

#include <unordered_map>
#include <string>

using namespace Wanted;

class NPC : public Actor
{
	RTTI_DECLARATIONS(NPC, Actor)

public:
	NPC(const Vector2& newPosition, 
		const char* npcName,
		const char* title,
		const char* message
	);

	void PlayCollisionEvent();

private:
	

private:
	// 나중에 Level에서 생성 시 npcName을 통해 불러옴.
	std::unordered_map<UINT, const char*> actions;

	const char* npcName;
	const char* title;
	const char* message;
};

