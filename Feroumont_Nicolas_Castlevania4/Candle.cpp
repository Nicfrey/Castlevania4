#include "pch.h"
#include "Candle.h"

#include <iostream>

#include "Money.h"
#include "utils.h"

float Candle::m_TimeBeforeSelfDestruct = 5.f;
float Candle::m_TimeFlashingBeforeSelfDestruct = 2.f;

// default contains little heart
Candle::Candle(const Point2f& pos) : Item{pos, 1, 0, 2, false}, m_IsDestroyed{false}, m_pItem{new Item{pos, 1, 2, 1}}
{
}

Candle::~Candle()
{
	delete m_pItem;
	m_pItem = nullptr;
}

Candle::Candle(const Candle& other) : Item{other}, m_IsDestroyed{false}, m_pItem{new Item{*other.m_pItem}}
{
}

Candle::Candle(Candle&& other) noexcept : Item{std::move(other)}, m_IsDestroyed{false}, m_pItem{other.m_pItem}
{
	other.m_pItem = nullptr;
}

Candle& Candle::operator=(const Candle& rhs)
{
	if (&rhs != this)
	{
		Item::operator=(rhs);
		delete m_pItem;
		m_pItem = new Item{*rhs.m_pItem};
		m_IsDestroyed = rhs.m_IsDestroyed;
	}
	return *this;
}

Candle& Candle::operator=(Candle&& rhs) noexcept
{
	if (&rhs != this)
	{
		Item::operator=(std::move(rhs));
		delete m_pItem;
		m_pItem = rhs.m_pItem;
		m_IsDestroyed = rhs.m_IsDestroyed;

		rhs.m_pItem = nullptr;
	}
	return *this;
}

bool Candle::GetIsDestroyed() const
{
	return m_IsDestroyed;
}

Point2f Candle::IsHit(const Rectf& shapeWeapon)
{
	const Point2f rayP1Horizontal{shapeWeapon.left, shapeWeapon.bottom + shapeWeapon.height / 2};
	const Point2f rayP2Horizontal{shapeWeapon.left + shapeWeapon.width, shapeWeapon.bottom + shapeWeapon.height / 2};

	// Translate Rect to polygon
	std::vector<Point2f> pointsCandle;
	pointsCandle.push_back(m_Position);
	pointsCandle.push_back(Point2f{m_Position.x, m_Position.y + GetShape().height});
	pointsCandle.push_back(Point2f{m_Position.x + GetShape().width, m_Position.y});
	pointsCandle.push_back(Point2f{m_Position.x + GetShape().width, m_Position.y + GetShape().height});

	utils::HitInfo hitInfo;
	if (utils::Raycast(pointsCandle, rayP1Horizontal, rayP2Horizontal, hitInfo))
	{
		m_IsDestroyed = true;
		return hitInfo.intersectPoint;
	}
	return Point2f{-300, -300};
}

void Candle::Update(float elapsedSec, const Level& level, const Rectf& camera)
{
	Item::Update(elapsedSec, level, camera);
}

void Candle::Draw() const
{
	Item::Draw();
}

Item* Candle::GetItem() const
{
	return m_pItem;
}
