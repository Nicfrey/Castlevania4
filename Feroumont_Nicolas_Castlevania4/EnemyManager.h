#pragma once
#include <vector>
#include <type_traits>

#include "SoundManagerEnemy.h"

class SoundManagerEnemy;
class Spirit;
class Sparkle;
class Level;
class Medusa;
class Turret;
class Worm;
class Unicorn;
class Bat;
class Skeleton;
class Player;
class Enemy;

class EnemyManager final
{
public:
	explicit EnemyManager();
	~EnemyManager();
	EnemyManager(const EnemyManager& enemyManager) = delete;
	EnemyManager(EnemyManager&& enemyManager) noexcept = delete;
	EnemyManager& operator=(const EnemyManager& rhs) = delete;
	EnemyManager& operator=(EnemyManager&& rhs) noexcept = delete;
	template <class T>
	void AddEnemy(const T& enemy);
	template <class T>
	void AddEnemyBack(const T& enemy);
	void Update(float elapsedSec, const Level& level, const Rectf& camera, Player& player);
	void DrawFront() const;
	void DrawBack() const;
	bool HitEnemy(const Player& player);
	bool HitEnemyBack(const Player& player);
	bool IsOverlappingEnemy(Player& player) const;
	bool IsOverlappingEnemyBack(Player& player) const;
	void Clear();

private:
	void PlaySounds(const Enemy* pEnemy) const;
	void PlaySoundHit() const;
	bool HandleOverlapping(Player& player, Enemy* pEnemy) const;
	void RemoveDeadEnemy(const size_t index, const Enemy* pEnemy, Player& player);
	void RemoveDeadEnemyBack(const size_t index, const Enemy* pEnemy, Player& player);
	void UpdateBack(float elapsedSec, const Level& level, const Rectf& camera, Player& player);
	void UpdateFront(float elapsedSec, const Level& level, const Rectf& camera, Player& player);
	void CreateSparkle(const Point2f& intersectionPoint);
	void CreateSparkleBack(const Point2f& intersectionPoint);
	void UpdateSparkles(float elapsedSec, const Level& level, const Rectf& camera);
	void UpdateSparklesBack(float elapsedSec, const Level& level, const Rectf& camera);
	Point2f GetIntersectionPointHit(const Player& player, Enemy* pEnemy);
	template <class T>
	void PlaySoundDeath(const T* enemy) const;
	SoundManagerEnemy* m_pSoundManager;
	std::vector<Enemy*> m_pEnemies;
	std::vector<Enemy*> m_pEnemiesBack;
	std::vector<Sparkle*> m_pSparkles;
	std::vector<Sparkle*> m_pSparklesBack;
};

template <class T>
void EnemyManager::AddEnemy(const T& enemy)
{
	if (std::is_base_of_v<Enemy, T>)
	{
		m_pEnemies.push_back(new T{enemy});
	}
}

template <class T>
void EnemyManager::AddEnemyBack(const T& enemy)
{
	if (std::is_base_of_v<Enemy, T>)
	{
		m_pEnemiesBack.push_back(new T{enemy});
	}
}

template <class T>
void EnemyManager::PlaySoundDeath(const T* enemy) const
{
	if (const Bat* pBat{dynamic_cast<const Bat*>(enemy)})
	{
		m_pSoundManager->PlayBat();
	}
	if (const Medusa* pMedusa{dynamic_cast<const Medusa*>(enemy)})
	{
		m_pSoundManager->PlayMedusa();
	}
	if (const Skeleton* pSkeleton{dynamic_cast<const Skeleton*>(enemy)})
	{
		m_pSoundManager->PlaySkeleton();
	}
	if (const Spirit* pSpirit{dynamic_cast<const Spirit*>(enemy)})
	{
		m_pSoundManager->PlaySpirit();
	}
	if (const Turret* pTurret{dynamic_cast<const Turret*>(enemy)})
	{
		m_pSoundManager->PlayTurret();
	}
	if (const Unicorn* pUnicorn{dynamic_cast<const Unicorn*>(enemy)})
	{
		m_pSoundManager->PlayUnicorn();
	}
	if (const Worm* pWorm{dynamic_cast<const Worm*>(enemy)})
	{
		m_pSoundManager->PlayWorm();
	}
}
