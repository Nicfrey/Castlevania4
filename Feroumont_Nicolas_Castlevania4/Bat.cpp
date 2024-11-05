#include "pch.h"
#include "Bat.h"

#include <iostream>

#include "Sprite.h"
#include "Texture.h"


unsigned Bat::m_CounterBat = 0;
Texture* Bat::m_pTextureBat;

Bat::Bat(const Point2f& pos) : Enemy{pos, 1.f}, m_Velocity{0.f, 0.f}, m_RotationTime{0.f}, m_Speed{-80.f}
{
	m_Health = 25.f;
	const std::string filePathBat{m_FolderPath + "Bat.png"};
	if (!m_pTextureBat)
	{
		m_pTextureBat = new Texture{filePathBat};
	}
	m_pSpriteEnemy = new Sprite{filePathBat, m_pTextureBat, 2, 1, 0, 1, 6};
	m_pSpriteEnemy->SetNrFrame(0, 2);
	m_Shape = Rectf{0.f, 0.f, m_pSpriteEnemy->GetShape().width, m_pSpriteEnemy->GetShape().width};
	m_Damage = 2.f;
	++m_CounterBat;
}

Bat::Bat(const Bat& bat) : Enemy{bat}, m_Velocity{bat.m_Velocity}, m_RotationTime{bat.m_RotationTime},
                           m_Speed{bat.m_Speed}
{
	const std::string filePathBat{m_FolderPath + "Bat.png"};
	++m_CounterBat;
}

Bat::Bat(Bat&& bat) noexcept : Enemy{std::move(bat)},
                               m_Velocity{bat.m_Velocity}, m_RotationTime{bat.m_RotationTime}, m_Speed{bat.m_Speed}
{
}

Bat& Bat::operator=(const Bat& rhs)
{
	if (&rhs != this)
	{
		Enemy::operator=(rhs);
		const std::string filePathBat{m_FolderPath + "Bat.png"};
		m_Velocity = rhs.m_Velocity;
		m_RotationTime = rhs.m_RotationTime;
		m_Speed = rhs.m_Speed;
	}
	return *this;
}

Bat& Bat::operator=(Bat&& rhs) noexcept
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

Bat::~Bat()
{
	if (m_CounterBat == 1)
	{
		delete m_pTextureBat;
		m_pTextureBat = nullptr;
	}
	--m_CounterBat;
}

void Bat::Update(float elapsedSec)
{
	Enemy::Update(elapsedSec);
	if (!m_IsHurt && (m_State != EnemyState::dead && m_State != EnemyState::dying))
	{
		m_pSpriteEnemy->HandleFrames(elapsedSec);
		HandlePosition(elapsedSec);
	}
}

void Bat::Draw() const
{
	Enemy::Draw();
}

void Bat::DrawAlive() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);
	m_pSpriteEnemy->Draw();
	glPopMatrix();
}

void Bat::DrawDying() const
{
	Enemy::DrawDying();
}


void Bat::HandlePosition(float elapsedSec)
{
	m_RotationTime += elapsedSec;
	m_Velocity.x = m_Speed * elapsedSec;
	m_Velocity.y = sinf(m_RotationTime * 4.f) * 0.5f;

	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
}
