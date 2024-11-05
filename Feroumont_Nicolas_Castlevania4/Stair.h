#pragma once
#include <vector>

#include "GameObject.h"

struct Vector2f;
class Player;

namespace utils
{
	enum class Direction;
}

class Stair final : public GameObject
{
public:
	Stair(const std::vector<Point2f>& vertices);
	~Stair() override;
	Stair(const Stair& other);
	Stair(Stair&& other) noexcept;
	Stair& operator=(const Stair& rhs);
	Stair& operator=(Stair&& rhs) noexcept;
	std::vector<Point2f> GetStairsVertices() const;
	utils::Direction GetDirectionUpStair() const;
	void HandleCollision(Player& player, Vector2f& actorVelocity, bool isOnGround,
	                     const utils::Direction& directionAvatar) const;

private:
	static unsigned m_InstanceStairs;
	Point2f m_StartStair;
	Point2f m_EndStair;
	utils::Direction m_UpDirection;
	utils::Direction m_DownDirection;
};
