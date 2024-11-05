#include "pch.h"
#include "Skeleton.h"

#include <iostream>

#include "Level.h"
#include "Sprite.h"
#include "Texture.h"

unsigned Skeleton::m_CounterSkeleton = 0;
Texture* Skeleton::m_pTextureSkeleton;

Skeleton::Skeleton(const Point2f& pos) : Enemy{pos}, m_Speed{200.f}, m_Velocity{0, -981.f}
{
	m_Health = 100.f;

	const std::string filePath{m_FolderPath + "Skeleton.png"};
	if (!m_pTextureSkeleton)
	{
		m_pTextureSkeleton = new Texture{filePath};
	}
	m_pSpriteEnemy = new Sprite{filePath, m_pTextureSkeleton, 2, 1, 0, 1};
	m_pSpriteEnemy->SetNrFrame(0, 2);
	m_Shape = Rectf{0.f, 0.f, m_pSpriteEnemy->GetShape().width, m_pSpriteEnemy->GetShape().height};
	m_Damage = 2.f;
	++m_CounterSkeleton;
}

Skeleton::Skeleton(const Skeleton& skeleton): Enemy{skeleton}, m_Speed{skeleton.m_Speed},
                                              m_Velocity{skeleton.m_Velocity}
{
	++m_CounterSkeleton;
}

Skeleton::Skeleton(Skeleton&& skeleton) noexcept : Enemy{std::move(skeleton)}, m_Speed{skeleton.m_Speed},
                                                   m_Velocity{skeleton.m_Velocity}
{
}

Skeleton& Skeleton::operator=(const Skeleton& rhs)
{
	if (&rhs != this)
	{
		Enemy::operator=(rhs);
		m_Speed = rhs.m_Speed;
		m_Velocity = rhs.m_Velocity;
	}
	return *this;
}

Skeleton& Skeleton::operator=(Skeleton&& rhs) noexcept
{
	if (&rhs != this)
	{
		Enemy::operator=(std::move(rhs));
		m_Speed = rhs.m_Speed;
		m_Velocity = rhs.m_Velocity;
	}
	return *this;
}

Skeleton::~Skeleton()
{
	if (m_CounterSkeleton <= 1)
	{
		delete m_pTextureSkeleton;
		m_pTextureSkeleton = nullptr;
	}
	--m_CounterSkeleton;
}

void Skeleton::Update(float elapsedSec)
{
	Enemy::Update(elapsedSec);
	if (!m_IsHurt && m_State != EnemyState::dying)
	{
		m_pSpriteEnemy->HandleFrames(elapsedSec);
		HandlePosition(elapsedSec);
	}
}

void Skeleton::Update(const Level& level)
{
	level.HandleCollisionActorsOnGround(m_Position, GetShape(), m_Velocity);
}

void Skeleton::Draw() const
{
	Enemy::Draw();
}

void Skeleton::DrawAlive() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);
	m_pSpriteEnemy->Draw();
	glPopMatrix();
}

void Skeleton::DrawDying() const
{
	Enemy::DrawDying();
}

void Skeleton::HandlePosition(float elapsedSec)
{
	m_Position.y += m_Velocity.y * elapsedSec;
}
