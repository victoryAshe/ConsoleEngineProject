#pragma once

#include <string>

struct NpcRow
{
	std::string StageName;

	std::wstring npcName;

	int dialogueStartID = 0;
	int dialogueEndID = 0;
};