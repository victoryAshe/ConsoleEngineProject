#pragma once
class Timer
{
public:
	// 생성지ㅏ: Timer 설정 시간 값 전달.
	Timer(float targetTime = 1.0f);
	
	// pointer를 쓰지 않으므로 소멸자는 두지 않음.

	// Update Timer.
	void Tick(float deltaTime);

	// Reset elapsedTime.
	void Reset();
	
	// 설정한 시간만큼 흘렀는지 확인하는 함수.
	bool IsTimeOut() const;

	// 목표 시간 설정 함수.
	void SetTargetTime(float newTargetTime);

private:

	// 경과 시간 확인용 변수.
	float  elapsedTime = 0.0f;

	// Timer 목표 시간.
	float targetTime = 0.0f;
};

