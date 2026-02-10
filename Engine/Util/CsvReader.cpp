#include "CsvReader.h"
#include <fstream>

namespace Wanted
{
	void CsvReader::TrimCR(std::string& s)
	{
		if (!s.empty() && s.back() == '\r') s.pop_back();
	}

	std::vector<std::string> CsvReader::ParseLine(const std::string& inLine)
	{
		std::string line = inLine;
		TrimCR(line);

		std::vector<std::string> fields;
		std::string field;

		bool inQuotes = false;
		for (size_t i = 0; i < line.size(); ++i)
		{
			char c = line[i];

			if (!inQuotes)
			{
				if (c == '"')
				{
					// quoted field: 감싸는 "는 결과에 안 넣기.
					inQuotes = true;
				}
				else if (c == ',')
				{
					fields.push_back(field);
					field.clear();
				}
				else
				{
					field.push_back(c);
				}
			}
			else
			{
				// === quoted field 내부. ===
				if (c == '"')
				{
					if (i + 1 < line.size() && line[i + 1] == '"')
					{
						// field를 감싸는 따옴표가 아니라 escape 복원.
						field.push_back('"');
						++i; // 두번째 " 소비.
					}
					else
					{
						// quoted field 종료. (감싸는 "는 포함 x).
						inQuotes = false;
					}
				}
				else
				{
					// quotes field 내부의 ,는 구분자가 아니므로 그냥 데이터.
					field.push_back(c);
				}
			}
		}
		fields.push_back(field);


		return fields;
	}

	bool CsvReader::ReadAll(
		const std::string& path, 
		std::vector<std::vector<std::string>>& outRows
	)
	{
		std::ifstream file(path);
		if (!file.is_open()) return false;

		outRows.clear();

		std::string line;
		while (std::getline(file, line))
		{
			TrimCR(line);
			if (line.empty()) continue;

			outRows.push_back(ParseLine(line));
		}

		return true;
	}
}