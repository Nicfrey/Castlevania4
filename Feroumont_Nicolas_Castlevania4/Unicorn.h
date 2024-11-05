#pragma once
#include "Enemy.h"
#include "Vector2f.h"

namespace utils
{
	enum class Direction;
}

class Texture;

class Unicorn final : public Enemy
{
public:
	Unicorn(const Point2f& pos);
	Unicorn(const Point2f& pos, const utils::Direction& direction);
	~Unicorn() override;
	Unicorn(const Unicorn& other);
	Unicorn(Unicorn&& other) noexcept;
	Unicorn& operator=(const Unicorn& rhs);
	Unicorn& operator=(Unicorn&& rhs) noexcept;
	void Update(float elapsedSec) override;
	void Update(float elapsedSec, const Rectf& actorHitBox);
	void Draw() const override;

protected:
	void DrawAlive() const override;
	void DrawDying() const override;
	void HandlePosition(float elapsedSec) override;

private:
	enum class UnicornState
	{
		sleep,
		dash,
		follow
	};

	static unsigned m_CounterUnicorn;
	static Texture* m_pTextureUnicorn;
	utils::Direction m_Direction;
	UnicornState m_CurrentState;
	float m_Speed;
	Vector2f m_Velocity;
	float m_RangeDetection;
	float m_RangeDid;


	void HandleDetectionPlayer(const Rectf& actorHitBox);
	void HasReachRange();
	void ChangeDirection(const Vector2f directionUnicorn);
	void HandlePosition(float elapsedSec, const Rectf& actorHitBox);
};
