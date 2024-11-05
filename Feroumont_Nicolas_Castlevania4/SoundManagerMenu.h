#pragma once
class SoundEffect;

class SoundManagerMenu final
{
public:
	explicit SoundManagerMenu();
	~SoundManagerMenu();
	SoundManagerMenu(const SoundManagerMenu& other) = delete;
	SoundManagerMenu(SoundManagerMenu&& other) noexcept = delete;
	SoundManagerMenu& operator=(const SoundManagerMenu& rhs) = delete;
	SoundManagerMenu& operator=(SoundManagerMenu&& rhs) noexcept = delete;
	void PlayChangeUI() const;
	void PlayChoose() const;
private:
	SoundEffect* m_pSoundChangeUI;
	SoundEffect* m_pSoundChoose;

	void LoadSounds();
};

