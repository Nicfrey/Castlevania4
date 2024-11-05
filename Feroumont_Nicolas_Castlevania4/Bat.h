#pragma once
#include "Enemy.h"
#include "Vector2f.h"

class Sprite;
class Texture;

class Bat final : public Enemy
{
public:
	Bat(const Point2f& pos);
	Bat(const Bat& bat);
	Bat(Bat&& bat) noexcept;
	Bat& operator=(const Bat& rhs);
	Bat& operator=(Bat&& rhs) noexcept;
	~Bat() override;
	void Update(float elapsedSec) override;
	void Draw() const override;

private:
	void DrawAlive() const override;
	void DrawDying() const override;
	void HandlePosition(float elapsedSec) override;
	static unsigned m_CounterBat;
	static Texture* m_pTextureBat;
	Vector2f m_Velocity;
	float m_RotationTime;
	float m_Speed;
};
