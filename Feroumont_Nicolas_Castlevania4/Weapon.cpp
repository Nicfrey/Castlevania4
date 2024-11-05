#include "pch.h"
#include "Weapon.h"

#include <iostream>

#include "SoundEffect.h"
#include "Sprite.h"
#include "utils.h"

Weapon::Weapon(const Point2f& pos, const WeaponType& weapon, bool isThrown): Item{pos}, m_HasHit{false},
                                                                             m_IsThrown{isThrown},
                                                                             m_IsOutOfBounds{false},
                                                                             m_Damage{0.f},
                                                                             m_Direction{utils::Direction::right},
                                                                             m_WeaponType{weapon}
{
	m_IsLimited = !isThrown;
	std::string filePath{};
	switch (weapon)
	{
	case WeaponType::spear:
		m_pSpriteItem->SetRowFrame(1);
		m_pSpriteItem->SetNrFrame(2);
		break;
	case WeaponType::time:
		m_pSpriteItem->SetRowFrame(1);
		m_pSpriteItem->SetNrFrame(7);
		break;
	case WeaponType::boomerang:
		m_pSpriteItem->SetRowFrame(1);
		m_pSpriteItem->SetNrFrame(8);
		break;
	case WeaponType::knives:
		m_pSpriteItem->SetRowFrame(2);
		m_pSpriteItem->SetNrFrame(2);
		break;
	case WeaponType::axe:
		m_pSpriteItem->SetRowFrame(2);
		m_pSpriteItem->SetNrFrame(4);
		break;
	case WeaponType::bottle:
		m_pSpriteItem->SetRowFrame(2);
		m_pSpriteItem->SetNrFrame(5);
		break;
	}
}

Weapon::Weapon(Weapon&& weapon) noexcept : Item{std::move(weapon)}, m_HasHit{weapon.m_HasHit},
                                           m_IsThrown{weapon.m_IsThrown}, m_IsOutOfBounds{weapon.m_IsOutOfBounds},
                                           m_Damage{weapon.m_Damage}, m_Direction{weapon.m_Direction},
                                           m_WeaponType{weapon.m_WeaponType}
{
	weapon.m_IsThrown = false;
	weapon.m_HasHit = false;
	weapon.m_IsOutOfBounds = false;
	weapon.m_Damage = 0.f;
	weapon.m_Direction = utils::Direction::upRight;
	weapon.m_WeaponType = WeaponType::time;
}

Weapon& Weapon::operator=(const Weapon& rhs)
{
	if (&rhs != this)
	{
		Item::operator=(rhs);
		m_IsThrown = rhs.m_IsThrown;
		m_HasHit = rhs.m_HasHit;
		m_IsOutOfBounds = rhs.m_IsOutOfBounds;
		m_Damage = rhs.m_Damage;
		m_Direction = rhs.m_Direction;
		m_WeaponType = rhs.m_WeaponType;
	}
	return *this;
}

Weapon& Weapon::operator=(Weapon&& rhs) noexcept
{
	if (&rhs != this)
	{
		Item::operator=(rhs);
		m_IsThrown = rhs.m_IsThrown;
		m_HasHit = rhs.m_HasHit;
		m_IsOutOfBounds = rhs.m_IsOutOfBounds;
		m_Damage = rhs.m_Damage;
		m_Direction = rhs.m_Direction;
		m_WeaponType = rhs.m_WeaponType;

		rhs.m_IsThrown = false;
		rhs.m_HasHit = false;
		rhs.m_IsOutOfBounds = false;
		rhs.m_Damage = 0.f;
		rhs.m_Direction = utils::Direction::upRight;
		rhs.m_WeaponType = WeaponType::time;
	}
	return *this;
}

void Weapon::Update(float elapsedSec, const Level& level, const Rectf& camera)
{
	Item::Update(elapsedSec, level, camera);
}

void Weapon::Draw() const
{
	Item::Draw();
}

Weapon::WeaponType Weapon::GetWeaponType() const
{
	return m_WeaponType;
}

bool Weapon::IsOutOfBound() const
{
	return m_IsOutOfBounds;
}

bool Weapon::HasBeenHit() const
{
	return m_HasHit;
}

float Weapon::GetDamage() const
{
	return m_Damage;
}

void Weapon::SetIsThrown(bool isThrown)
{
	m_IsThrown = isThrown;
}

void Weapon::Reset()
{
}

void Weapon::SetDirection(const utils::Direction& direction)
{
	m_Direction = direction;
}

void Weapon::Hit()
{
	m_HasHit = true;
}

unsigned Weapon::GetRowFrame() const
{
	return m_pSpriteItem->GetRowFrame();
}

unsigned Weapon::GetStartColumn() const
{
	return m_pSpriteItem->GetStartCol();
}
