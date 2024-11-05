#include "pch.h"
#include "Worm.h"

#include <iostream>
#include <vector>

#include "Level.h"
#include "Sprite.h"
#include "Texture.h"
#include "utils.h"

unsigned Worm::m_CounterWorm = 0;
Texture* Worm::m_pTextureWorm;

Worm::Worm(const Point2f& pos) : Enemy{pos}, m_CurrentState{WormState::waiting}, m_Direction{utils::Direction::left},
                                 m_Speed{40.f}, m_Gravity{-981.f}, m_Velocity{0, 0}
{
	m_Health = 50.f;
	const std::string filePath{m_FolderPath + "Worm.png"};
	if (!m_pTextureWorm)
	{
		m_pTextureWorm = new Texture{filePath};
	}
	m_pSpriteEnemy = new Sprite{filePath, m_pTextureWorm, 3, 1, 0, 2, 6};
	m_pSpriteEnemy->SetNrFrame(0, 3);
	const float scale{1.5f};
	m_pSpriteEnemy->SetScale(scale);
	m_Shape = Rectf{0.f, 0.f, m_pSpriteEnemy->GetShape().width, m_pSpriteEnemy->GetShape().height};
	m_Damage = 2.f;
	m_Position.y -= m_Shape.height;
	++m_CounterWorm;
}

Worm::Worm(const Worm& worm): Enemy{worm}, m_CurrentState{worm.m_CurrentState}, m_Direction{worm.m_Direction},
                              m_Speed{worm.m_Speed}, m_Gravity{worm.m_Gravity}, m_Velocity{worm.m_Velocity}
{
	++m_CounterWorm;
}

Worm::Worm(Worm&& worm) noexcept: Enemy{std::move(worm)}, m_CurrentState{worm.m_CurrentState},
                                  m_Direction{worm.m_Direction}, m_Speed{worm.m_Speed}, m_Gravity{worm.m_Gravity},
                                  m_Velocity{worm.m_Velocity}
{
	worm.m_CurrentState = WormState::waiting;
	worm.m_Direction = utils::Direction::upLeft;
	worm.m_Speed = 0.f;
	worm.m_Gravity = 0.f;
	worm.m_Velocity = Vector2f{};
}

Worm& Worm::operator=(const Worm& rhs)
{
	if (&rhs != this)
	{
		Enemy::operator=(rhs);
		m_CurrentState = rhs.m_CurrentState;
		m_Direction = rhs.m_Direction;
		m_Speed = rhs.m_Speed;
		m_Gravity = rhs.m_Gravity;
		m_Velocity = rhs.m_Velocity;
	}
	return *this;
}

Worm& Worm::operator=(Worm&& rhs) noexcept
{
	if (&rhs != this)
	{
		Enemy::operator=(std::move(rhs));
		m_CurrentState = rhs.m_CurrentState;
		m_Direction = rhs.m_Direction;
		m_Speed = rhs.m_Speed;
		m_Gravity = rhs.m_Gravity;
		m_Velocity = rhs.m_Velocity;

		rhs.m_CurrentState = WormState::waiting;
		rhs.m_Direction = utils::Direction::upLeft;
		rhs.m_Speed = 0.f;
		rhs.m_Gravity = 0.f;
		rhs.m_Velocity = Vector2f{};
	}
	return *this;
}

Worm::~Worm()
{
	if (m_CounterWorm <= 1)
	{
		delete m_pTextureWorm;
		m_pTextureWorm = nullptr;
	}
	--m_CounterWorm;
}

void Worm::Update(float elapsedSec)
{
	Enemy::Update(elapsedSec);
	m_Velocity.x = 0;
	if (m_CurrentState != WormState::waiting && (!m_IsHurt && m_State != EnemyState::dying))
	{
		HandlePosition(elapsedSec);
		m_pSpriteEnemy->HandleFrames(elapsedSec);
	}
}

void Worm::Update(const Rectf& hitBoxPlayer, const Level& level)
{
	switch (m_CurrentState)
	{
	case WormState::waiting:
		HandleDetectionPlayer(hitBoxPlayer);
		break;
	case WormState::falling:
		level.HandleCollisionActorsOnGround(m_Position, m_Shape, m_Velocity);
		if (level.IsOnGround(Rectf{m_Position.x, m_Position.y, m_Shape.width, m_Shape.height}))
		{
			m_CurrentState = WormState::attacking;
		}
		break;
	case WormState::attacking:
		level.HandleCollisionActorsOnGround(m_Position, m_Shape, m_Velocity);
		HandleDirection(hitBoxPlayer);
		break;
	}
}

void Worm::Draw() const
{
	Enemy::Draw();
}


void Worm::DrawAlive() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);
	if (m_CurrentState == WormState::waiting)
	{
		glTranslatef(0.f, m_Shape.height, 0.f);
		glScalef(1.f, -1.f, 1.f);
	}
	m_pSpriteEnemy->Draw();
	glPopMatrix();
}

void Worm::DrawDying() const
{
	Enemy::DrawDying();
}

void Worm::HandlePosition(float elapsedSec)
{
	switch (m_CurrentState)
	{
	case WormState::waiting:
		// Do not move
		break;
	case WormState::falling:
		m_Velocity.y += m_Gravity * elapsedSec;
		break;
	case WormState::attacking:
		m_Velocity.y += m_Gravity * elapsedSec;
		if (m_Direction == utils::Direction::left)
		{
			m_Velocity.x -= m_Speed * elapsedSec;
		}
		else
		{
			m_Velocity.x += m_Speed * elapsedSec;
		}
		break;
	}
	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y * elapsedSec;
}

void Worm::HandleDetectionPlayer(const Rectf& hitBoxPlayer)
{
	std::vector<Point2f> verticesPlayer;
	verticesPlayer.push_back(Point2f{hitBoxPlayer.left, hitBoxPlayer.bottom});
	verticesPlayer.push_back(Point2f{hitBoxPlayer.left, hitBoxPlayer.bottom + hitBoxPlayer.height});
	verticesPlayer.push_back(Point2f{
		hitBoxPlayer.left + hitBoxPlayer.width, hitBoxPlayer.bottom + hitBoxPlayer.height
	});
	verticesPlayer.push_back(Point2f{hitBoxPlayer.left + hitBoxPlayer.width, hitBoxPlayer.bottom});

	const Point2f rayP1{m_Position.x + m_Shape.width / 2, m_Position.y + m_Shape.height / 2};
	const Point2f rayP2{rayP1.x, rayP1.y - 1000.f};
	utils::HitInfo hitInfo;
	if (utils::Raycast(verticesPlayer, rayP1, rayP2, hitInfo))
	{
		m_CurrentState = WormState::falling;
	}
}

void Worm::HandleDirection(const Rectf& hitBoxPlayer)
{
	const Point2f posPlayer{hitBoxPlayer.left, hitBoxPlayer.bottom};
	if (posPlayer.x < m_Position.x)
	{
		m_Direction = utils::Direction::left;
	}
	if (posPlayer.x > m_Position.x)
	{
		m_Direction = utils::Direction::right;
	}
}
