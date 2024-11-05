#include "pch.h"
#include "Enemy.h"

#include <iostream>

#include "Sprite.h"
#include "Texture.h"
#include "utils.h"

unsigned Enemy::m_InstanceCounter = 0;
const std::string Enemy::m_FolderPath = "Sprites/Enemy/";
Texture* Enemy::m_pTextureDeath = nullptr;

Enemy::Enemy(const Point2f& pos) : GameObject{++m_InstanceCounter}, m_pSpriteEnemy{nullptr},
                                   m_State{EnemyState::alive}, m_IsHurt{false}, m_Health{100.f},
                                   m_Damage{}, m_TimeDying{2.f}, m_TimeFreeze{0.5f}, m_CurrentTime{0.f}
{
	m_Position = pos;
	const std::string filePath{"Sprites/Items.png"};
	if (!m_pTextureDeath)
	{
		m_pTextureDeath = new Texture{filePath};
	}
	m_pSpriteDeath = new Sprite{filePath, 10, 4, 0, 3, 6, 3};
}

Enemy::~Enemy()
{
	delete m_pSpriteEnemy;
	m_pSpriteEnemy = nullptr;
	delete m_pSpriteDeath;
	m_pSpriteDeath = nullptr;
	if (m_InstanceCounter == 1)
	{
		delete m_pTextureDeath;
		m_pTextureDeath = nullptr;
	}
	--m_InstanceCounter;
};

Enemy::Enemy(const Point2f& pos, float timeDying): Enemy{pos}
{
	m_TimeDying = timeDying;
}

Enemy::Enemy(const Enemy& enemy) : GameObject{enemy}, m_State{enemy.m_State}, m_Shape{enemy.m_Shape},
                                   m_IsHurt{enemy.m_IsHurt}, m_Health{enemy.m_Health}, m_Damage{enemy.m_Damage},
                                   m_TimeDying{enemy.m_TimeDying}, m_TimeFreeze{enemy.m_TimeFreeze},
                                   m_CurrentTime{enemy.m_CurrentTime}
{
	m_pSpriteEnemy = new Sprite{*enemy.m_pSpriteEnemy};
	m_pSpriteDeath = new Sprite{*enemy.m_pSpriteDeath};
	++m_InstanceCounter;
}

Enemy::Enemy(Enemy&& enemy) noexcept : GameObject{std::move(enemy)}, m_pSpriteEnemy{enemy.m_pSpriteEnemy},
                                       m_pSpriteDeath{enemy.m_pSpriteDeath}, m_State{enemy.m_State},
                                       m_Shape{enemy.m_Shape}, m_IsHurt{enemy.m_IsHurt}, m_Health{enemy.m_Health},
                                       m_Damage{enemy.m_Damage}, m_TimeDying{enemy.m_TimeDying},
                                       m_TimeFreeze{enemy.m_TimeFreeze}, m_CurrentTime{enemy.m_CurrentTime}
{
	enemy.m_pSpriteEnemy = nullptr;
	enemy.m_pSpriteDeath = nullptr;
}

Enemy& Enemy::operator=(const Enemy& rhs)
{
	if (&rhs != this)
	{
		GameObject::operator=(rhs);
		delete m_pSpriteEnemy;
		delete m_pSpriteDeath;
		m_pSpriteEnemy = new Sprite{*rhs.m_pSpriteEnemy};
		m_pSpriteDeath = new Sprite{*rhs.m_pSpriteDeath};
		m_Shape = rhs.m_Shape;
		m_State = rhs.m_State;
		m_IsHurt = rhs.m_IsHurt;
		m_Health = rhs.m_Health;
		m_Damage = rhs.m_Damage;
		m_TimeDying = rhs.m_TimeDying;
		m_TimeFreeze = rhs.m_TimeFreeze;
		m_CurrentTime = rhs.m_CurrentTime;
	}
	return *this;
}

Enemy& Enemy::operator=(Enemy&& rhs) noexcept
{
	if (&rhs != this)
	{
		GameObject::operator=(std::move(rhs));
		m_pSpriteEnemy = rhs.m_pSpriteEnemy;
		m_pSpriteDeath = rhs.m_pSpriteDeath;
		m_Shape = rhs.m_Shape;
		m_State = rhs.m_State;
		m_IsHurt = rhs.m_IsHurt;
		m_Health = rhs.m_Health;
		m_Damage = rhs.m_Damage;
		m_TimeDying = rhs.m_TimeDying;
		m_TimeFreeze = rhs.m_TimeFreeze;
		m_CurrentTime = rhs.m_CurrentTime;
		rhs.m_pSpriteEnemy = nullptr;
	}
	return *this;
}

