#pragma once
#include "Enemy.h"
#include "Vector2f.h"

class Level;
class Texture;
class Sprite;

class Skeleton final : public Enemy
{
public:
	Skeleton(const Point2f& pos);
	Skeleton(const Skeleton& skeleton);
	Skeleton(Skeleton&& skeleton) noexcept;
	Skeleton& operator=(const Skeleton& rhs);
	Skeleton& operator=(Skeleton&& rhs) noexcept;
	~Skeleton() override;
	void Update(float elapsedSec) override;
	void Update(const Level& level);
	void Draw() const override;

private:
	void DrawAlive() const override;
	void DrawDying() const override;
	void HandlePosition(float elapsedSec) override;
	static unsigned m_CounterSkeleton;
	static Texture* m_pTextureSkeleton;
	float m_Speed;
	Vector2f m_Velocity;
};
