#pragma once

#include <Windows.h>

namespace MessageEvent
{
	int MessageEvnetOK(const char* title, const char* message)
	{
		return MessageBoxA(nullptr,
			message,
			title,
			MB_OK | MB_ICONSTOP
		);
	}

	int MessageEventIgnore(const char* title, const char* message)
	{
		return MessageBoxA(nullptr,
			message,
			title,
			MB_ABORTRETRYIGNORE | MB_ICONWARNING
		);
	}
}