#include "pch.h"
#include "Food.h"

#include <iostream>

const unsigned Food::m_AmountWhenBig = 8;
const unsigned Food::m_AmountWhenSmall = 4;

Food::Food(const Point2f& pos, const FoodSize& foodSize): Item{pos, 2, 6 + static_cast<unsigned>(foodSize), 1}
{
	switch (foodSize)
	{
	case FoodSize::small:
		m_Amount = m_AmountWhenSmall;
		break;
	case FoodSize::big:
		m_Amount = m_AmountWhenBig;
		break;
	}
}

void Food::Update(float elapsedSec, const Level& level, const Rectf& camera)
{
	Item::Update(elapsedSec, level, camera);
}

void Food::Draw() const
{
	Item::Draw();
}

unsigned Food::GetAmount() const
{
	return m_Amount;
}
