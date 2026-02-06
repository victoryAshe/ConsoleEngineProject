#pragma once

#include "Actor/Actor.h"
#include "Util/Util.h"
#include "Util/Timer.h"

// Enemy가 죽었을 때 죽음 효과를 재생하기 위해 생성하는 Actor.

using namespace Wanted;

class EnemyDestroyEffect: public Actor
{
	RTTI_DECLARATIONS(EnemyDestroyEffect, Actor)

	// Animation Effect Frame struct.
	struct EffectFrame
	{
		EffectFrame(
			const char* frame,
			float playTime = 0.05f,
			Color color = Color::Red)
			: playTime(playTime), color(color)
		{
			// Copy charString.
			size_t length = strlen(frame) + 1;
			this->frame = new char[length];
			strcpy_s(this->frame, length, frame);
		}

		~EffectFrame()
		{
			SafeDeleteArray(frame);
		}

		// 화면에 보여줄 문자열 변수.
		char* frame = nullptr;

		// 재생 시간.
		float playTime = 0.0f;

		// 색상.
		Color color = Color::White;

	};

public:
	EnemyDestroyEffect(const Vector2& position);

	virtual void Tick(float deltaTime) override;

private:
	// Sequence string count: 배열의 원소 수. 
	// => Animation Frame count.
	int effectSequenceCount = 0;

	// 현재 보여지는 sequence의 index.
	int currentSequenceIndex = 0;

	// Timer to play Animation.
	// To Calculate time between sequence.
	Timer timer;

};

