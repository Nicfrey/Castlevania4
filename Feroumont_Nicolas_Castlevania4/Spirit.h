#pragma once
#include "Enemy.h"
#include "Vector2f.h"

namespace utils
{
	enum class Direction;
}

class Spirit final : public Enemy
{
public:
	Spirit(const Point2f& pos);
	Spirit(const Point2f& pos, const utils::Direction& direction);
	~Spirit() override;
	Spirit(const Spirit& other);
	Spirit(Spirit&& other) noexcept;
	Spirit& operator=(const Spirit& rhs);
	Spirit& operator=(Spirit&& rhs) noexcept;
	void Update(float elapsedSec) override;
	void Update(float elapsedSec, const Rectf& actorHitBox);
	void Draw() const override;
	bool CanBeHurt() const;

protected:
	void DrawAlive() const override;
	void DrawDying() const override;

private:
	enum class SpiritState
	{
		spawn,
		follow,
		dying
	};

	static unsigned m_CounterSpirit;
	static Texture* m_pTextureSpirit;
	utils::Direction m_Direction;
	SpiritState m_CurrentState;
	float m_Speed;
	Vector2f m_Velocity;

	void HandleSpawning();
	void HandlePosition(float elapsedSec) override;
	void ChangeDirection(const Vector2f& directionVector);
	void HandlePosition(float elapsedSec, const Rectf& actorHitBox);
};
