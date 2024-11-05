#include "pch.h"
#include "Medusa.h"

#include <iostream>

#include "Sprite.h"
#include "Texture.h"

unsigned Medusa::m_CounterMedusa = 0;
Texture* Medusa::m_pTextureMedusa;

Medusa::Medusa(const Point2f& pos): Enemy{pos, 1.f}, m_Velocity{0.f, 0.f}, m_RotationTime{0.f},
                                    m_Speed{-75.f}
{
	const std::string filePathMedusa{m_FolderPath + "Medusa.png"};
	if (!m_pTextureMedusa)
	{
		m_pTextureMedusa = new Texture{filePathMedusa};
	}
	m_pSpriteEnemy = new Sprite{filePathMedusa, m_pTextureMedusa, 2, 2, 0, 1};
	m_pSpriteEnemy->SetNrFrame(0, 2);
	m_Health = 25.f;

	m_Shape = Rectf{0.f, 0.f, m_pSpriteEnemy->GetShape().width, m_pSpriteEnemy->GetShape().height};
	const float damageMedusa{2.f};
	m_Damage = damageMedusa;
	++m_CounterMedusa;
}

Medusa::Medusa(const Medusa& medusa): Enemy{medusa}, m_Velocity{medusa.m_Velocity},
                                      m_RotationTime{medusa.m_RotationTime},
                                      m_Speed{medusa.m_Speed}
{
	++m_CounterMedusa;
}

Medusa::Medusa(Medusa&& medusa) noexcept : Enemy{std::move(medusa)},
                                           m_Velocity{medusa.m_Velocity}, m_RotationTime{medusa.m_RotationTime},
                                           m_Speed{medusa.m_Speed}
{
}

Medusa& Medusa::operator=(const Medusa& rhs)
{
	if (&rhs != this)
	{
		Enemy::operator=(rhs);
		m_Velocity = rhs.m_Velocity;
		m_RotationTime = rhs.m_RotationTime;
		m_Speed = rhs.m_Speed;
	}
	return *this;
}

Medusa& Medusa::operator=(Medusa&& rhs) noexcept
{
	if (&rhs != this)
	{
		Enemy::operator=(std::move(rhs));
		m_Velocity = rhs.m_Velocity;
		m_RotationTime = rhs.m_RotationTime;
		m_Speed = rhs.m_Speed;
	}
	return *this;
}

Medusa::~Medusa()
{
	if (m_CounterMedusa <= 1)
	{
		delete m_pTextureMedusa;
		m_pTextureMedusa = nullptr;
	}
	--m_CounterMedusa;
}

void Medusa::Update(float elapsedSec)
{
	Enemy::Update(elapsedSec);
	if (m_IsHurt || m_State == EnemyState::dead)
	{
		m_pSpriteEnemy->SetAnimFrame(0);
		m_pSpriteEnemy->SetRowFrame(1);
	}
	else if (!m_IsHurt && (m_State != EnemyState::dead && m_State != EnemyState::dying))
	{
		HandlePosition(elapsedSec);
		m_pSpriteEnemy->HandleFrames(elapsedSec);
	}
}

void Medusa::Draw() const
{
	Enemy::Draw();
}

void Medusa::DrawAlive() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);
	m_pSpriteEnemy->Draw();
	glPopMatrix();
}

void Medusa::DrawDying() const
{
	Enemy::DrawDying();
}


void Medusa::HandlePosition(float elapsedSec)
{
	m_RotationTime += elapsedSec;
	m_Velocity.x = m_Speed * elapsedSec;
	m_Velocity.y = sinf(m_RotationTime * 4.f) * 0.75f;

	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
}
