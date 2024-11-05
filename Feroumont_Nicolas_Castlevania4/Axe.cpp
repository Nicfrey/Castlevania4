#include "pch.h"
#include "Axe.h"

#include <iostream>

#include "Level.h"
#include "Sprite.h"
#include "utils.h"

Axe::Axe(const Point2f& pos, bool isThrown): Weapon{pos, WeaponType::axe, isThrown}, m_Velocity{0.f, 0.f},
                                             m_Speed{100.f}, m_MaxHeight{400.f}, m_RotationTime{0.f}
{
	m_Velocity.x = m_Speed;
	m_Velocity.y = m_MaxHeight;
	m_Damage = 30.f;
}

Axe::Axe(const Axe& axe) = default;

Axe::Axe(Axe&& axe) noexcept: Weapon{std::move(axe)}, m_Velocity{axe.m_Velocity},
                              m_Speed{axe.m_Speed}, m_MaxHeight{axe.m_MaxHeight}, m_RotationTime{axe.m_RotationTime}
{
	axe.m_Velocity = Vector2f{};
	axe.m_Speed = 0.f;
	axe.m_RotationTime = 0.f;
	axe.m_MaxHeight = 0.f;
}

Axe& Axe::operator=(const Axe& rhs)
{
	if (&rhs != this)
	{
		Weapon::operator=(rhs);
		m_Velocity = rhs.m_Velocity;
		m_Speed = rhs.m_Speed;
		m_RotationTime = rhs.m_RotationTime;
		m_MaxHeight = rhs.m_MaxHeight;
	}
	return *this;
}

Axe& Axe::operator=(Axe&& rhs) noexcept
{
	if (&rhs != this)
	{
		Weapon::operator=(std::move(rhs));
		m_Velocity = rhs.m_Velocity;
		m_Speed = rhs.m_Speed;
		m_RotationTime = rhs.m_RotationTime;
		m_MaxHeight = rhs.m_MaxHeight;

		rhs.m_Velocity = Vector2f{};
		rhs.m_Speed = 0.f;
		rhs.m_RotationTime = 0.f;
		rhs.m_MaxHeight = 0.f;
	}
	return *this;
}

void Axe::Update(float elapsedSec, const Level& level, const Rectf& camera)
{
	if (m_IsThrown && (!m_IsOutOfBounds && !m_HasHit))
	{
		HandlePosition(elapsedSec);
		level.IsOutOfBounds(GetShape(), m_IsOutOfBounds);
	}
	else
	{
		Weapon::Update(elapsedSec, level, camera);
	}
}

void Axe::Draw() const
{
	const Rectf shapeWeapon{GetShape()};
	glPushMatrix();
	glTranslatef(m_Position.x + shapeWeapon.width / 2, m_Position.y + shapeWeapon.height / 2, 0.f);
	glRotatef(utils::RadToDegree(m_RotationTime), 0, 0, 1);
	glTranslatef(-shapeWeapon.width / 2, -shapeWeapon.height / 2, 0.f);
	m_pSpriteItem->Draw();
	glPopMatrix();
}


void Axe::Reset()
{
	m_Velocity.x = m_Speed;
	m_Velocity.y = m_MaxHeight;
	m_IsOutOfBounds = false;
	m_HasHit = false;
}

void Axe::HandlePosition(float elapsedSec)
{
	// Gravity
	m_Velocity.y += m_Acceleration * elapsedSec;

	// Movement based on velocity
	if (m_Direction == utils::Direction::right)
	{
		m_Position.x += m_Velocity.x * elapsedSec;
	}
	else
	{
		m_Position.x -= m_Velocity.x * elapsedSec;
	}
	const float rotationSpeed{4.f};
	m_RotationTime += rotationSpeed * elapsedSec;

	m_Position.y += m_Velocity.y * elapsedSec;
}
