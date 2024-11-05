#include "pch.h"
#include "SoundManagerItem.h"

#include "SoundEffect.h"

SoundManagerItem::SoundManagerItem()
{
	const std::string folderPath{"Sounds/SoundEffect/"};
	std::string filePath{"BigHeart.mp3"};
	m_pSoundHeartBig = new SoundEffect{folderPath + filePath};
	filePath = "SmallHeart.mp3";
	m_pSoundHeartSmall = new SoundEffect{folderPath + filePath};
	filePath = "InvincibilityOn.mp3";
	m_pSoundInvincibilityOn = new SoundEffect{folderPath + filePath};
	filePath = "SpearGot.mp3";
	m_pSoundSpearGot = new SoundEffect{folderPath + filePath};
	filePath = "Money.mp3";
	m_pSoundMoney = new SoundEffect{folderPath + filePath};
	filePath = "SubWeaponGot.mp3";
	m_pSoundSubWeapon = new SoundEffect{folderPath + filePath};
	filePath = "Food.mp3";
	m_pSoundFood = new SoundEffect{folderPath + filePath};
}

SoundManagerItem::~SoundManagerItem()
{
	delete m_pSoundHeartSmall;
	delete m_pSoundFood;
	delete m_pSoundHeartBig;
	delete m_pSoundInvincibilityOn;
	delete m_pSoundMoney;
	delete m_pSoundSubWeapon;
	delete m_pSoundSpearGot;
	m_pSoundHeartSmall = nullptr;
	m_pSoundFood = nullptr;
	m_pSoundHeartBig = nullptr;
	m_pSoundInvincibilityOn = nullptr;
	m_pSoundMoney = nullptr;
	m_pSoundSubWeapon = nullptr;
	m_pSoundSpearGot = nullptr;
}

void SoundManagerItem::PlayHeartBig() const
{
	m_pSoundHeartBig->Play(0);
}

void SoundManagerItem::PlayHeartSmall() const
{
	m_pSoundHeartSmall->Play(0);
}

void SoundManagerItem::PlayMoney() const
{
	m_pSoundMoney->Play(0);
}

void SoundManagerItem::PlayInvincibilityOn() const
{
	m_pSoundInvincibilityOn->Play(0);
}

void SoundManagerItem::PlaySpear() const
{
	m_pSoundSpearGot->Play(0);
}

void SoundManagerItem::PlaySubWeapon() const
{
	m_pSoundSubWeapon->Play(0);
}

void SoundManagerItem::PlayFood() const
{
	m_pSoundFood->Play(0);
}

void SoundManagerItem::PlayHeart(unsigned amount) const
{
	if (amount == Heart::GetAmountMax())
	{
		PlayHeartBig();
	}
	else
	{
		PlayHeartSmall();
	}
}
