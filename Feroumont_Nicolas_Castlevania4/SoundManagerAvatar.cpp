#include "pch.h"
#include "SoundManagerAvatar.h"

#include "SoundEffect.h"

SoundManagerAvatar::SoundManagerAvatar() : m_pSoundHurt{ nullptr }, m_pSoundDead{nullptr}, m_pSoundRope{nullptr}, m_pSoundSpear{nullptr}, m_pSoundSubWeapon{nullptr}
{
	LoadSounds();
}

SoundManagerAvatar::~SoundManagerAvatar()
{
	delete m_pSoundDead;
	delete m_pSoundHurt;
	delete m_pSoundRope;
	delete m_pSoundSpear;
	delete m_pSoundSubWeapon;
	m_pSoundDead = nullptr;
	m_pSoundHurt = nullptr;
	m_pSoundRope = nullptr;
	m_pSoundSpear = nullptr;
	m_pSoundSubWeapon = nullptr;
}

void SoundManagerAvatar::PlayHurt() const
{
	m_pSoundHurt->Play(0);
}

void SoundManagerAvatar::PlayDead() const
{
	m_pSoundDead->Play(0);
}

void SoundManagerAvatar::LoadSounds()
{
	const std::string folderPath{ "Sounds/SoundEffect/" };
	std::string filePath{ "Dead.mp3" };
	m_pSoundDead = new SoundEffect{ folderPath + filePath };
	filePath = "Hurt.mp3";
	m_pSoundHurt = new SoundEffect{ folderPath + filePath };
	filePath = "Rope.mp3";
	m_pSoundRope = new SoundEffect{ folderPath + filePath };
	filePath = "Spear.mp3";
	m_pSoundSpear = new SoundEffect{ folderPath + filePath };
	filePath = "SubWeaponThrow.mp3";
	m_pSoundSubWeapon = new SoundEffect{ folderPath + filePath };
}

void SoundManagerAvatar::PlayRope() const
{
	m_pSoundRope->Play(0);
}

void SoundManagerAvatar::PlaySpear() const
{
	m_pSoundSpear->Play(0);
}

void SoundManagerAvatar::PlaySubWeapon() const
{
	m_pSoundSubWeapon->Play(0);
}

void SoundManagerAvatar::PlayAttack(bool hasSpear) const
{
	if (hasSpear)
	{
		PlaySpear();
	}
	else
	{
		PlayRope();
	}
}

void SoundManagerAvatar::PlayDamageTaken(const int health) const
{
	if (health == 0)
	{
		PlayDead();
	}
	else
	{
		PlayHurt();
	}
}
