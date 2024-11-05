#include "pch.h"
#include "Stair.h"

#include "utils.h"

unsigned Stair::m_InstanceStairs = 0;

Stair::Stair(const std::vector<Point2f>& vertices): GameObject{++m_InstanceStairs}, m_StartStair{vertices[0]},
                                                    m_EndStair{vertices[1]}
{
	if (m_StartStair.x < m_EndStair.x && m_StartStair.y < m_EndStair.y)
	{
		m_UpDirection = utils::Direction::right;
		m_DownDirection = utils::Direction::left;
	}
	else
	{
		m_UpDirection = utils::Direction::left;
		m_DownDirection = utils::Direction::right;
	}
}

Stair::~Stair() = default;

Stair::Stair(const Stair& other) : GameObject{other}, m_StartStair{other.m_StartStair}, m_EndStair{other.m_EndStair},
                                   m_UpDirection{other.m_UpDirection}, m_DownDirection{other.m_DownDirection}
{
	++m_InstanceStairs;
}

Stair::Stair(Stair&& other) noexcept : GameObject{std::move(other)}, m_StartStair{other.m_StartStair},
                                       m_EndStair{other.m_EndStair}, m_UpDirection{other.m_UpDirection},
                                       m_DownDirection{other.m_DownDirection}
{
}

Stair& Stair::operator=(const Stair& rhs)
{
	if (&rhs != this)
	{
		GameObject::operator=(rhs);
		m_StartStair = rhs.m_StartStair;
		m_EndStair = rhs.m_EndStair;
		m_UpDirection = rhs.m_UpDirection;
	}
	return *this;
}

Stair& Stair::operator=(Stair&& rhs) noexcept
{
	if (&rhs != this)
	{
		GameObject::operator=(std::move(rhs));
		m_StartStair = rhs.m_StartStair;
		m_EndStair = rhs.m_EndStair;
		m_UpDirection = rhs.m_UpDirection;
	}
	return *this;
}

std::vector<Point2f> Stair::GetStairsVertices() const
{
	std::vector<Point2f> verticesStairs;
	verticesStairs.push_back(m_StartStair);
	verticesStairs.push_back(m_EndStair);
	return verticesStairs;
}

utils::Direction Stair::GetDirectionUpStair() const
{
	return m_UpDirection;
}

void Stair::HandleCollision(Player& player, Vector2f& actorVelocity, bool isOnGround,
                            const utils::Direction& directionAvatar) const
{
}
