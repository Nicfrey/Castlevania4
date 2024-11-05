#include "pch.h"
#include "EnemyManager.h"

#include "Bat.h"
#include "Enemy.h"
#include "Level.h"
#include "Medusa.h"
#include "Player.h"
#include "Skeleton.h"
#include "SoundManagerEnemy.h"
#include "Sparkle.h"
#include "Spirit.h"
#include "Turret.h"
#include "Unicorn.h"
#include "Worm.h"

EnemyManager::EnemyManager(): m_pSoundManager{new SoundManagerEnemy{}}, m_pEnemies{std::vector<Enemy*>{}}
{
}

EnemyManager::~EnemyManager()
{
	delete m_pSoundManager;
	m_pSoundManager = nullptr;
	Clear();
}

void EnemyManager::Update(float elapsedSec, const Level& level, const Rectf& camera, Player& player)
{
	UpdateFront(elapsedSec, level, camera, player);
	UpdateSparkles(elapsedSec, level, camera);
	if (level.GetCurrentBlock() == 2)
	{
		UpdateBack(elapsedSec, level, camera, player);
		UpdateSparklesBack(elapsedSec, level, camera);
	}
}

void EnemyManager::UpdateBack(float elapsedSec, const Level& level, const Rectf& camera, Player& player)
{
	size_t index{0};
	for (Enemy* pEnemy : m_pEnemiesBack)
	{
		if (pEnemy->isOverlapping(camera))
		{
			pEnemy->Update(elapsedSec);
			Skeleton* pSkeleton{dynamic_cast<Skeleton*>(pEnemy)};
			if (pSkeleton)
			{
				pSkeleton->Update(level);
			}
			RemoveDeadEnemyBack(index, pEnemy, player);
		}
		++index;
	}
}

void EnemyManager::UpdateFront(float elapsedSec, const Level& level, const Rectf& camera, Player& player)
{
	size_t index{0};
	for (Enemy* pEnemy : m_pEnemies)
	{
		Turret* pTurret{dynamic_cast<Turret*>(pEnemy)};
		if (pTurret)
		{
			pTurret->UpdateProjectiles(elapsedSec, level, camera);
		}

		if (pEnemy->isOverlapping(camera))
		{
			pEnemy->Update(elapsedSec);

			Skeleton* pSkeleton{dynamic_cast<Skeleton*>(pEnemy)};
			if (pSkeleton)
			{
				pSkeleton->Update(level);
			}
			Worm* pWorm{dynamic_cast<Worm*>(pEnemy)};
			if (pWorm)
			{
				pWorm->Update(player.GetHitBoxAvatar(), level);
			}
			Unicorn* pUnicorn{dynamic_cast<Unicorn*>(pEnemy)};
			if (pUnicorn)
			{
				pUnicorn->Update(elapsedSec, player.GetHitBoxAvatar());
			}
			Spirit* pSpirit{dynamic_cast<Spirit*>(pEnemy)};
			if (pSpirit)
			{
				pSpirit->Update(elapsedSec, player.GetHitBoxAvatar());
			}
			RemoveDeadEnemy(index, pEnemy, player);
		}
		++index;
	}
}

void EnemyManager::DrawFront() const
{
	for (const Enemy* pEnemy : m_pEnemies)
	{
		pEnemy->Draw();
	}
	for (const Sparkle* pSparkle : m_pSparkles)
	{
		pSparkle->Draw();
	}
}

void EnemyManager::DrawBack() const
{
	for (const Enemy* pEnemy : m_pEnemiesBack)
	{
		pEnemy->Draw();
	}
	for (const Sparkle* pSparkle : m_pSparklesBack)
	{
		pSparkle->Draw();
	}
}

bool EnemyManager::HitEnemy(const Player& player)
{
	for (Enemy* pEnemy : m_pEnemies)
	{
		const Point2f intersectionPoint{GetIntersectionPointHit(player, pEnemy)};
		if (intersectionPoint.x > 0)
		{
			PlaySounds(pEnemy);
			CreateSparkle(intersectionPoint);
			return true;
		}
	}
	return false;
}

bool EnemyManager::HitEnemyBack(const Player& player)
{
	for (Enemy* pEnemy : m_pEnemiesBack)
	{
		const Point2f intersectionPoint = GetIntersectionPointHit(player, pEnemy);
		if (intersectionPoint.x > 0)
		{
			PlaySounds(pEnemy);
			CreateSparkleBack(intersectionPoint);
			return true;
		}
	}
	return false;
}

bool EnemyManager::IsOverlappingEnemy(Player& player) const
{
	if (player.IsInvincible())
	{
		return false;
	}
	bool isOverlapping{false};
	size_t index{0};
	while (!isOverlapping && index < m_pEnemies.size())
	{
		Enemy* pEnemy{m_pEnemies[index]};
		isOverlapping = HandleOverlapping(player, pEnemy);
		++index;
	}
	return isOverlapping;
}

bool EnemyManager::IsOverlappingEnemyBack(Player& player) const
{
	if (player.IsInvincible())
	{
		return false;
	}
	bool isOverlapping{false};
	size_t index{0};
	while (!isOverlapping && index < m_pEnemiesBack.size())
	{
		Enemy* pEnemy{m_pEnemiesBack[index]};
		isOverlapping = HandleOverlapping(player, pEnemy);
		++index;
	}
	return isOverlapping;
}

