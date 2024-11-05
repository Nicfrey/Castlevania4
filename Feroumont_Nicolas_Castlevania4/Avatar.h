#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Vector2f.h"

class SoundManagerAvatar;
class SoundEffect;
class Sprite;

namespace utils
{
	enum class Direction;
}

class Level;
class Texture;

class Avatar : public GameObject
{
public:
	enum class ActionState
	{
		waiting,
		moving,
		dying,
		hurting,
		dead
	};

	enum class BehaveState
	{
		standing,
		crouching,
		climbingStairs,
		hook
	};

	enum class ThrowingState
	{
		canThrow,
		isThrowing,
		cannotThrow
	};

	struct HookProperties
	{
		HookProperties();
		float angleLeft;
		float angleRight;
		float currentAngle;
		utils::Direction startDirection;
		utils::Direction currentDirection;
		float length;
		Point2f currentHook;
	};

	explicit Avatar();
	explicit Avatar(const Point2f& pos);
	~Avatar() override;
	utils::Direction GetDirection() const;
	bool IsDead() const;
	void ResetActionState();
	Avatar(const Avatar& other) = delete;
	Avatar(Avatar&& other) noexcept = delete;
	Avatar& operator=(const Avatar& rhs) = delete;
	Avatar& operator=(Avatar&& rhs) noexcept = delete;
	void Update(float elapsedSec, const Level& level, bool isDead, bool canThrow);
	void Draw() const;
	Rectf GetHitBoxBody() const;
	void HasSpear(bool hasSpear);
	void HitByEnemy(int& health, const unsigned& amountRemoved);
	ActionState GetActionState() const;
	Rectf GetShapeWeapon() const;
	bool CannotThrowSubWeapon() const;
	void SetStateSubWeapon(const ThrowingState& newState);
	void HasSubWeapon(bool hasSubWeapon);
	Point2f GetCenterHitBox() const;
	void ResetVelocity();

private:
	Sprite* m_pSpriteMovement;
	Sprite* m_pSpriteDeath;
	Sprite* m_pSpriteHorizontalRope;
	Sprite* m_pSpriteHorizontalSpear;
	Sprite* m_pSpriteSubWeapon;
	Sprite* m_pCurrentSprite;

	SoundManagerAvatar* m_pSoundManager;

	Rectf m_HitBoxBody;
	Rectf m_ShapeWeapon;
	Vector2f m_Velocity;
	Vector2f m_Acceleration;

	float m_SpeedMoving{80.f};
	float m_JumpHeight{350.f};
	float m_HurtHeight{150.f};
	bool m_HasSpear{};
	ActionState m_ActionState;
	BehaveState m_BehaveState;
	ThrowingState m_ThrowingState;
	utils::Direction m_DirectionMoving;
	utils::Direction m_DirectionAttack;

	bool m_IsJumping{};
	bool m_IsAttacking{};
	bool m_IsClimbingStairs{};
	float m_TimeBeforeHurting{2.f};
	float m_CurrentTimeHurt{};
	bool m_CanBeHurt{true};
	float m_HeightHitBox;
	bool m_HasSubWeapon;
	HookProperties m_HookProperties;

	void HandleStairState(const Level& level);
	void LoadTextures();
	void SetCurrentTexture(float elapsedSec);
	void HandleInput(const Level& level, bool canThrow);
	void HandleHurting(bool isDead);
	void HandleTimeBeforeHurt(float elapsedSec);
	void GetTextureForAttack();
	void HandleAttack();
	void HandleThrowing();
	void Move(const Level& level);
	void Jump();
	void Attack();
	void ThrowWeapon();
	void UpdatePosition(float elapsedSec);
	Rectf GetShapeWithPos() const;
	Rectf GetHitBoxBodyWithPos() const;
	void SetDirectionOnStair(const Level& level);
	void GetTextureClimbingStairs() const;
	void IsOutOfBounds(const Level& level);
	void UpdateWhenDead(float elapsedSec, const Level& level, bool isDead);
	void UpdateHook(float elapsedSec);
	void StartHook(const Level& level);
	void HandleInputHook();
	void DrawAttack() const;
	void DrawBasic() const;
};
