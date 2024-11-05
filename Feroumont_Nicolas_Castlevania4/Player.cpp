#include "pch.h"
#include "Player.h"

#include <iostream>
#include <utility>

#include "Avatar.h"
#include "Axe.h"
#include "Boomerang.h"
#include "Bottle.h"
#include "Knife.h"
#include "Level.h"
#include "SoundEffect.h"
#include "utils.h"
#include "Weapon.h"

const unsigned Player::m_MaxHealth = 16;
const unsigned Player::m_StartAmountWeapon = 5;
const unsigned Player::m_StartNrLife = 3;

Player::Player(const unsigned& levelBlock, const unsigned& previousBlock) : m_Score{0},
                                                                            m_AmountOfSubWeapon{m_StartAmountWeapon},
                                                                            m_Health{m_MaxHealth}, m_HasSpear{false},
                                                                            m_NrLife{m_StartNrLife},
                                                                            m_pCurrentWeapon{nullptr},
                                                                            m_IsInvincible{false},
                                                                            m_CurrentTimeInvincible{0.f},
                                                                            m_ValueUpdated{false}
{
	m_pAvatar = new Avatar{};
	ChangeBlock(levelBlock, previousBlock);
	const std::string filePath{"Sounds/SoundEffect/InvincibilityOff.mp3"};
	m_pSoundInvincibilityOff = new SoundEffect{filePath};
}

Player::~Player()
{
	delete m_pAvatar;
	m_pAvatar = nullptr;
	delete m_pCurrentWeapon;
	m_pAvatar = nullptr;
	delete m_pSoundInvincibilityOff;
	m_pSoundInvincibilityOff = nullptr;
}

void Player::Draw() const
{
	m_pAvatar->Draw();
	if (m_pAvatar->CannotThrowSubWeapon())
	{
		m_pCurrentWeapon->Draw();
	}
}

void Player::Update(float elapsedSec, const Level& level, const Rectf& camera)
{
	IsAvatarDead();
	m_pAvatar->Update(elapsedSec, level, IsDead(), CanThrowSubWeapon());
	HandleInvincibility(elapsedSec);
	if (m_pAvatar->CannotThrowSubWeapon())
	{
		m_pCurrentWeapon->SetIsThrown(true);
		HandleSubWeapon(elapsedSec, level, camera);
	}
	else if (m_pCurrentWeapon)
	{
		PrepareSubWeapon();
	}
}

Rectf Player::GetHitBoxAvatar() const
{
	return m_pAvatar->GetHitBoxBody();
}

unsigned Player::GetScore() const
{
	return m_Score;
}

void Player::AddScore(unsigned scoreAdded)
{
	m_Score += scoreAdded;
	SetValueUpdated(true);
}

void Player::AddAmountOfSubWeapon(unsigned amountAdded)
{
	m_AmountOfSubWeapon += amountAdded;
	SetValueUpdated(true);
}

void Player::AddHealth(unsigned amountAdded)
{
	m_Health += amountAdded;
	if (m_Health > m_MaxHealth)
	{
		m_Health = m_MaxHealth;
	}
	SetValueUpdated(true);
}

void Player::RemoveHealth(unsigned amountRemoved)
{
	if (m_pAvatar->GetActionState() != Avatar::ActionState::hurting)
	{
		m_pAvatar->HitByEnemy(m_Health, amountRemoved);
		SetValueUpdated(true);
	}
}

void Player::SetCurrentWeapon(Weapon weapon)
{
	delete m_pCurrentWeapon;
	m_pCurrentWeapon = nullptr;
	// Check every kind of weapon
	switch (weapon.GetWeaponType())
	{
	case Weapon::WeaponType::time:
		m_pCurrentWeapon = new Weapon{std::move(weapon)};
		break;
	case Weapon::WeaponType::boomerang:
		m_pCurrentWeapon = new Boomerang{m_pAvatar->GetPosition()};
		break;
	case Weapon::WeaponType::knives:
		{
			Knife* pKnife{new Knife{m_pAvatar->GetPosition()}};
			m_pCurrentWeapon = pKnife;
		}
		break;
	case Weapon::WeaponType::axe:
		{
			Axe* pAxe{new Axe{m_pAvatar->GetPosition()}};
			m_pCurrentWeapon = pAxe;
		}
		break;
	case Weapon::WeaponType::bottle:
		{
			Bottle* pAxe{new Bottle{m_pAvatar->GetPosition()}};
			m_pCurrentWeapon = pAxe;
		}
		break;
	case Weapon::WeaponType::spear:
		m_pCurrentWeapon = new Weapon{std::move(weapon)};
		break;
	}
	m_pAvatar->HasSubWeapon(true);
	SetValueUpdated(true);
}

float Player::GetDamageMainWeapon() const
{
	if (m_HasSpear)
	{
		return m_DamageSpear;
	}
	return m_DamageRope;
}

void Player::HasSpear()
{
	m_HasSpear = true;
	m_pAvatar->HasSpear(m_HasSpear);
}

