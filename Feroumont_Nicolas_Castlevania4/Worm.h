#pragma once
#include "Enemy.h"
#include "Level.h"
#include "Vector2f.h"

class Sprite;

namespace utils
{
	enum class Direction;
}

class Worm final : public Enemy
{
public:
	enum class WormState
	{
		waiting,
		falling,
		attacking
	};

	Worm(const Point2f& pos);
	Worm(const Worm& worm);
	Worm(Worm&& worm) noexcept;
	Worm& operator=(const Worm& rhs);
	Worm& operator=(Worm&& rhs) noexcept;
	~Worm() override;
	void Update(float elapsedSec) override;
	void Update(const Rectf& hitBoxPlayer, const Level& level);
	void Draw() const override;

private:
	static unsigned m_CounterWorm;
	static Texture* m_pTextureWorm;
	WormState m_CurrentState;
	utils::Direction m_Direction;
	float m_Speed;
	float m_Gravity;
	Vector2f m_Velocity;
	void DrawAlive() const override;
	void DrawDying() const override;
	void HandlePosition(float elapsedSec) override;
	void HandleDetectionPlayer(const Rectf& hitBoxPlayer);
	void HandleDirection(const Rectf& hitBoxPlayer);
};
