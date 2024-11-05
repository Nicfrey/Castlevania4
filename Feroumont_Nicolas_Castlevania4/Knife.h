#pragma once
#include "Vector2f.h"
#include "Weapon.h"

class Knife final : public Weapon
{
public:
	Knife(const Point2f& pos, bool isThrown = false);
	Knife(const Knife& knife);
	Knife(Knife&& knife) noexcept;
	Knife& operator=(const Knife& rhs);
	Knife& operator=(Knife&& rhs) noexcept;
	void Update(float elapsedSec, const Level& level, const Rectf& camera) override;
	void Draw() const override;
	~Knife() override = default;
	void Reset() override;

private:
	void HandlePosition(float elapsedSec);
	float m_Speed;
	Vector2f m_Velocity;
};
