#include "pch.h"
#include <iostream>

#include "Avatar.h"
#include "Level.h"
#include "SoundManagerAvatar.h"
#include "Sprite.h"
#include "Texture.h"
#include "utils.h"


Avatar::HookProperties::HookProperties() : angleLeft{}, angleRight{}, currentAngle{}, startDirection{},
                                           currentDirection{}, length{}
{
}

Avatar::Avatar() : GameObject{1}, m_pSpriteMovement{nullptr}, m_pSpriteDeath{nullptr},
                   m_pSpriteHorizontalRope{nullptr}, m_pSpriteHorizontalSpear{nullptr},
                   m_pSpriteSubWeapon{nullptr}, m_pCurrentSprite{nullptr},
                   m_pSoundManager{new SoundManagerAvatar{}},
                   m_Velocity{0.f, 0.f},
                   m_Acceleration{0.f, -981.f}, m_HasSpear{false},
                   m_ActionState{ActionState::waiting}, m_BehaveState{BehaveState::standing},
                   m_ThrowingState{ThrowingState::canThrow},
                   m_DirectionMoving{utils::Direction::right}, m_DirectionAttack{utils::Direction::right},
                   m_HasSubWeapon{false}
{
	LoadTextures();
	m_pCurrentSprite = m_pSpriteMovement;
	m_HitBoxBody = m_pCurrentSprite->GetShape();
	m_HeightHitBox = m_HitBoxBody.height;
}

Avatar::Avatar(const Point2f& pos) : GameObject{1, pos}, m_pSpriteMovement{nullptr}, m_pSpriteDeath{nullptr},
                                     m_pSpriteHorizontalRope{nullptr}, m_pSpriteHorizontalSpear{nullptr},
                                     m_pSpriteSubWeapon{nullptr}, m_pCurrentSprite{nullptr},
                                     m_pSoundManager{new SoundManagerAvatar{}},
                                     m_Velocity{0.f, 0.f},
                                     m_Acceleration{0.f, -981.f}, m_HasSpear{false},
                                     m_ActionState{ActionState::waiting}, m_BehaveState{BehaveState::standing},
                                     m_ThrowingState{ThrowingState::canThrow},
                                     m_DirectionMoving{utils::Direction::right},
                                     m_DirectionAttack{utils::Direction::right}, m_HasSubWeapon{false}
{
	LoadTextures();
	m_pCurrentSprite = m_pSpriteMovement;
	m_HitBoxBody = m_pCurrentSprite->GetShape();
	m_HeightHitBox = m_HitBoxBody.height;
}

Avatar::~Avatar()
{
	delete m_pSpriteMovement;
	m_pSpriteMovement = nullptr;

	delete m_pSpriteDeath;
	m_pSpriteDeath = nullptr;

	delete m_pSpriteHorizontalSpear;
	m_pSpriteHorizontalSpear = nullptr;

	delete m_pSpriteHorizontalRope;
	m_pSpriteHorizontalRope = nullptr;

	delete m_pSpriteSubWeapon;
	m_pSpriteSubWeapon = nullptr;

	m_pCurrentSprite = nullptr;

	delete m_pSoundManager;
	m_pSoundManager = nullptr;
}

utils::Direction Avatar::GetDirection() const
{
	return m_DirectionMoving;
}

bool Avatar::IsDead() const
{
	return m_ActionState == ActionState::dying || m_ActionState == ActionState::dead;
}

void Avatar::ResetActionState()
{
	m_ActionState = ActionState::waiting;
}

