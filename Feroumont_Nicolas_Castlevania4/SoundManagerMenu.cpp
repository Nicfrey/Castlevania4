#include "pch.h"
#include "SoundManagerMenu.h"

#include "SoundEffect.h"

SoundManagerMenu::SoundManagerMenu() : m_pSoundChangeUI{ nullptr }, m_pSoundChoose{ nullptr }
{
	LoadSounds();
}

SoundManagerMenu::~SoundManagerMenu()
{
	delete m_pSoundChangeUI;
	delete m_pSoundChoose;
	m_pSoundChangeUI = nullptr;
	m_pSoundChoose = nullptr;
}

void SoundManagerMenu::PlayChangeUI() const
{
	m_pSoundChangeUI->Play(0);
}

void SoundManagerMenu::PlayChoose() const
{
	m_pSoundChoose->Play(0);
}

void SoundManagerMenu::LoadSounds()
{
	const std::string folderPath{ "Sounds/SoundEffect/" };
	std::string filePath{ "SelectionMenu.mp3" };
	m_pSoundChangeUI = new SoundEffect{ folderPath + filePath };
	filePath = "WolfMainMenu.mp3";
	m_pSoundChoose = new SoundEffect{ folderPath + filePath };
}
