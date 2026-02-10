#pragma once
#include "DialogueRow.h"
#include <unordered_map>
#include <string>

// CSV Load
// DialogueID => DialogueRow의 저장소.
class DialogueTable
{
public:
	static bool LoadFromCSV(const std::string& path);
	const static bool TryGet(int dialougeID, DialogueRow& out);

private:
	static int ToInt(const std::string& s);
	static UINT ParseEventID(const std::string& s);

private:
	static std::unordered_map<int, DialogueRow> table;

	static inline std::wstring UTF8toWide(const std::string& utf8);
};