void Avatar::Update(float elapsedSec, const Level& level, bool isDead, bool canThrow)
{
	m_HitBoxBody.height = m_HeightHitBox;
	m_IsJumping = !level.IsOnGround(GetHitBoxBody(), m_IsClimbingStairs) && !m_IsClimbingStairs;
	HandleStairState(level);
	if (level.HitHooks(GetShapeWeapon()).x != -1.f)
	{
		StartHook(level);
	}
	if (m_IsAttacking)
	{
		HandleAttack();
	}
	if (m_ThrowingState == ThrowingState::isThrowing)
	{
		HandleThrowing();
	}
	if (isDead)
	{
		UpdateWhenDead(elapsedSec, level, isDead);
	}
	else
	{
		if (m_ActionState != ActionState::hurting && !m_IsAttacking && m_BehaveState != BehaveState::hook &&
			m_ThrowingState != ThrowingState::isThrowing)
		{
			HandleInput(level, canThrow);
		}
		if (m_ActionState == ActionState::hurting)
		{
			HandleHurting(isDead);
		}
		if (m_BehaveState == BehaveState::crouching)
		{
			m_HitBoxBody.height = m_HitBoxBody.height - 14.f;
		}
		if (m_BehaveState == BehaveState::hook)
		{
			UpdateHook(elapsedSec);
		}
		HandleTimeBeforeHurt(elapsedSec);
		UpdatePosition(elapsedSec);
		SetCurrentTexture(elapsedSec);
		if (m_IsClimbingStairs)
		{
			if (m_ActionState != ActionState::hurting)
			{
				level.HandleCollisionActorOnStairs(m_Position, m_HitBoxBody, m_Velocity);
			}
		}
		else
		{
			level.HandleCollisionActorsOnGround(m_Position, m_HitBoxBody, m_Velocity,
			                                    m_BehaveState == BehaveState::hook);
		}
		IsOutOfBounds(level);
	}
}

void Avatar::Draw() const
{
	if (m_IsAttacking)
	{
		DrawAttack();
	}
	if (!m_IsAttacking)
	{
		DrawBasic();
	}
}

Rectf Avatar::GetHitBoxBody() const
{
	return GetHitBoxBodyWithPos();
}

void Avatar::HasSpear(bool hasSpear)
{
	m_HasSpear = hasSpear;
}

void Avatar::HitByEnemy(int& health, const unsigned& amountRemoved)
{
	if (!m_CanBeHurt)
	{
		return;
	}
	m_ActionState = ActionState::hurting;
	if (m_IsClimbingStairs)
	{
		m_IsClimbingStairs = false;
	}
	m_CanBeHurt = false;
	health -= amountRemoved;
	m_pSoundManager->PlayDamageTaken(health);
	switch (m_DirectionMoving)
	{
	case utils::Direction::left:
	case utils::Direction::upLeft:
	case utils::Direction::downLeft:
		m_Velocity.x = m_SpeedMoving;
		m_Velocity.y = m_HurtHeight;
		break;
	case utils::Direction::upRight:
	case utils::Direction::right:
	case utils::Direction::downRight:
		m_Velocity.x = -m_SpeedMoving;
		m_Velocity.y = m_HurtHeight;
		break;
	}
}

Avatar::ActionState Avatar::GetActionState() const
{
	return m_ActionState;
}

Rectf Avatar::GetShapeWeapon() const
{
	if (m_IsAttacking && m_pCurrentSprite->GetAnimFrame() == 2)
	{
		const Point2f centerOfAvatar{utils::GetCenterOfRectangle(GetHitBoxBody())};
		const float lengthWeapon{57.f};
		const float heightDisplayWeapon{5.f};
		const float heightOfWeapon{5.f};
		if (m_DirectionAttack == utils::Direction::left)
		{
			if (m_BehaveState == BehaveState::crouching)
			{
				return Rectf{
					(centerOfAvatar.x - m_HitBoxBody.width / 2) - lengthWeapon, centerOfAvatar.y, lengthWeapon,
					heightOfWeapon
				};
			}
			return Rectf{
				(centerOfAvatar.x - m_HitBoxBody.width / 2) - lengthWeapon, centerOfAvatar.y + heightDisplayWeapon,
				lengthWeapon, heightOfWeapon
			};
		}
		if (m_BehaveState == BehaveState::crouching)
		{
			return Rectf{centerOfAvatar.x + m_HitBoxBody.width / 2, centerOfAvatar.y, lengthWeapon, heightOfWeapon};
		}
		return Rectf{
			centerOfAvatar.x + m_HitBoxBody.width / 2, centerOfAvatar.y + heightDisplayWeapon, lengthWeapon,
			heightOfWeapon
		};
	}
	return Rectf{-200.f, -200.f, 0.f, 0.f};
}

bool Avatar::CannotThrowSubWeapon() const
{
	return m_ThrowingState == ThrowingState::cannotThrow;
}

