#include "FileIO.h"

namespace Util
{
	FILE* FileIO::OpenFile(const char* PATH, const char* MODE)
	{
		FILE* file = nullptr;
		fopen_s(&file, PATH, MODE);

		// Failed to read file.
		if (file == nullptr)
		{
			std::cout << "Failed to read file.\n";
			__debugbreak();
		}

		return file;
	}

	void FileIO::CloseFile(FILE* file)
	{
		fclose(file);
		file = nullptr;
	}

	void FileIO::WriteFile(const char* PATH, const char* message)
	{
		FILE* file = OpenFile(PATH, WRITE_MODE);

		fputs(message, file);

		CloseFile(file);
	}

	size_t FileIO::FileSize(FILE*& file)
	{
		fseek(file, 0, SEEK_END);
		size_t fileSize = ftell(file);

		// ! File Size 가늠해본 뒤에, 다시 처음부터 읽으려면 FP 되돌려야 함.
		rewind(file);

		return fileSize;
	}
}