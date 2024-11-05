#include "pch.h"
#include "Turret.h"

#include <iostream>

#include "Medusa.h"
#include "Projectile.h"
#include "Sprite.h"
#include "Texture.h"
#include "utils.h"

unsigned Turret::m_CounterTurret = 0;
Texture* Turret::m_pTextureTurret;

Turret::Turret(const Point2f& pos): Enemy{pos, 1.5f}, m_Projectiles{std::vector<Projectile*>{}},
                                    m_NrProjectilesMax{3}, m_StateTime{}, m_StateTurret{StateTurret::idle},
                                    m_TimeBeforePrepare{4.f}, m_TimeBeforeShoot{2.f}, m_TimeBetweenShot{0.8f},
                                    m_ProjectileShot{}
{
	m_Health = 125.f;

	const std::string filePath{m_FolderPath + "Turret.png"};
	if (!m_pTextureTurret)
	{
		m_pTextureTurret = new Texture{filePath};
	}
	m_pSpriteEnemy = new Sprite{filePath, m_pTextureTurret, 2, 2, 0, 1, 4};
	m_pSpriteEnemy->SetNrFrame(0, 2);
	m_Shape = Rectf{0.f, 0.f, m_pSpriteEnemy->GetShape().width, m_pSpriteEnemy->GetShape().height};
	m_Damage = 3.f;
	++m_CounterTurret;
}

Turret::Turret(const Turret& turret): Turret{turret.GetPosition()}
{
}

Turret::~Turret()
{
	if (m_CounterTurret <= 1)
	{
		delete m_pTextureTurret;
		m_pTextureTurret = nullptr;
	}
	for (const Projectile* pProjectile : m_Projectiles)
	{
		delete pProjectile;
		pProjectile = nullptr;
	}
	--m_CounterTurret;
}

void Turret::Update(float elapsedSec)
{
	Enemy::Update(elapsedSec);
	HandleState(elapsedSec);
}

void Turret::UpdateProjectiles(float elapsedSec, const Level& level, const Rectf& camera)
{
	size_t index{};
	for (Projectile* pProjectile : m_Projectiles)
	{
		pProjectile->Update(elapsedSec, level);
		if (pProjectile->GetHasCollided() || !pProjectile->IsOverlapping(camera))
		{
			m_Projectiles.erase(m_Projectiles.begin() + index);
			delete pProjectile;
			pProjectile = nullptr;
		}
		++index;
	}
}


void Turret::Draw() const
{
	Enemy::Draw();
	DrawProjectiles();
}

bool Turret::ProjectileHitShape(const Rectf& shape)
{
	size_t index{0};
	for (const Projectile* pProjectile : m_Projectiles)
	{
		if (pProjectile->IsOverlapping(shape))
		{
			m_Projectiles.erase(m_Projectiles.begin() + index);
			delete pProjectile;
			pProjectile = nullptr;
			return true;
		}
		++index;
	}
	return false;
}

void Turret::DrawAlive() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);
	m_pSpriteEnemy->Draw();
	glPopMatrix();
}

void Turret::DrawDying() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);
	m_pSpriteEnemy->Draw();
	glPopMatrix();
}

void Turret::DrawProjectiles() const
{
	for (const Projectile* pProjectile : m_Projectiles)
	{
		pProjectile->Draw();
	}
}

void Turret::HandleState(float elapsedSec)
{
	m_StateTime += elapsedSec;
	if (m_State == EnemyState::dying)
	{
		m_pSpriteEnemy->SetRowFrame(2);
		if (m_State == EnemyState::dying)
		{
			m_pSpriteEnemy->HandleFrames(elapsedSec, 1);
		}
		else
		{
			m_pSpriteEnemy->HandleFrames(elapsedSec);
		}
	}
	else
	{
		switch (m_StateTurret)
		{
		case StateTurret::idle:
			m_pSpriteEnemy->SetAnimFrame(0);
			if (m_StateTime > m_TimeBeforePrepare)
			{
				m_StateTurret = StateTurret::prepare;
				m_StateTime -= m_TimeBeforePrepare;
			}
			break;
		case StateTurret::prepare:
			if (m_State == EnemyState::dying)
			{
				m_pSpriteEnemy->HandleFrames(elapsedSec, 1);
			}
			else
			{
				m_pSpriteEnemy->HandleFrames(elapsedSec);
			}
			if (m_StateTime > m_TimeBeforeShoot)
			{
				m_StateTurret = StateTurret::shoot;
				m_StateTime -= m_TimeBeforeShoot;
			}
			break;
		case StateTurret::shoot:
			m_pSpriteEnemy->SetAnimFrame(0);
			if (m_Projectiles.size() < m_NrProjectilesMax)
			{
				if (m_StateTime > m_TimeBetweenShot)
				{
					// if the direction is left
					const Point2f pos{m_Position.x, m_Position.y + GetShape().height / 4 * 3};
					// if the direction is right
					// const Point2f pos{ GetPosition().x, GetPosition().y + (GetShape().height / 4) };
					m_Projectiles.push_back(new Projectile{
						pos, utils::Direction::left, m_FolderPath + "TurretProjectiles.png"
					});
					m_StateTime -= m_TimeBetweenShot;
					++m_ProjectileShot;
				}
			}
			if (m_ProjectileShot == m_NrProjectilesMax)
			{
				m_StateTurret = StateTurret::idle;
				m_StateTime -= m_TimeBeforeShoot;
				m_ProjectileShot = 0;
			}
			break;
		}
	}
}