void EnemyManager::Clear()
{
	for (const Enemy* pEnemy : m_pEnemies)
	{
		delete pEnemy;
		pEnemy = nullptr;
	}
	for (const Enemy* pEnemy : m_pEnemiesBack)
	{
		delete pEnemy;
		pEnemy = nullptr;
	}
	for (const Sparkle* pSparkle : m_pSparkles)
	{
		delete pSparkle;
		pSparkle = nullptr;
	}
	for (const Sparkle* pSparkle : m_pSparklesBack)
	{
		delete pSparkle;
		pSparkle = nullptr;
	}
	m_pEnemiesBack.clear();
	m_pEnemies.clear();
	m_pSparkles.clear();
	m_pSparkles.clear();
}

void EnemyManager::PlaySounds(const Enemy* pEnemy) const
{
	PlaySoundHit();
	if (pEnemy->IsDying())
	{
		PlaySoundDeath(pEnemy);
	}
}

void EnemyManager::PlaySoundHit() const
{
	m_pSoundManager->PlayHit();
}

bool EnemyManager::HandleOverlapping(Player& player, Enemy* pEnemy) const
{
	if (pEnemy && pEnemy->hasCollided(player.GetHitBoxAvatar()))
	{
		if (pEnemy->GetState() != Enemy::EnemyState::dying)
		{
			player.RemoveHealth(static_cast<unsigned>(pEnemy->GetDamage()));
			return true;
		}
	}
	Turret* pTurret{dynamic_cast<Turret*>(pEnemy)};
	if (pTurret)
	{
		if (pTurret->ProjectileHitShape(player.GetHitBoxAvatar()))
		{
			if (pTurret->GetState() != Enemy::EnemyState::dying)
			{
				player.RemoveHealth(static_cast<unsigned>(pTurret->GetDamage()));
				return true;
			}
		}
	}
	return false;
}

void EnemyManager::RemoveDeadEnemy(const size_t index, const Enemy* pEnemy, Player& player)
{
	if (pEnemy->IsDead())
	{
		player.AddScore(100);
		m_pEnemies.erase(m_pEnemies.begin() + index);
		delete pEnemy;
	}
}

void EnemyManager::RemoveDeadEnemyBack(const size_t index, const Enemy* pEnemy, Player& player)
{
	if (pEnemy->IsDead())
	{
		player.AddScore(100);
		m_pEnemiesBack.erase(m_pEnemiesBack.begin() + index);
		delete pEnemy;
	}
}

void EnemyManager::CreateSparkle(const Point2f& intersectionPoint)
{
	m_pSparkles.push_back(new Sparkle{intersectionPoint});
}

void EnemyManager::CreateSparkleBack(const Point2f& intersectionPoint)
{
	m_pSparklesBack.push_back(new Sparkle{intersectionPoint});
}

void EnemyManager::UpdateSparkles(float elapsedSec, const Level& level, const Rectf& camera)
{
	size_t index{};
	for (Sparkle* pSparkle : m_pSparkles)
	{
		pSparkle->Update(elapsedSec, level, camera);
		if (!pSparkle->CanBeDrawn())
		{
			delete pSparkle;
			pSparkle = nullptr;
			m_pSparkles.erase(m_pSparkles.begin() + index);
		}
		++index;
	}
}

void EnemyManager::UpdateSparklesBack(float elapsedSec, const Level& level, const Rectf& camera)
{
	size_t index{};
	for (Sparkle* pSparkle : m_pSparklesBack)
	{
		pSparkle->Update(elapsedSec, level, camera);
		if (!pSparkle->CanBeDrawn())
		{
			delete pSparkle;
			pSparkle = nullptr;
			m_pSparklesBack.erase(m_pSparklesBack.begin() + index);
		}
		++index;
	}
}

Point2f EnemyManager::GetIntersectionPointHit(const Player& player, Enemy* pEnemy)
{
	Point2f intersectionPoint;
	Spirit* pSpirit{dynamic_cast<Spirit*>(pEnemy)};
	if (pSpirit)
	{
		if (pSpirit->CanBeHurt())
		{
			intersectionPoint = pSpirit->Hit(player.GetShapeMainWeapon(), player.GetDamageMainWeapon());
			if (player.IsThrownSubWeapon())
			{
				intersectionPoint = pSpirit->Hit(player.GetShapeSubWeapon(), player.GetDamageSubWeapon());
				if (intersectionPoint.x > -1.f)
				{
					player.SubWeaponHit();
				}
			}
		}
	}
	else
	{
		intersectionPoint = pEnemy->Hit(player.GetShapeMainWeapon(), player.GetDamageMainWeapon());
		if (player.IsThrownSubWeapon())
		{
			intersectionPoint = pEnemy->Hit(player.GetShapeSubWeapon(), player.GetDamageSubWeapon());
			if (intersectionPoint.x > -1.f)
			{
				player.SubWeaponHit();
			}
		}
	}
	return intersectionPoint;
}
