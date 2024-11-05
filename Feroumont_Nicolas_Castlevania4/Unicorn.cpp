#include "pch.h"
#include "Unicorn.h"

#include <iostream>

#include "Avatar.h"
#include "Sprite.h"
#include "Texture.h"
#include "utils.h"

unsigned Unicorn::m_CounterUnicorn = 0;
Texture* Unicorn::m_pTextureUnicorn = nullptr;

Unicorn::Unicorn(const Point2f& pos): Enemy{pos}, m_Direction{utils::Direction::left},
                                      m_CurrentState{UnicornState::sleep}, m_Speed{40.f}, m_RangeDetection{100.f},
                                      m_RangeDid{0.f}
{
	m_Health = 70.f;
	const std::string filePath{m_FolderPath + "Unicorn.png"};
	if (!m_pTextureUnicorn)
	{
		m_pTextureUnicorn = new Texture{filePath};
	}
	m_pSpriteEnemy = new Sprite{filePath, m_pTextureUnicorn, 3, 1, 0, 3};
	m_pSpriteEnemy->SetNrFrame(0);
	m_Shape = Rectf{0, 0, m_pSpriteEnemy->GetShape().width, m_pSpriteEnemy->GetShape().height};
	m_Damage = 2.f;
	++m_CounterUnicorn;
}

Unicorn::Unicorn(const Point2f& pos, const utils::Direction& direction): Unicorn{pos}
{
	m_Direction = direction;
}

Unicorn::~Unicorn()
{
	if (m_CounterUnicorn == 1)
	{
		delete m_pTextureUnicorn;
		m_pTextureUnicorn = nullptr;
	}
	--m_CounterUnicorn;
}

Unicorn::Unicorn(const Unicorn& other) : Enemy{other}, m_Direction{other.m_Direction},
                                         m_CurrentState{other.m_CurrentState}, m_Speed{other.m_Speed},
                                         m_Velocity{other.m_Velocity}, m_RangeDetection{other.m_RangeDetection},
                                         m_RangeDid{other.m_RangeDid}
{
	++m_CounterUnicorn;
}

Unicorn::Unicorn(Unicorn&& other) noexcept : Enemy{std::move(other)}, m_Direction{other.m_Direction},
                                             m_CurrentState{other.m_CurrentState}, m_Speed{other.m_Speed},
                                             m_Velocity{other.m_Velocity}, m_RangeDetection{other.m_RangeDetection},
                                             m_RangeDid{other.m_RangeDid}

{
}

void Unicorn::Update(float elapsedSec)
{
	Enemy::Update(elapsedSec);
	if (m_CurrentState != UnicornState::sleep && (!m_IsHurt && m_State != EnemyState::dying))
	{
		HandlePosition(elapsedSec);
		HasReachRange();
		m_pSpriteEnemy->HandleFrames(elapsedSec);
	}
	if (m_IsHurt)
	{
		m_Velocity.Set(0, 0);
	}
}

void Unicorn::Update(float elapsedSec, const Rectf& actorHitBox)
{
	HandleDetectionPlayer(actorHitBox);
	HandlePosition(elapsedSec, actorHitBox);
}

void Unicorn::Draw() const
{
	Enemy::Draw();
}

void Unicorn::HandlePosition(float elapsedSec)
{
	switch (m_CurrentState)
	{
	case UnicornState::sleep:
		// do not move
		break;
	case UnicornState::dash:
		m_Velocity.Set(0, 0);
		if (m_Direction == utils::Direction::left)
		{
			m_Velocity.x -= m_Speed * elapsedSec;
		}
		else
		{
			m_Velocity.x += m_Speed * elapsedSec;
		}
		m_RangeDid += m_Speed * elapsedSec;
		break;
	case UnicornState::follow:
		// Do nothing because need the actor
		break;
	}
	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
}

void Unicorn::HandlePosition(float elapsedSec, const Rectf& actorHitBox)
{
	if (m_CurrentState == UnicornState::follow)
	{
		// Get the vector direction from enemy to actor
		const Point2f actorPos{utils::GetCenterOfRectangle(actorHitBox)};
		const Vector2f directionVector{
			utils::GetCenterOfRectangle(Rectf{m_Position.x, m_Position.y, m_Shape.width, m_Shape.height}), actorPos
		};
		ChangeDirection(directionVector);
		m_Velocity.x += directionVector.x * elapsedSec * elapsedSec;
		m_Velocity.y += directionVector.y * elapsedSec * elapsedSec;
	}
}

void Unicorn::DrawAlive() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);
	if (m_Direction == utils::Direction::right)
	{
		glScalef(-1.f, 1.f, 1.f);
		glTranslatef(-m_Shape.width, 0.f, 0.f);
	}
	m_pSpriteEnemy->Draw();
	glPopMatrix();
}

void Unicorn::DrawDying() const
{
	Enemy::DrawDying();
}

void Unicorn::HandleDetectionPlayer(const Rectf& actorHitBox)
{
	if (m_CurrentState != UnicornState::sleep)
	{
		return;
	}
	std::vector<Point2f> verticesPlayer;
	verticesPlayer.push_back(Point2f{actorHitBox.left, actorHitBox.bottom});
	verticesPlayer.push_back(Point2f{actorHitBox.left, actorHitBox.bottom + actorHitBox.height});
	verticesPlayer.push_back(Point2f{actorHitBox.left + actorHitBox.width, actorHitBox.bottom + actorHitBox.height});
	verticesPlayer.push_back(Point2f{actorHitBox.left + actorHitBox.width, actorHitBox.bottom});

	const Point2f rayP1{m_Position.x + m_Shape.width / 2, m_Position.y + m_Shape.height / 2};
	utils::HitInfo hitInfo;
	if (m_Direction == utils::Direction::left)
	{
		const Point2f rayP2{rayP1.x - m_RangeDetection, rayP1.y};
		if (utils::Raycast(verticesPlayer, rayP1, rayP2, hitInfo))
		{
			m_CurrentState = UnicornState::dash;
			m_pSpriteEnemy->SetNrFrame(2);
		}
	}
	else
	{
		const Point2f rayP2{rayP1.x + m_RangeDetection, rayP1.y};
		if (utils::Raycast(verticesPlayer, rayP1, rayP2, hitInfo))
		{
			m_CurrentState = UnicornState::dash;
			m_pSpriteEnemy->SetNrFrame(2);
		}
	}
}

void Unicorn::HasReachRange()
{
	if (m_RangeDid >= m_RangeDetection && m_CurrentState != UnicornState::follow)
	{
		m_CurrentState = UnicornState::follow;
		m_pSpriteEnemy->SetNrFrame(1, 2);
	}
}

void Unicorn::ChangeDirection(const Vector2f directionUnicorn)
{
	if (directionUnicorn.x < 0)
	{
		m_Direction = utils::Direction::left;
	}
	if (directionUnicorn.x > 0)
	{
		m_Direction = utils::Direction::right;
	}
}
