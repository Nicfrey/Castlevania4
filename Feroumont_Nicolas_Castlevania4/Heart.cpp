#include "pch.h"
#include "Heart.h"

#include <iostream>

const unsigned Heart::m_AmountWhenBig = 5;
const unsigned Heart::m_AmountWhenSmall = 1;

Heart::Heart(const Point2f& pos, HeartType type) : Item{pos, 2, 8 + static_cast<unsigned>(type), 1}
{
	switch (type)
	{
	case HeartType::big:
		m_Amount = m_AmountWhenBig;
		break;
	case HeartType::small:
		m_Amount = m_AmountWhenSmall;
		break;
	}
}


Heart::Heart(Heart&& heart) noexcept: Item{std::move(heart)}, m_Amount{heart.m_Amount}
{
	heart.m_Amount = 0;
}

Heart& Heart::operator=(const Heart& rhs)
{
	if (&rhs != this)
	{
		m_Amount = rhs.m_Amount;
	}
	return *this;
}

Heart& Heart::operator=(Heart&& rhs) noexcept
{
	if (&rhs != this)
	{
		m_Amount = rhs.m_Amount;

		rhs.m_Amount = 0;
	}
	return *this;
}

void Heart::Update(float elapsedSec, const Level& level, const Rectf& camera)
{
	Item::Update(elapsedSec, level, camera);
	// TODO When is falling, the heart make a sinusoidal move
}

void Heart::Draw() const
{
	Item::Draw();
}

unsigned Heart::GetAmount() const
{
	return m_Amount;
}

unsigned Heart::GetAmountMax()
{
	return m_AmountWhenBig;
}
