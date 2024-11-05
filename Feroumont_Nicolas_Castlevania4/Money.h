#pragma once
#include "Item.h"

class Money final : public Item
{
public:
	Money(const Point2f& pos, const unsigned& score);
	~Money() override = default;
	Money(const Money& other) = default;
	Money(Money&& other) noexcept;
	Money& operator=(const Money& rhs);
	Money& operator=(Money&& rhs) noexcept;
	void Update(float elapsedSec, const Level& level, const Rectf& camera) override;
	void Draw() const override;
	unsigned GetScore() const;

private:
	unsigned m_Score;
};
