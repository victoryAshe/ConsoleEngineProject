#include "Renderer.h"
#include "ScreenBuffer.h"
#include "Util/Util.h"

namespace Wanted
{
	Renderer::Frame::Frame(int bufferCount)
	{
		// 배열 생성 및 초기화.
		charInfoArray = new CHAR_INFO[bufferCount];
		memset(charInfoArray, 0, sizeof(CHAR_INFO) * bufferCount);
		//ZeroMemory(charInfoArray, sizeof(CHAR_INFO) * bufferCount); // 위와 동일한 기능.

		sortingOrderArray = new int[bufferCount];
		memset(sortingOrderArray, 0, sizeof(int) * bufferCount);
	}

	Renderer::Frame::~Frame()
	{
		SafeDeleteArray(charInfoArray);
		SafeDeleteArray(sortingOrderArray);
	}

	void Renderer::Frame::Clear(const Vector2& screenSize)
	{
		// 2차원 배열로 다루는 1차원 배열을 순회하면서
		// 빈 문자(' ')를 설정!
		const int width = screenSize.x;
		const int height = screenSize.y;
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				// 배열 index 구하기.
				const int index = (y * width) + x;

				// 글자 값 및 속성 설정.
				CHAR_INFO& info = charInfoArray[index];
				info.Char.AsciiChar = ' ';
				info.Attributes = 0;

				// sortingOrder 초기화.
				sortingOrderArray[index] = -1;
			}
		}
	}

	// ==================== Frame ============================ //

	// 정적 변수 초기화.
	Renderer* Renderer::instance = nullptr;

	Renderer::Renderer(const Vector2& screenSize)
		: screenSize(screenSize)
	{
		instance = this;

		// Create instance of Frame struct
		const int bufferCount = screenSize.x * screenSize.y;
		frame = new Frame(bufferCount);

		// Initialize frame.
		frame->Clear(screenSize);

		// Create instance of Double Buffer
		screenBuffers[0] = new ScreenBuffer(screenSize);
		screenBuffers[0]->Clear();

		screenBuffers[1] = new ScreenBuffer(screenSize);
		screenBuffers[1]->Clear();
	}

	Renderer::~Renderer()
	{
		SafeDelete(frame);
		for (ScreenBuffer*& buffer : screenBuffers)
		{
			SafeDeleteArray(buffer);
		}
	}

	Renderer& Renderer::Get()
	{
		if (!instance)
		{
			MessageBoxA(
				nullptr,
				"Renderer::Get() - instance is null.",
				"Error",
				MB_OK
			);

			__debugbreak();
		}

		return *instance;
	}

	void Renderer::Clear()
	{
		// Clear Screen.
		// 1. Frmae(2차원 배열 데이터) 지우기.
		frame->Clear(screenSize);

		// 콘솔 버퍼 지우기.
		GetCurrentBuffer()->Clear();
	}

	void Renderer::Draw()
	{
		// Clear Screen.
		Clear();

		// 전제 조건: Level의 모든 Actor가 Renderer에 Submit을 완료해야 함!
		// RenderQueue를 순회하면서 Frame 채우기.
		for (const RenderCommand& command : renderQueue)
		{
			// 화면에 그릴 text가 없으면 건너뜀.
			if (!command.text)
			{
				continue;
			}

			// 세로 기준 화면 벗어났는지 확인.
			if (command.position.y < 0
				|| command.position.y >= screenSize.y)
			{
				continue;
			}

			// 화면에 그릴 문자열 길이.
			const int length = static_cast<int>(strlen(command.text));

			// 안그려도 되면 건너뜀.
			if (length <= 0)
			{
				continue;
			}

			// x좌표 기준으로 화면에서 벗어났는지 확인.
			const int startX = command.position.x;
			const int endX = command.position.x + length - 1;

			if (endX < 0 || startX >= screenSize.x)
			{
				continue;
			}

			// Get Visible Positon.
			const int visibleStart = startX < 0 ? 0 : startX;
			const int visibleEnd = endX >= screenSize.x ? screenSize.x - 1 : endX;

			// Set String.
			for (int x = visibleStart; x <= visibleEnd; ++x)
			{
				// 문자열 안의 문자 index.
				const int sourceIndex = x - startX;

				// Frame (2차원 문자 배열) index.
				const int index
					= (command.position.y * screenSize.x) + x;

				// Sorting order 비교.
				if (frame->sortingOrderArray[index] 
					> command.sortingOrder)
				{
					// 같은 경우에도 덮어씀.
					continue;
				}

				// Data 기록.
				frame->charInfoArray[index].Char.AsciiChar
					= command.text[sourceIndex];

				frame->charInfoArray[index].Attributes
					= (WORD)command.color;

				// Update Sorting order.
				frame->sortingOrderArray[index] = command.sortingOrder;
			}

		}

		// Draw.
		GetCurrentBuffer()->Draw(frame->charInfoArray);

		// Buffer 교환.
		Present();

		// Clear RenderQueue.
		renderQueue.clear();
	}

	void Renderer::Submit(
		const char* text,
		const Vector2& position,
		Color color,
		int sortingOrder
	)
	{
		// Create Render Data => Add to Queue
		RenderCommand command = {};
		command.text = text;
		command.position = position;
		command.color = color;
		command.sortingOrder = sortingOrder;

		renderQueue.emplace_back(command);
	}

	void Renderer::PresentImmediately()
	{
		Draw();
		GetCurrentBuffer()->Draw(frame->charInfoArray);
		Present();
	}

	void Renderer::Present()
	{
		// Buffer 교환.
		SetConsoleActiveScreenBuffer(
			GetCurrentBuffer()->GetBuffer()
		);

		// index 교체.
		currentBufferIndex = 1 - currentBufferIndex;
	}

	ScreenBuffer* Renderer::GetCurrentBuffer()
	{
		return screenBuffers[currentBufferIndex];
	}
}