void Avatar::SetStateSubWeapon(const ThrowingState& newState)
{
	m_ThrowingState = newState;
}

void Avatar::HasSubWeapon(bool hasSubWeapon)
{
	m_HasSubWeapon = hasSubWeapon;
}

Point2f Avatar::GetCenterHitBox() const
{
	return utils::GetCenterOfRectangle(GetHitBoxBody());
}

void Avatar::ResetVelocity()
{
	m_Velocity.Set(0, 0);
}

void Avatar::HandleStairState(const Level& level)
{
	if (m_IsClimbingStairs)
	{
		// IsOnGroundWhenOnStairs
		const bool isOnGround{level.IsOnGround(GetHitBoxBody(), m_IsClimbingStairs)};
		if (isOnGround)
		{
			m_IsClimbingStairs = false;
		}
	}
}

void Avatar::LoadTextures()
{
	const std::string spriteFolder{"Sprites/"};
	const unsigned framePerSecMovement{6};
	const unsigned framePerSecAttack{7};

	// Load Basic movement
	const std::string filePathBasicMovement{spriteFolder + "SimonBelmontBasicMoves.png"};
	m_pSpriteMovement = new Sprite{filePathBasicMovement, 6, 5, 0, 6, framePerSecMovement};

	// Load Death Animation
	const std::string filePathDeath{spriteFolder + "SimonBelmontDeath.png"};
	m_pSpriteDeath = new Sprite{filePathDeath, 3, 1, 0, 3, framePerSecMovement};

	// Load Horizontal Rope
	const std::string filePathHorizontalRope{spriteFolder + "SimonBelmontAttackHorizontalRope.png"};
	m_pSpriteHorizontalRope = new Sprite{filePathHorizontalRope, 3, 5, 0, 3, framePerSecAttack};

	// Load Horizontal Spear
	const std::string filePathHorizontalSpear{spriteFolder + "SimonBelmontAttackHorizontalSpear.png"};
	m_pSpriteHorizontalSpear = new Sprite{filePathHorizontalSpear, 3, 5, 0, 3, framePerSecAttack};

	// Load thrown texture
	const std::string filePathWeaponThrown{spriteFolder + "SimonBelmontThrowing.png"};
	m_pSpriteSubWeapon = new Sprite{filePathWeaponThrown, 3, 1, 0, 3, framePerSecAttack};
}

void Avatar::SetCurrentTexture(float elapsedSec)
{
	if (m_IsJumping && !m_IsAttacking && m_ThrowingState != ThrowingState::isThrowing && !m_IsClimbingStairs)
	{
		m_pCurrentSprite = m_pSpriteMovement;

		if (m_ActionState == ActionState::hurting)
		{
			m_pCurrentSprite->SetRowFrame(1);
			m_pCurrentSprite->SetAnimFrame(3);
		}
		else
		{
			m_pCurrentSprite->SetRowFrame(1);
			m_pCurrentSprite->SetAnimFrame(1);
		}
	}
	else if (m_IsAttacking)
	{
		GetTextureForAttack();
		m_pCurrentSprite->HandleFrames(elapsedSec);
	}
	else if ((m_IsJumping && m_ThrowingState == ThrowingState::isThrowing) || m_ThrowingState ==
		ThrowingState::isThrowing)
	{
		m_pCurrentSprite = m_pSpriteSubWeapon;
		if (m_pCurrentSprite->HasDoneLoop())
		{
			m_pCurrentSprite->SetRowFrame(1);
			m_pCurrentSprite->SetAnimFrame(0);
		}
		m_pCurrentSprite->HandleFrames(elapsedSec);
	}
	else if (!m_IsAttacking && !m_IsJumping && !m_IsClimbingStairs)
	{
		m_pCurrentSprite = m_pSpriteMovement;
		switch (m_ActionState)
		{
		case ActionState::waiting:
			switch (m_BehaveState)
			{
			case BehaveState::standing:
				m_pCurrentSprite->SetRowFrame(1);
				m_pCurrentSprite->SetAnimFrame(0);
				break;
			case BehaveState::crouching:
				m_pCurrentSprite->SetRowFrame(1);
				m_pCurrentSprite->SetAnimFrame(2);
				break;
			}
			break;
		case ActionState::moving:
			switch (m_BehaveState)
			{
			case BehaveState::standing:
				m_pCurrentSprite->SetRowFrame(2);
				m_pCurrentSprite->HandleFrames(elapsedSec);
				break;
			case BehaveState::crouching:
				m_pCurrentSprite->SetRowFrame(5);
				m_pCurrentSprite->HandleFrames(elapsedSec);
				break;
			}
			break;
		case ActionState::dying:
		case ActionState::dead:
			m_pCurrentSprite = m_pSpriteDeath;
			m_pCurrentSprite->HandleFrames(elapsedSec);
			break;
		case ActionState::hurting:
			m_pCurrentSprite->SetRowFrame(1);
			m_pCurrentSprite->SetAnimFrame(3);
			break;
		}
	}
	else if (m_IsClimbingStairs)
	{
		m_pCurrentSprite = m_pSpriteMovement;
		switch (m_ActionState)
		{
		case ActionState::waiting:
			GetTextureClimbingStairs();
			m_pCurrentSprite->SetAnimFrame(0);
			break;
		case ActionState::moving:
			GetTextureClimbingStairs();
			m_pCurrentSprite->HandleFrames(elapsedSec);
			break;
		case ActionState::hurting:
			m_pCurrentSprite->SetRowFrame(1);
			m_pCurrentSprite->SetAnimFrame(3);
			break;
		default: ;
		}
	}
}

