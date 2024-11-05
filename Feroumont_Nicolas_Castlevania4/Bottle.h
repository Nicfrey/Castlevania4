#pragma once
#include "Vector2f.h"
#include "Weapon.h"

class Bottle final : public Weapon
{
public:
	enum class BottleState
	{
		bottle,
		fire,
		end
	};

	Bottle(const Point2f& pos, bool isThrown = false);
	Bottle(const Bottle& bottle);
	Bottle(Bottle&& bottle) noexcept;
	Bottle& operator=(const Bottle& rhs);
	Bottle& operator=(Bottle&& rhs) noexcept;
	void Update(float elapsedSec, const Level& level, const Rectf& camera) override;
	void Draw() const override;
	~Bottle() override;
	void Reset() override;
	Rectf GetShape() const override;

private:
	static float m_TimeFire;
	Vector2f m_Velocity;
	float m_Speed;
	float m_RotationTime;
	BottleState m_CurrentState;
	Sprite* m_pSpriteFire;
	float m_CurrentTimeFire;


	void HandlePosition(float elapsedSec);
	void HandleBottleState(float elapsedSec, const Level& level);
	void HandleSprite(float elapsedSec) const;
};
