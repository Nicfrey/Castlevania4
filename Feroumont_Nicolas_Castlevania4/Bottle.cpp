#include "pch.h"
#include "Bottle.h"

#include "Level.h"
#include "Sprite.h"
#include "utils.h"

float Bottle::m_TimeFire = 2.5f;

Bottle::Bottle(const Point2f& pos, bool isThrown): Weapon{pos, WeaponType::bottle, isThrown}, m_Velocity{Vector2f{}},
                                                   m_Speed{200.f}, m_RotationTime{0.f},
                                                   m_CurrentState{BottleState::bottle}, m_CurrentTimeFire{0.f}
{
	m_Damage = 10.f;
	m_Velocity.x = m_Speed;
	const std::string filePath{"Sprites/Items.png"};
	const float scale{1.5f};
	m_pSpriteFire = new Sprite{filePath, m_pTextureItem, 10, 4, 0, 2, 6, 3};
	m_pSpriteFire->SetNrFrame(0, 3);
	m_pSpriteFire->SetScale(scale);
}

Bottle::Bottle(const Bottle& bottle): Weapon{bottle}, m_Velocity{bottle.m_Velocity}, m_Speed{bottle.m_Speed},
                                      m_RotationTime{bottle.m_RotationTime}, m_CurrentState{bottle.m_CurrentState},
                                      m_CurrentTimeFire{bottle.m_CurrentTimeFire}
{
	m_pSpriteFire = new Sprite{*bottle.m_pSpriteFire};
}

Bottle::Bottle(Bottle&& bottle) noexcept: Weapon{std::move(bottle)}, m_Velocity{bottle.m_Velocity},
                                          m_Speed{bottle.m_Speed}, m_RotationTime{bottle.m_RotationTime},
                                          m_CurrentState{bottle.m_CurrentState}, m_pSpriteFire{bottle.m_pSpriteFire},
                                          m_CurrentTimeFire{bottle.m_CurrentTimeFire}
{
	bottle.m_pSpriteFire = nullptr;
	bottle.m_Velocity = Vector2f{};
	bottle.m_CurrentState = BottleState::end;
	bottle.m_RotationTime = 0.f;
	bottle.m_Speed = 0.f;
	bottle.m_CurrentTimeFire = 0.f;
}

Bottle& Bottle::operator=(const Bottle& rhs)
{
	if (&rhs != this)
	{
		delete m_pSpriteFire;
		m_pSpriteFire = new Sprite{*rhs.m_pSpriteFire};
		m_Velocity = rhs.m_Velocity;
		m_CurrentState = rhs.m_CurrentState;
		m_RotationTime = rhs.m_RotationTime;
		m_Speed = rhs.m_Speed;
		m_CurrentTimeFire = rhs.m_CurrentTimeFire;
	}
	return *this;
}

Bottle& Bottle::operator=(Bottle&& rhs) noexcept
{
	if (&rhs != this)
	{
		m_pSpriteFire = rhs.m_pSpriteFire;
		m_Velocity = rhs.m_Velocity;
		m_CurrentState = rhs.m_CurrentState;
		m_RotationTime = rhs.m_RotationTime;
		m_Speed = rhs.m_Speed;
		m_CurrentTimeFire = rhs.m_CurrentTimeFire;

		rhs.m_pSpriteFire = nullptr;
		rhs.m_Velocity = Vector2f{};
		rhs.m_CurrentState = BottleState::end;
		rhs.m_RotationTime = 0.f;
		rhs.m_Speed = 0.f;
		rhs.m_CurrentTimeFire = 0.f;
	}
	return *this;
}

void Bottle::Update(float elapsedSec, const Level& level, const Rectf& camera)
{
	if (m_IsThrown || m_CurrentState != BottleState::end)
	{
		HandleBottleState(elapsedSec, level);
		HandlePosition(elapsedSec);
		HandleSprite(elapsedSec);
	}
	else
	{
		Weapon::Update(elapsedSec, level, camera);
	}
}

void Bottle::Draw() const
{
	const Rectf shapeWeapon{GetShape()};
	glPushMatrix();
	switch (m_CurrentState)
	{
	case BottleState::bottle:
		glTranslatef(m_Position.x + shapeWeapon.width / 2, m_Position.y + shapeWeapon.height / 2, 0.f);
		glRotatef(utils::RadToDegree(m_RotationTime), 0, 0, 1);
		glTranslatef(-shapeWeapon.width / 2, -shapeWeapon.height / 2, 0.f);
		m_pSpriteItem->Draw();
		break;
	case BottleState::fire:
		glTranslatef(m_Position.x, m_Position.y, 0.f);
		m_pSpriteFire->Draw();
		break;
	case BottleState::end:
		// Draw nothing
		break;
	}
	glPopMatrix();
}

Bottle::~Bottle()
{
	delete m_pSpriteFire;
	m_pSpriteFire = nullptr;
}

void Bottle::Reset()
{
	m_CurrentState = BottleState::bottle;
	m_Velocity.x = m_Speed;
	m_Velocity.y = 0.f;
	m_IsOutOfBounds = false;
	m_HasHit = false;
	m_CurrentTimeFire = 0.f;
	m_RotationTime = 0.f;
}

Rectf Bottle::GetShape() const
{
	if (m_CurrentState == BottleState::bottle)
	{
		return Weapon::GetShape();
	}
	return Rectf{m_Position.x, m_Position.y, m_pSpriteFire->GetShape().width, m_pSpriteFire->GetShape().height};
}

void Bottle::HandlePosition(float elapsedSec)
{
	switch (m_CurrentState)
	{
	case BottleState::bottle:
		{
			const float rotationSpeed{3.f};
			m_RotationTime += elapsedSec * rotationSpeed;
		}
		if (m_Direction == utils::Direction::right)
		{
			m_Velocity.x = m_Speed;
		}
		else
		{
			m_Velocity.x = -m_Speed;
		}
		m_Velocity.y += m_Acceleration * elapsedSec;
		break;
	case BottleState::fire:
		m_Velocity.x = 0.f;
		m_Velocity.y = 0.f;
		break;
	case BottleState::end:
		// does nothing
		break;
	}
	m_Position.x += m_Velocity.x * elapsedSec;
	m_Position.y += m_Velocity.y * elapsedSec;
}

void Bottle::HandleBottleState(float elapsedSec, const Level& level)
{
	switch (m_CurrentState)
	{
	case BottleState::bottle:
		if (level.IsOnGround(GetShape()))
		{
			m_CurrentState = BottleState::fire;
		}
		break;
	case BottleState::fire:
		if (m_CurrentTimeFire <= m_TimeFire)
		{
			m_CurrentTimeFire += elapsedSec;
		}
		else
		{
			m_CurrentState = BottleState::end;
		}
		break;
	case BottleState::end:
		m_HasHit = true;
		break;
	}
}

void Bottle::HandleSprite(float elapsedSec) const
{
	if (m_CurrentState == BottleState::fire)
	{
		m_pSpriteFire->HandleFrames(elapsedSec);
	}
}
