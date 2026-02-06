#include "EnemyDestroyEffect.h"
#include "Engine/Engine.h"

// 효과 재생에 사용할 문자열 Sequence (일종의 Animation Frame).
static const EnemyDestroyEffect::EffectFrame sequence[] =
{
	EnemyDestroyEffect::EffectFrame("  @  ", 0.08f, Color::Red),
	EnemyDestroyEffect::EffectFrame(" @@  ", 0.08f, Color::Blue),
	EnemyDestroyEffect::EffectFrame(" @@@ ", 0.08f, Color::Green),
	EnemyDestroyEffect::EffectFrame("@@@@ ", 0.08f, Color::Red),
	EnemyDestroyEffect::EffectFrame("  +1 ", 0.5f, Color::Green)
};

EnemyDestroyEffect::EnemyDestroyEffect(const Vector2& position)
	:super(sequence[0].frame, position, Color::Red)
{
	int effectFrameImageLength = 6;

	// x 위치 고정: Console 화면 벗어나지 않도록.
	this->position.x = position.x < 0 ? effectFrameImageLength + position.x : position.x;
	this->position.x = position.x + effectFrameImageLength > Engine::Get().GetWidth() ?
		position.x - effectFrameImageLength : position.x;

	// Animation Sequence 개수 구하기.
	effectSequenceCount = sizeof(sequence) / sizeof(sequence[0]);

	// 다음 Animation Frame까지 대기할 시간.
	timer.SetTargetTime(sequence[0].playTime);

	// Set Color.
	color = sequence[0].color;
}

void EnemyDestroyEffect::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// Update for Play Animation.
	timer.Tick(deltaTime);
	if(!timer.IsTimeOut())
	{
		return;
	}

	// Check: Is Animation Finished.
	if (currentSequenceIndex == effectSequenceCount - 1)
	{
		Destroy();
		return;
	}

	// Reset Timer.
	timer.Reset();

	// Update Effect Frame.
	++currentSequenceIndex;

	// Set Timer: Next Sequence Play Time
	timer.SetTargetTime(sequence[currentSequenceIndex].playTime);

	// Copy: AnimationFramString => Actor.
	ChangeImage(sequence[currentSequenceIndex].frame);

	// Set Color.
	color = sequence[currentSequenceIndex].color;
}
