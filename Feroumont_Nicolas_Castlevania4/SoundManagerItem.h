#pragma once
#include "Heart.h"
class SoundEffect;

class SoundManagerItem final
{
public:
	explicit SoundManagerItem();
	~SoundManagerItem();
	SoundManagerItem(const SoundManagerItem& other) = delete;
	SoundManagerItem(SoundManagerItem&& other) noexcept = delete;
	SoundManagerItem& operator=(const SoundManagerItem& rhs) = delete;
	SoundManagerItem& operator=(SoundManagerItem&& rhs) noexcept = delete;
	void PlayMoney() const;
	void PlayInvincibilityOn() const;
	void PlaySpear() const;
	void PlaySubWeapon() const;
	void PlayFood() const;
	void PlayHeart(unsigned amount) const;

private:
	SoundEffect* m_pSoundHeartBig;
	SoundEffect* m_pSoundHeartSmall;
	SoundEffect* m_pSoundMoney;
	SoundEffect* m_pSoundInvincibilityOn;
	SoundEffect* m_pSoundSpearGot;
	SoundEffect* m_pSoundSubWeapon;
	SoundEffect* m_pSoundFood;
	void PlayHeartBig() const;
	void PlayHeartSmall() const;
};
