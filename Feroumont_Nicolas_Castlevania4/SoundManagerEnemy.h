#pragma once
class SoundEffect;

class SoundManagerEnemy final
{
public:
	explicit SoundManagerEnemy();
	~SoundManagerEnemy();
	SoundManagerEnemy(const SoundManagerEnemy& other) = delete;
	SoundManagerEnemy(SoundManagerEnemy&& other) noexcept = delete;
	SoundManagerEnemy& operator=(const SoundManagerEnemy& rhs) = delete;
	SoundManagerEnemy& operator=(SoundManagerEnemy&& rhs) noexcept = delete;
	void PlaySkeleton() const;
	void PlayBat() const;
	void PlayTurret() const;
	void PlayMedusa() const;
	void PlayWorm() const;
	void PlayUnicorn() const;
	void PlaySpirit() const;
	void PlayHit() const;

private:
	SoundEffect* m_pSoundDeathSkeleton;
	SoundEffect* m_pSoundDeathBat;
	SoundEffect* m_pSoundDeathTurret;
	SoundEffect* m_pSoundDeathMedusa;
	SoundEffect* m_pSoundDeathWorm;
	SoundEffect* m_pSoundDeathUnicorn;
	SoundEffect* m_pSoundDeathSpirit;
	SoundEffect* m_pSoundHit;
};
