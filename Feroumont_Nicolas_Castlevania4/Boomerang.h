#pragma once
#include "Avatar.h"
#include "Vector2f.h"
#include "Weapon.h"

class Boomerang final : public Weapon
{
public:
	Boomerang(const Point2f& pos, bool isThrown = false);
	Boomerang(const Point2f& pos, const Point2f& startPosition, bool isThrown = false);
	Boomerang(const Boomerang& boomerang);
	Boomerang(Boomerang&& boomerang) noexcept;
	Boomerang& operator=(const Boomerang& rhs);
	Boomerang& operator=(Boomerang&& rhs) noexcept;
	~Boomerang() override;
	void Draw() const override;
	void Update(float elapsedSec, const Level& level, const Rectf& camera) override;
	bool HasHitPlayer(const Rectf& shapePlayer);
	void Reset() override;
	void SetStartPosition(const Point2f& centerPlayer);

private:
	void HandlePosition(float elapsedSec);
	void HandleGoingBack();
	void IsOutOfCamera(const Rectf& camera);

private:
	Point2f m_StartPosition;
	float m_Speed;
	float m_LengthMax;
	Vector2f m_Velocity;
	float m_RotationTime;
	bool m_IsGoingBack;
};
