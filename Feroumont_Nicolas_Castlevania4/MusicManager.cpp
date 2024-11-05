#include "pch.h"
#include "MusicManager.h"

#include <iostream>

#include "SoundStream.h"

MusicManager::MusicManager() : m_CurrentMusicType{MusicType::opening}, m_pOpeningMusic{nullptr}, m_pFirstBlockMusic{nullptr},
                               m_pSecondBlockMusic{nullptr}, m_pDeadMusic{nullptr}, m_pGameOverMusic{nullptr}
{
	LoadMusics();
	PlayOpeningMusic();
}

MusicManager::~MusicManager()
{
	delete m_pDeadMusic;
	delete m_pFirstBlockMusic;
	delete m_pGameOverMusic;
	delete m_pSecondBlockMusic;
	delete m_pOpeningMusic;
	m_pDeadMusic = nullptr;
	m_pFirstBlockMusic = nullptr;
	m_pGameOverMusic = nullptr;
	m_pSecondBlockMusic = nullptr;
	m_pOpeningMusic = nullptr;
}

void MusicManager::PlayMusicLevel(const unsigned currentBlock)
{
	if (currentBlock == 1)
	{
		SoundStream::Stop();
		PlayFirstBlockMusic();
		m_CurrentMusicType = MusicType::first;
	}
	else if (currentBlock == 2)
	{
		SoundStream::Stop();
		PlaySecondBlockMusic();
		m_CurrentMusicType = MusicType::second;
	}
	else
	{
		if (!SoundStream::IsPlaying())
		{
			SoundStream::Stop();
			PlaySecondBlockMusic();
			m_CurrentMusicType = MusicType::second;
		}
	}
}

bool MusicManager::PlayDeadMusic()
{
	SoundStream::Stop();
	m_CurrentMusicType = MusicType::dead;
	return m_pDeadMusic->Play(false);
}

bool MusicManager::PlayGameOverMusic()
{
	SoundStream::Stop();
	m_CurrentMusicType = MusicType::gameOver;
	return m_pGameOverMusic->Play(false);
}

bool MusicManager::IsPlaying()
{
	return SoundStream::IsPlaying();
}

MusicManager::MusicType MusicManager::GetCurrentMusic() const
{
	return m_CurrentMusicType;
}

bool MusicManager::PlayFirstBlockMusic() const
{
	return m_pFirstBlockMusic->Play(true);
}

bool MusicManager::PlaySecondBlockMusic() const
{
	return m_pSecondBlockMusic->Play(true);
}

void MusicManager::LoadMusics()
{
	const std::string folderPath{"Sounds/Musics/"};
	std::string file{"Prologue.mp3"};
	m_pFirstBlockMusic = new SoundStream{folderPath + file};
	if (!m_pFirstBlockMusic->IsLoaded())
	{
		std::cout << "Can't load the music file at path: " << folderPath + file << "\n";
		exit(0);
	}
	file = "Level.mp3";
	m_pSecondBlockMusic = new SoundStream{folderPath + file};
	if (!m_pSecondBlockMusic->IsLoaded())
	{
		std::cout << "Can't load the music file at path: " << folderPath + file << "\n";
		exit(0);
	}
	file = "GameOver.mp3";
	m_pGameOverMusic = new SoundStream{folderPath + file};
	if (!m_pGameOverMusic->IsLoaded())
	{
		std::cout << "Can't load the music file at path: " << folderPath + file << "\n";
		exit(0);
	}
	file = "Dead.mp3";
	m_pDeadMusic = new SoundStream{folderPath + file};
	if (!m_pDeadMusic->IsLoaded())
	{
		std::cout << "Can't load the music file at path: " << folderPath + file << "\n";
		exit(0);
	}
	file = "Opening.mp3";
	m_pOpeningMusic = new SoundStream{ folderPath + file };
	if (!m_pDeadMusic->IsLoaded())
	{
		std::cout << "Can't load the music file at path: " << folderPath + file << "\n";
		exit(0);
	}
}

bool MusicManager::PlayOpeningMusic() const
{
	return m_pOpeningMusic->Play(true);
}
