#include "ShootingGameLevel.h"
#include "ShootingActor/Player.h"
#include "ShootingActor/Enemy.h"
#include "ShootingActor/PlayerBullet.h"
#include "ShootingActor/EnemyBullet.h"
#include "ShootingActor/EnemySpawner.h"
#include "ShootingActor/MouseTester.h"
#include "Render/Renderer.h"
#include "Engine/Engine.h"

ShootingGameLevel::ShootingGameLevel()
{
	// Add Player Actor.
	AddNewActor(new Player());

	// Add Enemy Spawner.
	AddNewActor(new EnemySpawner());
	
	// Test: Add Mouse Tester.
	AddNewActor(new MouseTester());
}

ShootingGameLevel::~ShootingGameLevel()
{
}

void ShootingGameLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	ProcessCollisionPlayerAndEnemyBullet();
	ProcessCollisionPlayerBulletAndEnemy();
}

void ShootingGameLevel::Draw()
{
	super::Draw();

	if (isPlayerDead)
	{
		// Submit to Renderer: Player Dead Message.
		Renderer::Get().Submit("!Dead!", playerDeadPosition);

		// Show score.
		ShowScore();

		// Present Result Immediately.
		Renderer::Get().PresentImmediately();

		// Sleep: Pause Program
		Sleep(2000);

		// Quit Game.
		Engine::Get().QuitEngine();
	}

	// Show Score.
	ShowScore();
}

void ShootingGameLevel::ProcessCollisionPlayerBulletAndEnemy()
{
	// Filtering: Player Bullet과 Enemy Actor
	std::vector<Actor*> bullets;
	std::vector<Enemy*> enemies;

	// Filtering Actor.
	for (Actor* const actor : actors)
	{
		if (actor->IsTypeOf<PlayerBullet>())
		{
			bullets.emplace_back(actor);
			continue;
		}

		if(actor->IsTypeOf<Enemy>())	
		{
			enemies.emplace_back(actor->As<Enemy>());
		}
	}

	// 판정 안해도 되는지 확인.
	if (bullets.size() == 0 || enemies.size() == 0)
	{
		return;
	}

	// 충돌 판정.
	for (Actor* const bullet : bullets)
	{
		for (Enemy* const enemy : enemies)
		{
			// AABB 겹침 판정.
			if (bullet->TestIntersect(enemy))
			{
				enemy->OnDamaged();
				bullet->Destroy();

				// 점수 추가.
				score += 1;
				continue;
			}
		}
	}
}

void ShootingGameLevel::ProcessCollisionPlayerAndEnemyBullet()
{
	// Filtering Actor.
	Player* player = nullptr;
	std::vector<Actor*> bullets;
	for (Actor* const actor : actors)
	{
		if (!player && actor->IsTypeOf<Player>())
		{
			player = actor->As<Player>();
			continue;
		}

		if (actor->IsTypeOf<EnemyBullet>())
		{
			bullets.emplace_back(actor);
		}
	}

	// 판정 처리 안해도 되는지 확인.
	if (bullets.size() == 0 || !player)
	{
		return;
	}

	// 충돌 판정.
	for (Actor* const bullet : bullets)
	{
		if (bullet->TestIntersect(player))
		{
			// Set Player Dead Flag.
			isPlayerDead = true;

			// Save Player Dead Position.
			playerDeadPosition = player->GetPosition();

			player->Destroy();
			bullet->Destroy();
			break;
		}
	}
}

void ShootingGameLevel::ShowScore()
{
	sprintf_s(scoreString, 128, "Score: %d", score);
	Renderer::Get().Submit(
		scoreString,
		Vector2(0, Engine::Get().GetHeight() - 1)
	);
}
