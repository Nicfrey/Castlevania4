#pragma once
#include "Vector2f.h"
#include "Weapon.h"

class Axe final : public Weapon
{
public:
	Axe(const Point2f& pos, bool isThrown = false);
	Axe(const Axe& axe);
	Axe(Axe&& axe) noexcept;
	Axe& operator=(const Axe& rhs);
	Axe& operator=(Axe&& rhs) noexcept;
	void Update(float elapsedSec, const Level& level, const Rectf& camera) override;
	void Draw() const override;
	~Axe() override = default;
	void Reset() override;

private:
	Vector2f m_Velocity;
	float m_Speed;
	float m_MaxHeight;
	float m_RotationTime;

	void HandlePosition(float elapsedSec);
};
