#pragma once
#include <iostream>
namespace Util
{

#define READ_MODE "rt" //rt: read-text mode.
#define WRITE_MODE "wt" //wt: write-text mode.

	class FileIO
	{
	public:
		// File*: void*
		// file을 다룰 수 있도록 하는 pointer로, file 그 자체는 아님.
		inline FILE* OpenFile(const char* PATH, const char* MODE);

		inline void CloseFile(FILE* file);

		inline void WriteFile(const char* PATH, const char* message);

	private:
		// File Size를 가늠해보기.
		inline size_t FileSize(FILE*& file);
	};


}