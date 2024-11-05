#pragma once
#include "HUD.h"

class SoundEffect;
class Level;
class Weapon;
class Avatar;

class Player final
{
public:
	explicit Player(const unsigned& levelBlock = 1, const unsigned& previousBlock = 0);
	~Player();
	Player(const Player& player) = delete;
	Player(Player&& player) noexcept = delete;
	Player& operator=(const Player& rhs) = delete;
	Player& operator=(Player&& rhs) noexcept = delete;
	void Draw() const;
	void Update(float elapsedSec, const Level& level, const Rectf& camera);
	Rectf GetHitBoxAvatar() const;
	unsigned GetScore() const;
	void AddScore(unsigned scoreAdded);
	void AddAmountOfSubWeapon(unsigned amountAdded);
	void AddHealth(unsigned amountAdded);
	void RemoveHealth(unsigned amountRemoved);
	void SetCurrentWeapon(Weapon weapon);
	float GetDamageMainWeapon() const;
	void HasSpear();
	void ChangeBlock(const unsigned& currentBlock, const unsigned& previousBlock) const;
	Rectf GetShapeMainWeapon() const;
	Rectf GetShapeSubWeapon() const;
	bool HasSubWeapon() const;
	bool IsThrownSubWeapon() const;
	void SetInvincibility(bool isInvincible);
	bool IsInvincible() const;
	float GetDamageSubWeapon() const;
	void SubWeaponHit() const;
	bool IsValueUpdated() const;
	void SetValueUpdated(const bool updated);
	int GetHealth() const;
	unsigned GetAmountOfSubWeapon() const;
	unsigned GetRowFrameWeapon() const;
	unsigned GetStartColumnWeapon() const;
	unsigned GetNrLife() const;
	bool IsDead() const;
	void ResetAfterDead();
	bool IsGameOver() const;
	void ResetAfterGameOver();
	bool CanThrowSubWeapon() const;
	void Die();

private:
	/**
	 * \brief The avatar of the player being shown on screen
	 */
	Avatar* m_pAvatar;
	/**
	 * \brief The current score of the player
	 */
	unsigned m_Score;
	/**
	 * \brief The current amount of sub weapon the player can shoot
	 */
	unsigned m_AmountOfSubWeapon;

	int m_Health;
	bool m_HasSpear;
	unsigned m_NrLife;
	const static unsigned m_StartNrLife;
	const static unsigned m_MaxHealth;
	const static unsigned m_StartAmountWeapon;
	/**
	 * \brief The current weapon of the player. Can be empty
	 */
	Weapon* m_pCurrentWeapon;
	SoundEffect* m_pSoundInvincibilityOff;

	bool m_IsInvincible;
	float m_CurrentTimeInvincible;
	bool m_ValueUpdated;
	const float m_TimeInvincible{5.f};
	const float m_DamageSpear{34.f};
	const float m_DamageRope{17.f};
	void HandleInvincibility(float elapsedSec);
	void HandleSubWeapon(float elapsedSec, const Level& level, const Rectf& camera);
	void PrepareSubWeapon() const;
	void IsAvatarDead();
};
