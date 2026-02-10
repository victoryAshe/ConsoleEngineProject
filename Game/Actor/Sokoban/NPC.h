#pragma once

#include "Actor/Actor.h"

#include <string>

using namespace Wanted;

class NPC : public Actor
{
	RTTI_DECLARATIONS(NPC, Actor)

public:
	NPC(const Vector2& newPosition, 
		const char* npcName,
		const char* dialogueCsvPath,
		int startDialougeID
	);

	// Player-NPC Collision event.
	void PlayCollisionEvent();

	// DialogueSession이 종료될 때 호출(중복 실행 방지 Flag 해제).
	void NotifyDialogueEnded();

private:
	void StartDialogueSession();
	

private:
	std::string npcName;

	// Related Dialogue Data.
	std::string dialogueCsvPath;
	int startDialogueID = 0;

	// Dialgoue 중복 발동 방지용.
	bool isDialogueActive = false;
};