void Enemy::Update(float elapsedSec)
{
	if (m_IsHurt)
	{
		HandleHurt(elapsedSec);
	}
	if (m_State == EnemyState::dying)
	{
		HandleDying(elapsedSec);
	}
}

void Enemy::Draw() const
{
	switch (m_State)
	{
	case EnemyState::alive:
		DrawAlive();
		break;
	case EnemyState::dying:
		DrawDying();
		break;
	case EnemyState::dead:
		// Draw Nothing
		break;
	}
}

Rectf Enemy::GetShape() const
{
	const Rectf shape{m_Position.x, m_Position.y, m_Shape.width, m_Shape.height};
	return shape;
}

bool Enemy::IsDead() const
{
	return m_State == EnemyState::dead;
}

bool Enemy::IsDying() const
{
	return m_State == EnemyState::dying;
}

void Enemy::SetHealth(float damage)
{
	m_Health -= damage;
	if (m_Health < 0.f)
	{
		m_State = EnemyState::dying;
	}
}

bool Enemy::hasCollided(const Rectf& shape) const
{
	if (utils::IsOverlapping(GetShape(), shape))
	{
		return true;
	}
	return false;
}

float Enemy::GetDamage() const
{
	return m_Damage;
}

Point2f Enemy::Hit(const Rectf& shapeWeapon, const float damage)
{
	if (m_IsHurt || m_State == EnemyState::dying)
	{
		return Point2f{-300.f, -300.f};
	}
	const Point2f rayP1Horizontal{shapeWeapon.left, shapeWeapon.bottom + shapeWeapon.height / 2};
	const Point2f rayP2Horizontal{shapeWeapon.left + shapeWeapon.width, shapeWeapon.bottom + shapeWeapon.height / 2};

	const Point2f rayP1Vertical{shapeWeapon.left + shapeWeapon.width / 2, shapeWeapon.bottom};
	const Point2f rayP2Vertical{shapeWeapon.left + shapeWeapon.width / 2, shapeWeapon.bottom + shapeWeapon.height};

	// Translate Rect to polygon
	std::vector<Point2f> pointsEnemy;
	pointsEnemy.push_back(m_Position);
	pointsEnemy.push_back(Point2f{m_Position.x, m_Position.y + GetShape().height});
	pointsEnemy.push_back(Point2f{m_Position.x + GetShape().width, m_Position.y});
	pointsEnemy.push_back(Point2f{m_Position.x + GetShape().width, m_Position.y + GetShape().height});

	utils::HitInfo hitInfo;
	if (utils::Raycast(pointsEnemy, rayP1Horizontal, rayP2Horizontal, hitInfo))
	{
		SetHealth(damage);
		m_IsHurt = true;
		return hitInfo.intersectPoint;
	}
	if (utils::Raycast(pointsEnemy, rayP1Vertical, rayP2Vertical, hitInfo))
	{
		SetHealth(damage);
		m_IsHurt = true;
		return hitInfo.intersectPoint;
	}
	return Point2f{-300.f, -300.f};
}

bool Enemy::isOverlapping(const Rectf& shape) const
{
	const Rectf shapeEnemy{m_Position.x, m_Position.y, m_Shape.width, m_Shape.height};
	return utils::IsOverlapping(shapeEnemy, shape);
}

Enemy::EnemyState Enemy::GetState() const
{
	return m_State;
}


void Enemy::DrawDying() const
{
	const Point2f centerEnemy{utils::GetElementCenterOfRectangle(GetShape(), m_pSpriteDeath->GetShape())};
	glPushMatrix();
	glTranslatef(centerEnemy.x, centerEnemy.y, 0.f);
	m_pSpriteDeath->Draw();
	glPopMatrix();
}

void Enemy::HandlePosition(float elapsedSec)
{
}

void Enemy::HandleDying(float elapsedSec)
{
	if (m_CurrentTime < m_TimeDying)
	{
		m_CurrentTime += elapsedSec;
		m_pSpriteDeath->HandleFrames(elapsedSec);
	}
	if (m_CurrentTime > m_TimeDying)
	{
		m_State = EnemyState::dead;
	}
}

void Enemy::HandleHurt(float elapsedSec)
{
	if (m_CurrentTime < m_TimeFreeze)
	{
		m_CurrentTime += elapsedSec;
	}
	if (m_CurrentTime > m_TimeFreeze)
	{
		m_IsHurt = false;
		m_CurrentTime = 0.f;
	}
}