void Avatar::HandleInput(const Level& level, bool canThrow)
{
	m_Velocity.x = 0;
	if (!m_IsJumping)
	{
		// Jump
		Jump();
		// Move
		Move(level);
		// Attack
		Attack();
		// Throw sub weapon
		if (canThrow)
		{
			ThrowWeapon();
		}
	}
	else
	{
		// Move
		Move(level);
		// Attack
		Attack();
		// Throw sub weapon
		if (canThrow)
		{
			ThrowWeapon();
		}
	}
}

void Avatar::HandleHurting(bool isDead)
{
	if ((!m_IsJumping && m_Velocity.y == 0.f) || m_BehaveState == BehaveState::hook)
	{
		if (isDead)
		{
			m_ActionState = ActionState::dying;
			m_pCurrentSprite->SetAnimFrame(0);
		}
		else
		{
			m_ActionState = ActionState::waiting;
		}
	}
}

void Avatar::HandleTimeBeforeHurt(float elapsedSec)
{
	if (m_CanBeHurt)
	{
		return;
	}
	if (m_CurrentTimeHurt < m_TimeBeforeHurting)
	{
		m_CurrentTimeHurt += elapsedSec;
	}
	if (m_CurrentTimeHurt >= m_TimeBeforeHurting)
	{
		m_CanBeHurt = true;
		m_CurrentTimeHurt -= m_TimeBeforeHurting;
	}
}

void Avatar::GetTextureForAttack()
{
	switch (m_DirectionAttack)
	{
	case utils::Direction::left:
	case utils::Direction::right:
		if (m_HasSpear)
		{
			m_pCurrentSprite = m_pSpriteHorizontalSpear;
		}
		else
		{
			m_pCurrentSprite = m_pSpriteHorizontalRope;
		}
		if (m_IsJumping)
		{
			m_pCurrentSprite->SetRowFrame(5);
		}
		else
		{
			switch (m_BehaveState)
			{
			case BehaveState::standing:
				m_pCurrentSprite->SetRowFrame(1);
				break;
			case BehaveState::crouching:
				m_pCurrentSprite->SetRowFrame(2);
				break;
			case BehaveState::climbingStairs:
				// Check direction
				break;
			}
		}
	}
}

void Avatar::HandleAttack()
{
	if (!m_IsJumping || m_IsClimbingStairs)
	{
		m_Velocity.x = 0.f;
	}
	if (m_pCurrentSprite->HasDoneLoop())
	{
		m_IsAttacking = false;
	}
}

void Avatar::HandleThrowing()
{
	if (!m_IsJumping)
	{
		m_Velocity.x = 0.f;
	}
	if (m_pCurrentSprite->HasDoneLoop())
	{
		m_ThrowingState = ThrowingState::cannotThrow;
	}
}

