#include "pch.h"
#include "Projectile.h"

#include <iostream>

#include "Level.h"
#include "Sprite.h"
#include "Texture.h"
#include "utils.h"

unsigned Projectile::m_InstanceProjectile = 0;
Texture* Projectile::m_pTextureProjectile = nullptr;

Projectile::Projectile(const Point2f& pos, const utils::Direction& direction, const std::string& filePath) : GameObject{
	++m_InstanceProjectile, pos
}, m_HasCollided{false}
{
	if (!m_pTextureProjectile)
	{
		m_pTextureProjectile = new Texture{filePath};
	}
	m_pSpriteProjectile = new Sprite{filePath, m_pTextureProjectile, 2, 1, 0, 1, 2};
	m_pSpriteProjectile->SetNrFrame(0, 2);
	switch (direction)
	{
	case utils::Direction::left:
		m_Speed = -75.f;
		break;
	case utils::Direction::right:
		m_Speed = 75.f;
		break;
	}
}

Projectile::Projectile(const Projectile& projectile): GameObject{projectile}, m_Speed(projectile.m_Speed),
                                                      m_HasCollided{projectile.m_HasCollided}
{
	m_pSpriteProjectile = new Sprite{*projectile.m_pSpriteProjectile};
	++m_InstanceProjectile;
}

Projectile::Projectile(Projectile&& projectile) noexcept: GameObject{std::move(projectile)},
                                                          m_pSpriteProjectile{projectile.m_pSpriteProjectile},
                                                          m_Speed(projectile.m_Speed),
                                                          m_HasCollided{projectile.m_HasCollided}
{
	projectile.m_pSpriteProjectile = nullptr;
}

Projectile& Projectile::operator=(const Projectile& rhs)
{
	if (&rhs != this)
	{
		delete m_pSpriteProjectile;
		m_pSpriteProjectile = nullptr;
		m_pSpriteProjectile = new Sprite{*rhs.m_pSpriteProjectile};
	}
	return *this;
}

Projectile& Projectile::operator=(Projectile&& rhs) noexcept
{
	if (&rhs != this)
	{
		delete m_pSpriteProjectile;
		m_pSpriteProjectile = rhs.m_pSpriteProjectile;
		m_Speed = rhs.m_Speed;
		m_HasCollided = rhs.m_HasCollided;
	}
	return *this;
}

Projectile::~Projectile()
{
	if (m_InstanceProjectile == 1)
	{
		delete m_pTextureProjectile;
		m_pTextureProjectile = nullptr;
	}
	delete m_pSpriteProjectile;
	m_pSpriteProjectile = nullptr;
	--m_InstanceProjectile;
}

void Projectile::Update(float elapsedSec, const Level& level)
{
	m_Position.x += m_Speed * elapsedSec;
	m_pSpriteProjectile->HandleFrames(elapsedSec);
	m_HasCollided = level.HandleCollisionProjectile(GetShape());
}

void Projectile::Draw() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);
	m_pSpriteProjectile->Draw();
	glPopMatrix();
}

Rectf Projectile::GetShape() const
{
	return Rectf{
		m_Position.x, m_Position.y, m_pSpriteProjectile->GetShape().width, m_pSpriteProjectile->GetShape().height
	};
}

bool Projectile::GetHasCollided() const
{
	return m_HasCollided;
}

bool Projectile::IsOverlapping(const Rectf& shape) const
{
	return utils::IsOverlapping(shape, GetShape());
}
