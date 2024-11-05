#include "pch.h"
#include "Boomerang.h"

#include <iostream>

#include "Level.h"
#include "Sprite.h"
#include "Texture.h"
#include "utils.h"
#include "Weapon.h"


Boomerang::Boomerang(const Point2f& pos, bool isThrown): Weapon{
	                                                         pos, WeaponType::boomerang, isThrown
                                                         }, m_StartPosition{pos}, m_Speed{80.f},
                                                         m_LengthMax{120.f}, m_Velocity{Vector2f{}},
                                                         m_RotationTime{0.f}, m_IsGoingBack{false}
{
	m_Damage = 50.f;
}

Boomerang::Boomerang(const Point2f& pos, const Point2f& startPos, bool isThrown) : Boomerang{pos, isThrown}
{
	m_StartPosition = startPos;
}

Boomerang::Boomerang(const Boomerang& boomerang) = default;

Boomerang::Boomerang(Boomerang&& boomerang) noexcept: Weapon{std::move(boomerang)},
                                                      m_StartPosition{boomerang.m_StartPosition},
                                                      m_Speed{boomerang.m_Speed},
                                                      m_LengthMax{boomerang.m_LengthMax},
                                                      m_Velocity{boomerang.m_Velocity},
                                                      m_RotationTime{boomerang.m_RotationTime},
                                                      m_IsGoingBack{boomerang.m_IsGoingBack}
{
	boomerang.m_StartPosition = Point2f{};
	boomerang.m_Speed = 0.f;
	boomerang.m_LengthMax = 0.f;
	boomerang.m_RotationTime = 0.f;
	boomerang.m_Velocity = Vector2f{};
	boomerang.m_IsGoingBack = false;
}

Boomerang& Boomerang::operator=(const Boomerang& rhs)
{
	if (&rhs != this)
	{
		Weapon::operator=(rhs);
		m_StartPosition = rhs.m_StartPosition;
		m_Speed = rhs.m_Speed;
		m_LengthMax = rhs.m_LengthMax;
		m_RotationTime = rhs.m_RotationTime;
		m_Velocity = rhs.m_Velocity;
		m_IsGoingBack = rhs.m_IsGoingBack;
	}
	return *this;
}

Boomerang& Boomerang::operator=(Boomerang&& rhs) noexcept
{
	if (&rhs != this)
	{
		Weapon::operator=(std::move(rhs));
		m_StartPosition = rhs.m_StartPosition;
		m_Speed = rhs.m_Speed;
		m_LengthMax = rhs.m_LengthMax;
		m_RotationTime = rhs.m_RotationTime;
		m_Velocity = rhs.m_Velocity;
		m_IsGoingBack = rhs.m_IsGoingBack;

		rhs.m_StartPosition = Point2f{};
		rhs.m_Speed = 0.f;
		rhs.m_LengthMax = 0.f;
		rhs.m_RotationTime = 0.f;
		rhs.m_Velocity = Vector2f{};
		rhs.m_IsGoingBack = false;
	}
	return *this;
}

Boomerang::~Boomerang()
{
	if (m_InstanceCounter <= 1)
	{
		delete m_pTextureItem;
		m_pTextureItem = nullptr;
	}
	delete m_pSpriteItem;
	m_pSpriteItem = nullptr;
	--m_InstanceCounter;
};

void Boomerang::Draw() const
{
	const Rectf shapeWeapon{GetShape()};
	glPushMatrix();
	glTranslatef(m_Position.x + shapeWeapon.width / 2, m_Position.y + shapeWeapon.height / 2, 0.f);
	glRotatef(utils::RadToDegree(m_RotationTime), 0, 0, 1);
	glTranslatef(-shapeWeapon.width / 2, -shapeWeapon.height / 2, 0.f);
	m_pSpriteItem->Draw();
	glPopMatrix();
}

void Boomerang::Update(float elapsedSec, const Level& level, const Rectf& camera)
{
	if (m_IsThrown && (!m_HasHit && !m_IsOutOfBounds))
	{
		HandleGoingBack();
		HandlePosition(elapsedSec);
		level.IsOutOfBounds(GetShape(), m_IsOutOfBounds);
		IsOutOfCamera(camera);
	}
	else
	{
		Weapon::Update(elapsedSec, level, camera);
	}
}

bool Boomerang::HasHitPlayer(const Rectf& shapePlayer)
{
	if (m_IsGoingBack)
	{
		if (utils::IsOverlapping(shapePlayer, GetShape()))
		{
			m_HasHit = true;
			return true;
		}
	}
	return false;
}

void Boomerang::Reset()
{
	m_IsOutOfBounds = false;
	m_HasHit = false;
	m_IsGoingBack = false;
	m_StartPosition = m_Position;
}

void Boomerang::SetStartPosition(const Point2f& centerPlayer)
{
	m_StartPosition = centerPlayer;
}

void Boomerang::HandlePosition(float elapsedSec)
{
	if (m_IsGoingBack)
	{
		m_Velocity.x = -m_Speed * elapsedSec;
	}
	else
	{
		m_Velocity.x = m_Speed * elapsedSec;
	}
	const float rotationSpeed{5.f};
	m_RotationTime += rotationSpeed * elapsedSec;
	m_Position.x += m_Velocity.x;
}

void Boomerang::HandleGoingBack()
{
	if (!m_IsGoingBack)
	{
		if (m_StartPosition.x + m_LengthMax < m_Position.x)
		{
			m_IsGoingBack = true;
		}
	}
}

void Boomerang::IsOutOfCamera(const Rectf& camera)
{
	if (m_IsGoingBack)
	{
		if (camera.left > m_Position.x + GetShape().width)
		{
			m_IsOutOfBounds = true;
		}
	}
}
