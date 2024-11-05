#include "pch.h"
#include "Knife.h"

#include <iostream>

#include "Level.h"
#include "Sprite.h"
#include "utils.h"

Knife::Knife(const Point2f& pos, bool isThrown): Weapon{pos, WeaponType::knives, isThrown}, m_Speed{150.f}
{
	m_Velocity.x = m_Speed;
	m_Damage = 20.f;
}

Knife::Knife(const Knife& knife): Weapon{knife}, m_Speed{knife.m_Speed}, m_Velocity{knife.m_Velocity}
{
}

Knife::Knife(Knife&& knife) noexcept : Weapon{std::move(knife)}, m_Speed{knife.m_Speed}, m_Velocity{knife.m_Velocity}
{
	knife.m_Speed = 0.f;
	knife.m_Velocity = Vector2f{};
}

Knife& Knife::operator=(const Knife& rhs)
{
	if (&rhs != this)
	{
		Weapon::operator=(rhs);
		m_Speed = rhs.m_Speed;
		m_Velocity = rhs.m_Velocity;
	}
	return *this;
}

Knife& Knife::operator=(Knife&& rhs) noexcept
{
	if (&rhs != this)
	{
		Weapon::operator=(std::move(rhs));
		m_Speed = rhs.m_Speed;
		m_Velocity = rhs.m_Velocity;

		rhs.m_Speed = 0.f;
		rhs.m_Velocity = Vector2f{};
	}
	return *this;
}

void Knife::Update(float elapsedSec, const Level& level, const Rectf& camera)
{
	if (m_IsThrown && (!m_IsOutOfBounds && !m_HasHit))
	{
		HandlePosition(elapsedSec);
		level.IsOutOfBounds(GetShape(), m_IsOutOfBounds);
		m_IsOutOfBounds = !utils::IsOverlapping(GetShape(), camera);
		level.HasHitGround(GetShape(), m_HasHit);
	}
	else
	{
		Weapon::Update(elapsedSec, level, camera);
	}
}

void Knife::Draw() const
{
	if (!m_IsOutOfBounds && !m_HasHit)
	{
		glPushMatrix();
		glTranslatef(m_Position.x, m_Position.y, 0.f);
		if (m_Direction == utils::Direction::right)
		{
			glScalef(-1.f, 1.f, 1.f);
		}
		m_pSpriteItem->Draw();
		glPopMatrix();
	}
}


void Knife::Reset()
{
	m_Velocity.x = m_Speed;
	m_IsOutOfBounds = false;
	m_HasHit = false;
}

void Knife::HandlePosition(float elapsedSec)
{
	// Movement based on velocity
	if (m_Direction == utils::Direction::right)
	{
		m_Position.x += m_Velocity.x * elapsedSec;
	}
	else
	{
		m_Position.x -= m_Velocity.x * elapsedSec;
	}
}
