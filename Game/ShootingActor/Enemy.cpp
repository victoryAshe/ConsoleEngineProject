#include "Enemy.h"
#include "Util/Util.h"
#include "Engine/Engine.h"
#include "Level/Level.h"
#include "EnemyBullet.h"
#include "EnemyDestroyEffect.h"

Enemy::Enemy(const char* image, int yPosition)
	:super(image)
{
	int random = Util::Random(1, 10);

	if (random % 2 == 0)
	{
		// 화면 오른쪽에서 생성. "(oOo)"
		direction = MoveDirection::Left;
		xPosition = static_cast<float>(
			Engine::Get().GetWidth() - width - 1
			);
	}
	else
	{
		// 화면 왼쪽에서 생성.
		direction = MoveDirection::Right;
		xPosition = 0.0f;
	}

	// 이동 방향에 따른 적 위치 설정.
	SetPosition(
		Vector2(static_cast<int>(xPosition), yPosition)
	);

	// 발사 타이머 목표 시간 설정.
	timer.SetTargetTime(Util::RandomRange(1.0f, 3.0f));
}

Enemy::~Enemy()
{
}

void Enemy::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 이동 처리.
	float dir = direction == MoveDirection::Left ? -1.0f : 1.0f;
	xPosition += moveSpeed * dir * deltaTime;

	// 좌표 검사.
	// 화면 왼쪽을 완전히 벗어났으면.
	if (xPosition + width < 0)
	{
		Destroy();
		return;
	}

	// 화면 오른쪽을 완전히 벗어났으면.
	if(xPosition > Engine::Get().GetWidth()-1)
	{
		Destroy();
		return;
	}

	// 위치 설정.
	SetPosition(Vector2(
		static_cast<int>(xPosition),
		position.y
	));

	// 발사 타이머 update.
	timer.Tick(deltaTime);
	if (!timer.IsTimeOut())
	{
		return;
	}

	// Timer Reset.
	timer.Reset();

	// 탄약 발사.
	GetOwner()->AddNewActor(new EnemyBullet(
		Vector2(position.x + width / 2, position.y),
		Util::RandomRange(10.0f, 20.0f)
	));
}

void Enemy::OnDamaged()
{
	// Actor delete.
	Destroy();

	// Effect 생성(재생을 위해).
	GetOwner()->AddNewActor(new EnemyDestroyEffect(position));
}