void Player::ChangeBlock(const unsigned& currentBlock, const unsigned& previousBlock) const
{
	switch (currentBlock)
	{
	case 1:
	case 2:
		m_pAvatar->SetPosition(0.f, 30.f);
		break;
	case 3:
		m_pAvatar->SetPosition(0.f, 30.f);
		break;
	case 4:
		m_pAvatar->SetPosition(0.f, 38.f);
		break;
	case 5:
		m_pAvatar->SetPosition(0.f, 100.f);
		break;
	case 6:
		if (previousBlock == 7)
		{
			m_pAvatar->SetPosition(1700, 180);
		}
		else
		{
			m_pAvatar->SetPosition(0.f, 38.f);
		}
		break;
	case 7:
		m_pAvatar->SetPosition(0.f, 100.f);
		break;
	}
}

Rectf Player::GetShapeMainWeapon() const
{
	return m_pAvatar->GetShapeWeapon();
}

Rectf Player::GetShapeSubWeapon() const
{
	if (m_pCurrentWeapon != nullptr)
	{
		return m_pCurrentWeapon->GetShape();
	}
	return Rectf{-100, -100, 1, 1};
}

bool Player::HasSubWeapon() const
{
	return m_pCurrentWeapon != nullptr;
}

bool Player::IsThrownSubWeapon() const
{
	return m_pCurrentWeapon != nullptr && m_pAvatar->CannotThrowSubWeapon();
}

void Player::SetInvincibility(bool isInvincible)
{
	m_IsInvincible = isInvincible;
}

bool Player::IsInvincible() const
{
	return m_IsInvincible;
}

float Player::GetDamageSubWeapon() const
{
	return m_pCurrentWeapon->GetDamage();
}

void Player::SubWeaponHit() const
{
	const Knife* pKnife = dynamic_cast<Knife*>(m_pCurrentWeapon);
	if (pKnife != nullptr)
	{
		m_pCurrentWeapon->Hit();
	}
}

bool Player::IsValueUpdated() const
{
	return m_ValueUpdated;
}

void Player::SetValueUpdated(const bool updated)
{
	m_ValueUpdated = updated;
}

int Player::GetHealth() const
{
	return m_Health;
}

unsigned Player::GetAmountOfSubWeapon() const
{
	return m_AmountOfSubWeapon;
}

unsigned Player::GetRowFrameWeapon() const
{
	return m_pCurrentWeapon->GetRowFrame();
}

unsigned Player::GetStartColumnWeapon() const
{
	return m_pCurrentWeapon->GetStartColumn();
}

unsigned Player::GetNrLife() const
{
	return m_NrLife;
}

bool Player::IsDead() const
{
	return m_Health <= 0;
}

void Player::ResetAfterDead()
{
	delete m_pCurrentWeapon;
	m_pCurrentWeapon = nullptr;
	m_Health = m_MaxHealth;
	m_AmountOfSubWeapon = m_StartAmountWeapon;
	m_pAvatar->ResetVelocity();
	m_pAvatar->ResetActionState();
	m_HasSpear = false;
	m_pAvatar->HasSpear(false);
	--m_NrLife;
}

bool Player::IsGameOver() const
{
	return m_NrLife == 0;
}

void Player::ResetAfterGameOver()
{
	delete m_pCurrentWeapon;
	m_pCurrentWeapon = nullptr;
	m_Health = m_MaxHealth;
	m_AmountOfSubWeapon = m_StartAmountWeapon;
	m_pAvatar->ResetVelocity();
	m_NrLife = m_StartNrLife;
	m_HasSpear = false;
	m_pAvatar->HasSpear(false);
}

bool Player::CanThrowSubWeapon() const
{
	return m_AmountOfSubWeapon > 0;
}

void Player::Die()
{
	m_Health = 0;
}

void Player::HandleInvincibility(float elapsedSec)
{
	if (m_IsInvincible)
	{
		m_CurrentTimeInvincible += elapsedSec;
		if (m_CurrentTimeInvincible >= m_TimeInvincible)
		{
			m_IsInvincible = false;
			m_CurrentTimeInvincible = 0.f;
			m_pSoundInvincibilityOff->Play(0);
		}
	}
}

void Player::HandleSubWeapon(float elapsedSec, const Level& level, const Rectf& camera)
{
	if (m_pCurrentWeapon)
	{
		m_pCurrentWeapon->Update(elapsedSec, level, camera);
		Boomerang* pBoomerang{dynamic_cast<Boomerang*>(m_pCurrentWeapon)};
		if (pBoomerang)
		{
			pBoomerang->HasHitPlayer(GetHitBoxAvatar());
		}
		if (m_pCurrentWeapon->HasBeenHit() || m_pCurrentWeapon->IsOutOfBound())
		{
			m_pAvatar->SetStateSubWeapon(Avatar::ThrowingState::canThrow);
			m_pCurrentWeapon->Reset();
			m_pCurrentWeapon->SetIsThrown(false);
			--m_AmountOfSubWeapon;
			SetValueUpdated(true);
		}
	}
}

void Player::PrepareSubWeapon() const
{
	m_pCurrentWeapon->SetPosition(m_pAvatar->GetCenterHitBox());
	m_pCurrentWeapon->SetDirection(m_pAvatar->GetDirection());
	Boomerang* pBoomerang{dynamic_cast<Boomerang*>(m_pCurrentWeapon)};
	if (pBoomerang)
	{
		pBoomerang->SetStartPosition(m_pAvatar->GetCenterHitBox());
	}
}

void Player::IsAvatarDead()
{
	if (m_pAvatar->IsDead())
	{
		m_Health = 0;
	}
}
