#include "pch.h"
#include "Spirit.h"

#include "Sprite.h"
#include "Texture.h"
#include "utils.h"

unsigned Spirit::m_CounterSpirit = 0;
Texture* Spirit::m_pTextureSpirit = nullptr;

Spirit::Spirit(const Point2f& pos): Enemy{pos}, m_Direction{utils::Direction::left},
                                    m_CurrentState{SpiritState::spawn}, m_Speed{25.f}
{
	m_Health = 60.f;
	const std::string filePath{m_FolderPath + "Spirit.png"};
	if (!m_pTextureSpirit)
	{
		m_pTextureSpirit = new Texture{filePath};
	}
	m_pSpriteEnemy = new Sprite{filePath, m_pTextureSpirit, 3, 2, 0, 3, 1, 2};
	m_pSpriteEnemy->SetNrFrame(0, 3);
	m_Shape = Rectf{0, 0, m_pSpriteEnemy->GetShape().width, m_pSpriteEnemy->GetShape().height};
	m_Damage = 2.f;
	++m_CounterSpirit;
}

Spirit::Spirit(const Point2f& pos, const utils::Direction& direction): Spirit{pos}
{
	m_Direction = direction;
}

Spirit::~Spirit()
{
	if (m_CounterSpirit == 1)
	{
		delete m_pTextureSpirit;
		m_pTextureSpirit = nullptr;
	}
	--m_CounterSpirit;
}

Spirit::Spirit(const Spirit& other): Enemy{other}, m_Direction{other.m_Direction},
                                     m_CurrentState{other.m_CurrentState}, m_Speed{other.m_Speed},
                                     m_Velocity{other.m_Velocity}
{
	++m_CounterSpirit;
}

Spirit::Spirit(Spirit&& other) noexcept : Enemy{std::move(other)}, m_Direction{other.m_Direction},
                                          m_CurrentState{other.m_CurrentState}, m_Speed{other.m_Speed},
                                          m_Velocity{other.m_Velocity}
{
}

Spirit& Spirit::operator=(const Spirit& rhs)
{
	if (&rhs != this)
	{
		Enemy::operator=(rhs);
		m_Velocity = rhs.m_Velocity;
		m_CurrentState = rhs.m_CurrentState;
		m_Speed = rhs.m_Speed;
		m_Direction = rhs.m_Direction;
	}
	return *this;
}

Spirit& Spirit::operator=(Spirit&& rhs) noexcept
{
	if (&rhs != this)
	{
		Enemy::operator=(std::move(rhs));
		m_Velocity = rhs.m_Velocity;
		m_CurrentState = rhs.m_CurrentState;
		m_Speed = rhs.m_Speed;
		m_Direction = rhs.m_Direction;
	}
	return *this;
}

void Spirit::Update(float elapsedSec)
{
	Enemy::Update(elapsedSec);
	if (!m_IsHurt && m_State != EnemyState::dying)
	{
		HandlePosition(elapsedSec);
		m_pSpriteEnemy->HandleFrames(elapsedSec);
		HandleSpawning();
	}
	if (m_IsHurt)
	{
		m_Velocity.Set(0, 0);
	}
}

void Spirit::Update(float elapsedSec, const Rectf& actorHitBox)
{
	HandlePosition(elapsedSec, actorHitBox);
}

void Spirit::Draw() const
{
	Enemy::Draw();
}

void Spirit::HandleSpawning()
{
	if (m_CurrentState == SpiritState::spawn)
	{
		if (m_pSpriteEnemy->HasDoneLoop())
		{
			m_pSpriteEnemy->SetRowFrame(1);
			m_pSpriteEnemy->SetNrFrame(0, 2);
			m_CurrentState = SpiritState::follow;
		}
	}
}

void Spirit::HandlePosition(float elapsedSec)
{
	if (m_CurrentState == SpiritState::follow)
	{
		m_Position.x += m_Velocity.x;
		m_Position.y += m_Velocity.y;
	}
	m_Velocity.Set(0, 0);
}

void Spirit::ChangeDirection(const Vector2f& directionVector)
{
	if (directionVector.x < 0)
	{
		m_Direction = utils::Direction::left;
	}
	if (directionVector.x > 0)
	{
		m_Direction = utils::Direction::right;
	}
}

void Spirit::HandlePosition(float elapsedSec, const Rectf& actorHitBox)
{
	if (m_CurrentState == SpiritState::follow)
	{
		// Get the vector direction from enemy to actor
		const Point2f actorPos{utils::GetCenterOfRectangle(actorHitBox)};
		const Vector2f directionVector{
			utils::GetCenterOfRectangle(Rectf{m_Position.x, m_Position.y, m_Shape.width, m_Shape.height}), actorPos
		};
		ChangeDirection(directionVector);
		const Vector2f normalized{directionVector.Normalized()};
		m_Velocity.x += normalized.x * elapsedSec * m_Speed;
		m_Velocity.y += normalized.y * elapsedSec * m_Speed;
	}
}

bool Spirit::CanBeHurt() const
{
	return m_CurrentState != SpiritState::spawn;
}

void Spirit::DrawAlive() const
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

void Spirit::DrawDying() const
{
	Enemy::DrawDying();
}
