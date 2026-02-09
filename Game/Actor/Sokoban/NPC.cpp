#include "NPC.h"

#include "Util/MessageEvent.h"

NPC::NPC(const Vector2& newPosition,
	const char* npcName,
	const char* title,
	const char* message)
	:super("N", newPosition, Color::Red), 
	npcName(npcName), title(title), message(message)
{
	// Set SortingOrder
	sortingOrder = 8;
}

void NPC::PlayCollisionEvent()
{
	int ok = MessageEvent::MessageEvnetOK(title, message);
	if (ok == IDOK)
	{
		MessageEvent::MessageEvnetOK("확인", "아니다, 지나가.");
		Destroy();
	}
}

