#include "DialogueTable.h"
#include "Util/CsvReader.h"

std::unordered_map<int, DialogueRow> DialogueTable::table;

int DialogueTable::ToInt(const std::string& s)
{
    return std::atoi(s.c_str());
}

UINT DialogueTable::ParseEventID(const std::string& s)
{
    const char* begin = s.c_str();
    char* end = nullptr;
    unsigned long val = std::strtoul(begin, &end, 0);

    return static_cast<UINT>(val);
}

std::wstring DialogueTable::UTF8toWide(const std::string& utf8)
{
    if (utf8.empty()) return std::wstring();

    int size = MultiByteToWideChar(
        CP_UTF8,
        0,
        utf8.c_str(),
        -1,
        nullptr,
        0
    );

    std::wstring wide(size - 1, L'\0');

    MultiByteToWideChar(
        CP_UTF8,
        0,
        utf8.c_str(),
        -1,
        &wide[0],
        size
    );

    return wide;
}

bool DialogueTable::LoadFromCSV(const std::string& path)
{
    std::vector<std::vector<std::string>> rows;
    if (!Wanted::CsvReader::ReadAll(path, rows)) return false;


    bool skipHeader = true;
    for (const std::vector<std::string>& fields : rows)
    {
        if (fields.empty()) continue;

        if (skipHeader)
        {
            skipHeader = false;
            continue;
        }

        // dialogueID, EventID, script, character, JumpAnswer1, JumpAnsewer2, JumpAnswer3.
        if (fields.size() < 7) continue;

        DialogueRow row;
        row.dialogueID = ToInt(fields[0]);
        row.eventID = ParseEventID(fields[1]);
        row.script = UTF8toWide(fields[2]);
        row.character = UTF8toWide(fields[3]);
        row.jump1 = ToInt(fields[4]);
        row.jump2 = ToInt(fields[5]);
        row.jump3 = ToInt(fields[6]);

        table[row.dialogueID] = row;
    }

    return true;
}

const bool DialogueTable::TryGet(int dialougeID, DialogueRow& out)
{
    auto it = table.find(dialougeID);
    if (it == table.end()) return false;

    out = it->second;
    return true;
}
