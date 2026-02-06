#include "ShootingPlayer.h"
#include "PlayerBullet.h"
#include "Core/Input.h"
#include "Engine/Engine.h"
#include "Level/Level.h"
#include "Render/Renderer.h"

ShootingPlayer::ShootingPlayer()
	: super("<=A=>", Vector2::Zero, Color::Green),
	fireMode(FireMode::OneShot)
{
	// Set position to create.
	int xPosition = (Engine::Get().GetWidth() / 2) - (width / 2);
	int yPosition = Engine::Get().GetHeight() - 2;
	SetPosition(Vector2(xPosition, yPosition));

	// Timer 목표 시간 설정.
	timer.SetTargetTime(fireInterval);
}

ShootingPlayer::~ShootingPlayer()
{
}

void ShootingPlayer::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 종료 처리.
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// 게임 종료.
		QuitGame();
	}

	// 경과 시간 update.
	timer.Tick(deltaTime);

	// 좌우 방향키 입력처리.
	if (Input::Get().GetKey(VK_LEFT))
	{
		MoveLeft();
	}

	if (Input::Get().GetKey(VK_RIGHT))
	{
		MoveRight();
	}

	// Space->탄약 발사
	if (fireMode == FireMode::OneShot)
	{
		if (Input::Get().GetKeyDown(VK_SPACE))
		{
			Fire();
		}
	}
	else if(fireMode == FireMode::Repeat)
	{
		if(Input::Get().GetKey(VK_SPACE))
		{
			FireInterval();
		}
	}

	// 발사 모드 전환.
	if(Input::Get().GetKeyDown('R'))
	{
		int mode = static_cast<int>(fireMode);
		mode = 1 - mode;
		fireMode = static_cast<FireMode>(mode);
	}
}


void ShootingPlayer::MoveLeft()
{
	position.x -= 1;

	// 좌표 검사.
	if(position.x < 0)
	{
		position.x = 0;
	}
}


void ShootingPlayer::MoveRight()
{
	position.x += 1;

	if (position.x + width > Engine::Get().GetWidth())
	{
		position.x -=1;
	}
}

void ShootingPlayer::Fire()
{
	// 경과 시간 초기화.
	timer.Reset();

	// Set new Actor's position.
	Vector2 bulletPosition(
		position.x + (width / 2),
		position.y
	);

	// Create Bullet.
	GetOwner()->AddNewActor(new PlayerBullet(bulletPosition));
}

void ShootingPlayer::FireInterval()
{
	// 발사 가능 여부 확인.
	if (!CanShoot())
	{
		return;
	}

	// 발사.
	Fire();
}

bool ShootingPlayer::CanShoot() const
{
	// 경과 시간 확인.
	// 발사 간격보다 더 많이 흘렀는지.
	return timer.IsTimeOut();
}
