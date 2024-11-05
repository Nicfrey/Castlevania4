#pragma once
#include "Avatar.h"
class SoundEffect;

class SoundManagerAvatar final
{
public:
	explicit SoundManagerAvatar();
	~SoundManagerAvatar();
	SoundManagerAvatar(const SoundManagerAvatar& other) = delete;
	SoundManagerAvatar(SoundManagerAvatar&& other) noexcept = delete;
	SoundManagerAvatar& operator=(const SoundManagerAvatar& rhs) = delete;
	SoundManagerAvatar& operator=(SoundManagerAvatar&& rhs) noexcept = delete;
	void PlaySubWeapon() const;
	void PlayAttack(bool hasSpear) const;
	void PlayDamageTaken(const int health) const;

private:
	SoundEffect* m_pSoundHurt;
	SoundEffect* m_pSoundDead;
	SoundEffect* m_pSoundRope;
	SoundEffect* m_pSoundSpear;
	SoundEffect* m_pSoundSubWeapon;
	void PlayRope() const;
	void PlaySpear() const;
	void PlayHurt() const;
	void PlayDead() const;

	void LoadSounds();
};
