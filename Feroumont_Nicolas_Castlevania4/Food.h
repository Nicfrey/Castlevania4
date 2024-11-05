#pragma once
#include "Item.h"

class Food final : public Item
{
public:
	enum class FoodSize
	{
		small = 0,
		big = 1
	};

	Food(const Point2f& pos, const FoodSize& foodSize);
	void Update(float elapsedSec, const Level& level, const Rectf& camera) override;
	void Draw() const override;
	unsigned GetAmount() const;
	~Food() override = default;

private:
	unsigned m_Amount;
	static const unsigned m_AmountWhenSmall;
	static const unsigned m_AmountWhenBig;
};
