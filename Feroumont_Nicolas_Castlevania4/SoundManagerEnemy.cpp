#include "pch.h"
#include "SoundManagerEnemy.h"

#include "SoundEffect.h"

SoundManagerEnemy::SoundManagerEnemy()
{
	const std::string folderPath{"Sounds/SoundEffect/"};
	std::string filePath{"SkeletonDeath.mp3"};
	m_pSoundDeathSkeleton = new SoundEffect{folderPath + filePath};
	filePath = "BatDeath.mp3";
	m_pSoundDeathBat = new SoundEffect{folderPath + filePath};
	filePath = "TurretDeath.mp3";
	m_pSoundDeathTurret = new SoundEffect{folderPath + filePath};
	filePath = "MedusaDeath.mp3";
	m_pSoundDeathMedusa = new SoundEffect{folderPath + filePath};
	filePath = "WormDeath.mp3";
	m_pSoundDeathWorm = new SoundEffect{folderPath + filePath};
	filePath = "UnicornDeath.mp3";
	m_pSoundDeathUnicorn = new SoundEffect{folderPath + filePath};
	filePath = "SpiritDeath.mp3";
	m_pSoundDeathSpirit = new SoundEffect{folderPath + filePath};
	filePath = "Hit.mp3";
	m_pSoundHit = new SoundEffect{folderPath + filePath};
}

SoundManagerEnemy::~SoundManagerEnemy()
{
	delete m_pSoundDeathSkeleton;
	delete m_pSoundDeathBat;
	delete m_pSoundDeathTurret;
	delete m_pSoundDeathMedusa;
	delete m_pSoundDeathWorm;
	delete m_pSoundDeathUnicorn;
	delete m_pSoundDeathSpirit;
	delete m_pSoundHit;
	m_pSoundDeathSkeleton = nullptr;
	m_pSoundDeathBat = nullptr;
	m_pSoundDeathTurret = nullptr;
	m_pSoundDeathMedusa = nullptr;
	m_pSoundDeathWorm = nullptr;
	m_pSoundDeathUnicorn = nullptr;
	m_pSoundDeathSpirit = nullptr;
	m_pSoundHit = nullptr;
}

void SoundManagerEnemy::PlaySkeleton() const
{
	m_pSoundDeathSkeleton->Play(0);
}

void SoundManagerEnemy::PlayBat() const
{
	m_pSoundDeathBat->Play(0);
}

void SoundManagerEnemy::PlayTurret() const
{
	m_pSoundDeathTurret->Play(0);
}

void SoundManagerEnemy::PlayMedusa() const
{
	m_pSoundDeathMedusa->Play(0);
}

void SoundManagerEnemy::PlayWorm() const
{
	m_pSoundDeathWorm->Play(0);
}

void SoundManagerEnemy::PlayUnicorn() const
{
	m_pSoundDeathUnicorn->Play(0);
}

void SoundManagerEnemy::PlaySpirit() const
{
	m_pSoundDeathSpirit->Play(0);
}

void SoundManagerEnemy::PlayHit() const
{
	m_pSoundHit->Play(0);
}