void Avatar::Move(const Level& level)
{
	m_ActionState = ActionState::waiting;
	m_BehaveState = BehaveState::standing;
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_LEFT])
	{
		m_DirectionMoving = utils::Direction::left;
		m_DirectionAttack = utils::Direction::left;
		if (pStates[SDL_SCANCODE_DOWN])
		{
			m_ActionState = ActionState::moving;
			if (level.IsOnStairs(GetHitBoxBody(), m_Velocity))
			{
				m_Velocity.x -= m_SpeedMoving;
				m_IsClimbingStairs = true;
				SetDirectionOnStair(level);
			}
			else
			{
				m_Velocity.x -= m_SpeedMoving / 2;
				m_BehaveState = BehaveState::crouching;
			}
		}
		else if (pStates[SDL_SCANCODE_UP])
		{
			m_Velocity.x -= m_SpeedMoving;
			m_ActionState = ActionState::moving;
			if (level.IsOnStairs(GetHitBoxBody(), m_Velocity))
			{
				m_IsClimbingStairs = true;
				SetDirectionOnStair(level);
			}
			else
			{
				m_BehaveState = BehaveState::standing;
			}
		}
		else
		{
			m_Velocity.x -= m_SpeedMoving;
			m_ActionState = ActionState::moving;
			m_DirectionAttack = utils::Direction::left;
			if (m_IsClimbingStairs && level.IsOnStairs(GetHitBoxBody(), m_Velocity))
			{
				SetDirectionOnStair(level);
			}
		}
	}
	if (pStates[SDL_SCANCODE_RIGHT])
	{
		m_DirectionMoving = utils::Direction::right;
		m_DirectionAttack = utils::Direction::right;
		if (pStates[SDL_SCANCODE_DOWN])
		{
			m_BehaveState = BehaveState::crouching;
			m_ActionState = ActionState::moving;
			if (level.IsOnStairs(GetHitBoxBody(), m_Velocity))
			{
				m_Velocity.x += m_SpeedMoving;
				m_IsClimbingStairs = true;
				SetDirectionOnStair(level);
			}
			else
			{
				m_Velocity.x += m_SpeedMoving / 2;
				m_BehaveState = BehaveState::crouching;
			}
		}
		else if (pStates[SDL_SCANCODE_UP])
		{
			m_Velocity.x += m_SpeedMoving;
			m_ActionState = ActionState::moving;
			if (level.IsOnStairs(GetHitBoxBody(), m_Velocity))
			{
				m_IsClimbingStairs = true;
				SetDirectionOnStair(level);
			}
			else
			{
				m_BehaveState = BehaveState::standing;
			}
		}
		else
		{
			m_Velocity.x += m_SpeedMoving;
			m_ActionState = ActionState::moving;
			if (m_IsClimbingStairs && level.IsOnStairs(GetHitBoxBody(), m_Velocity))
			{
				SetDirectionOnStair(level);
			}
		}
	}
	if (pStates[SDL_SCANCODE_DOWN] && !pStates[SDL_SCANCODE_LEFT] && !pStates[SDL_SCANCODE_RIGHT])
	{
		// Check stairs
		m_BehaveState = BehaveState::crouching;
	}
	if (pStates[SDL_SCANCODE_UP] && !pStates[SDL_SCANCODE_LEFT] && !pStates[SDL_SCANCODE_RIGHT])
	{
		// Check stairs
		m_BehaveState = BehaveState::standing;
	}
}

void Avatar::Jump()
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_W])
	{
		if (!m_IsClimbingStairs)
		{
			m_Velocity.y = m_JumpHeight;
		}
	}
}

void Avatar::Attack()
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_X])
	{
		if (!m_IsAttacking && m_ThrowingState != ThrowingState::isThrowing)
		{
			m_IsAttacking = true;
			m_pCurrentSprite->SetAnimFrame(0);
			m_pSoundManager->PlayAttack(m_HasSpear);
		}
	}
}

void Avatar::ThrowWeapon()
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_C])
	{
		if (m_ThrowingState == ThrowingState::canThrow && !m_IsAttacking && m_HasSubWeapon)
		{
			m_ThrowingState = ThrowingState::isThrowing;
			m_pCurrentSprite->SetAnimFrame(0);
		}
	}
}

