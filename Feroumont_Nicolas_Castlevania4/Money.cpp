#include "pch.h"
#include "Money.h"

#include <iostream>

Money::Money(const Point2f& pos, const unsigned& score): Item{pos, 1, 3, 1}, m_Score{score}
{
}

Money::Money(Money&& other) noexcept: Item{std::move(other)}, m_Score{other.m_Score}
{
}

Money& Money::operator=(const Money& rhs)
{
	if (&rhs != this)
	{
		Item::operator=(rhs);
		m_Score = rhs.m_Score;
	}
	return *this;
}

Money& Money::operator=(Money&& rhs) noexcept
{
	if (&rhs != this)
	{
		Item::operator=(std::move(rhs));
		m_Score = rhs.m_Score;
	}
	return *this;
}

void Money::Update(float elapsedSec, const Level& level, const Rectf& camera)
{
	Item::Update(elapsedSec, level, camera);
}

void Money::Draw() const
{
	Item::Draw();
}

unsigned Money::GetScore() const
{
	return m_Score;
}
