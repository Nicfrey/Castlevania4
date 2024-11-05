#pragma once
class SoundStream;

class MusicManager final
{
public:
	enum class MusicType
	{
		opening,
		first,
		second,
		gameOver,
		dead
	};

	explicit MusicManager();
	~MusicManager();
	MusicManager(const MusicManager& other) = delete;
	MusicManager(MusicManager&& other) noexcept = delete;
	MusicManager& operator=(const MusicManager& rhs) = delete;
	MusicManager& operator=(MusicManager&& rhs) noexcept = delete;
	void PlayMusicLevel(const unsigned currentBlock);
	bool PlayDeadMusic();
	bool PlayGameOverMusic();
	static bool IsPlaying();
	MusicType GetCurrentMusic() const;

private:
	MusicType m_CurrentMusicType;
	SoundStream* m_pOpeningMusic;
	SoundStream* m_pFirstBlockMusic;
	SoundStream* m_pSecondBlockMusic;
	SoundStream* m_pDeadMusic;
	SoundStream* m_pGameOverMusic;
	bool PlayFirstBlockMusic() const;
	bool PlaySecondBlockMusic() const;
	void LoadMusics();
	bool PlayOpeningMusic() const;
};