void Avatar::UpdatePosition(float elapsedSec)
{
	// Gravity
	if (m_BehaveState != BehaveState::hook)
	{
		m_Velocity.y += m_Acceleration.y * elapsedSec;
	}

	// Movement based on velocity
	m_Position.x += m_Velocity.x * elapsedSec;
	m_Position.y += m_Velocity.y * elapsedSec;
}

Rectf Avatar::GetShapeWithPos() const
{
	const Rectf shape{
		m_Position.x, m_Position.y, m_pCurrentSprite->GetShape().width, m_pCurrentSprite->GetShape().height
	};
	return shape;
}

Rectf Avatar::GetHitBoxBodyWithPos() const
{
	const Rectf shape{m_Position.x, m_Position.y, m_HitBoxBody.width, m_HitBoxBody.height};
	return shape;
}

void Avatar::SetDirectionOnStair(const Level& level)
{
	if (level.GetDirectionStairOnActor(GetHitBoxBody(), m_DirectionMoving) == utils::Direction::left)
	{
		switch (m_DirectionMoving)
		{
		case utils::Direction::left:
		case utils::Direction::upLeft:
		case utils::Direction::up:
			m_DirectionMoving = utils::Direction::upLeft;
			break;
		case utils::Direction::right:
		case utils::Direction::downRight:
		case utils::Direction::down:
			m_DirectionMoving = utils::Direction::downRight;
			break;
		}
	}
	else if (level.GetDirectionStairOnActor(GetHitBoxBody(), m_DirectionMoving) == utils::Direction::right)
	{
		switch (m_DirectionMoving)
		{
		case utils::Direction::left:
		case utils::Direction::downLeft:
		case utils::Direction::down:
			m_DirectionMoving = utils::Direction::downLeft;
			break;
		case utils::Direction::right:
		case utils::Direction::upRight:
		case utils::Direction::up:
			m_DirectionMoving = utils::Direction::upRight;
			break;
		}
	}
}

void Avatar::GetTextureClimbingStairs() const
{
	switch (m_DirectionMoving)
	{
	case utils::Direction::downLeft:
	case utils::Direction::downRight:
		m_pCurrentSprite->SetRowFrame(4);
		break;
	case utils::Direction::upLeft:
	case utils::Direction::upRight:
		m_pCurrentSprite->SetRowFrame(3);
		break;
	}
}

void Avatar::IsOutOfBounds(const Level& level)
{
	if (!utils::IsOverlapping(level.GetBoundaries(), GetHitBoxBody()))
	{
		m_pSoundManager->PlayDamageTaken(0);
		m_ActionState = ActionState::dead;
	}
}

void Avatar::UpdateWhenDead(float elapsedSec, const Level& level, bool isDead)
{
	if (m_ActionState != ActionState::dying && m_ActionState != ActionState::dead)
	{
		IsOutOfBounds(level);
	}
	if (m_ActionState == ActionState::hurting)
	{
		HandleHurting(isDead);
	}
	else
	{
		if (m_pCurrentSprite->GetAnimFrame() == 2)
			if (m_pCurrentSprite->GetAnimFrame() == 2)
			{
				m_pCurrentSprite->ResetAnimTime();
				m_ActionState = ActionState::dead;
			}
		m_Velocity.x = 0.f;
	}
	SetCurrentTexture(elapsedSec);
	UpdatePosition(elapsedSec);
	level.HandleCollisionActorsOnGround(m_Position, m_HitBoxBody, m_Velocity, m_BehaveState == BehaveState::hook);
}

