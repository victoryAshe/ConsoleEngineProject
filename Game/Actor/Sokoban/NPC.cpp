#include "NPC.h"
#include "Level/Level.h"

#include "Util/MessageEvent.h"

#include "Dialogue/DialogueSession.h"


NPC::NPC(const Vector2& newPosition,
	const char* npcName,
	const char* dialogueCsvPath,
	int startDialougeID)
	:super("N", newPosition, Color::Red), 
	npcName(npcName ? npcName : ""), 
	dialogueCsvPath(dialogueCsvPath), 
	startDialogueID(startDialougeID)
{
	// Set SortingOrder
	sortingOrder = 8;
}

void NPC::PlayCollisionEvent()
{
	// 이미 대화 중이면 재발동 금지.
	if (isDialogueActive) return;

	StartDialogueSession();
}


void NPC::StartDialogueSession()
{
	Level* level = GetOwner();
	if (!level) return;

	isDialogueActive = true;

	// 1회성 session Actor를 생성해서 Level에 추가.
	// Session이 끝나면 NPC->NotifyDialogueEnded() 호출 후 자기자신을 Destroy.
	level->AddNewActor(new DialogueSession(this, dialogueCsvPath, startDialogueID));
}

void NPC::NotifyDialogueEnded()
{
	isDialogueActive = false;
}


