#pragma once
#include "Enemy.h"
#include "Vector2f.h"

class Texture;
class Sprite;

class Medusa final : public Enemy
{
public:
	Medusa(const Point2f& pos);
	Medusa(const Medusa& medusa);
	Medusa(Medusa&& medusa) noexcept;
	Medusa& operator=(const Medusa& rhs);
	Medusa& operator=(Medusa&& rhs) noexcept;
	~Medusa() override;
	void Update(float elapsedSec) override;
	void Draw() const override;

private:
	void DrawAlive() const override;
	void DrawDying() const override;
	void HandlePosition(float elapsedSec) override;
	static unsigned m_CounterMedusa;
	static Texture* m_pTextureMedusa;
	Vector2f m_Velocity;
	float m_RotationTime;
	float m_Speed;
};