void Avatar::UpdateHook(float elapsedSec)
{
	HandleInputHook();
	float newX{}, newY{};
	switch (m_HookProperties.currentDirection)
	{
	case utils::Direction::downLeft:
	case utils::Direction::left:
	case utils::Direction::upLeft:
		m_HookProperties.currentAngle -= elapsedSec;
		newX = m_HookProperties.currentHook.x + m_HookProperties.length * cosf(m_HookProperties.currentAngle);
		newY = m_HookProperties.currentHook.y + m_HookProperties.length * sinf(m_HookProperties.currentAngle);
		SetPosition(newX, newY);
		if (m_HookProperties.currentAngle <= m_HookProperties.angleLeft)
		{
			m_HookProperties.currentDirection = utils::Direction::right;
		}
		break;
	case utils::Direction::right:
	case utils::Direction::upRight:
	case utils::Direction::downRight:
		m_HookProperties.currentAngle += elapsedSec;
		newX = m_HookProperties.currentHook.x + m_HookProperties.length * cosf(m_HookProperties.currentAngle);
		newY = m_HookProperties.currentHook.y + m_HookProperties.length * sinf(m_HookProperties.currentAngle);
		SetPosition(newX, newY);
		if (m_HookProperties.currentAngle >= m_HookProperties.angleRight)
		{
			m_HookProperties.currentDirection = utils::Direction::left;
		}
		break;
	}
}

void Avatar::StartHook(const Level& level)
{
	// Set properties for hook
	m_HookProperties.currentHook = level.HitHooks(GetShapeWeapon());
	switch (m_DirectionAttack)
	{
	case utils::Direction::left:
	case utils::Direction::upLeft:
	case utils::Direction::downLeft:
		m_HookProperties.angleRight = level.GetAngleFromHook(GetCenterHitBox(), m_HookProperties.currentHook);
		m_HookProperties.angleLeft = utils::DegreeToRad(utils::RadToDegree(m_HookProperties.angleLeft) - 180.f);
		break;
	case utils::Direction::upRight:
	case utils::Direction::right:
	case utils::Direction::downRight:
		m_HookProperties.angleLeft = level.GetAngleFromHook(GetCenterHitBox(), m_HookProperties.currentHook);
		m_HookProperties.angleRight = utils::DegreeToRad(utils::RadToDegree(m_HookProperties.angleLeft) + 180.f);
		break;
	}
	m_HookProperties.currentAngle = m_HookProperties.angleLeft;
	m_HookProperties.currentDirection = m_DirectionAttack;
	m_HookProperties.startDirection = m_DirectionAttack;
	const Vector2f vectorActorToHook{m_HookProperties.currentHook, m_Position};
	m_HookProperties.length = vectorActorToHook.Length();

	// Setup for update hooking
	m_BehaveState = BehaveState::hook;
	m_pCurrentSprite->ResetAnimTime();
	m_pCurrentSprite->SetAnimFrame();
	m_IsAttacking = false;
	m_Velocity.Set(0, 0);
}

void Avatar::HandleInputHook()
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_W])
	{
		if (m_BehaveState == BehaveState::hook)
		{
			m_BehaveState = BehaveState::standing;
		}
	}
}

void Avatar::DrawAttack() const
{
	glPushMatrix();
	glTranslatef(GetPosition().x, GetPosition().y, 0);
	if (m_DirectionMoving == utils::Direction::left
		|| m_DirectionMoving == utils::Direction::upLeft
		|| m_DirectionMoving == utils::Direction::downLeft)
	{
		glScalef(-1.f, 1.f, 1.f);
		glTranslatef(-m_HitBoxBody.width, 0.f, 0.f);
	}
	// Set the shape correctly
	if (m_BehaveState == BehaveState::standing)
	{
		glTranslatef(-20.f, -5.f, 0.f);
	}
	if (m_BehaveState == BehaveState::crouching)
	{
		glTranslatef(-25.f, -16.f, 0.f);
	}
	if (m_IsJumping)
	{
		glTranslatef(-5.f, 0.f, 0.f);
	}
	m_pCurrentSprite->Draw();
	glPopMatrix();
}

void Avatar::DrawBasic() const
{
	glPushMatrix();
	glTranslatef(GetPosition().x, GetPosition().y, 0);
	if (m_DirectionMoving == utils::Direction::left)
	{
		glScalef(-1.f, 1.f, 1.f);
		glTranslatef(-m_HitBoxBody.width, 0.f, 0.f);
	}
	m_pCurrentSprite->Draw();
	glPopMatrix();
	if(m_BehaveState == BehaveState::hook)
	{
		utils::DrawLine(m_HookProperties.currentHook, GetCenterHitBox(), 5);
		utils::DrawEllipse(m_HookProperties.currentHook, m_HookProperties.length, m_HookProperties.length, 4);
	}
}
