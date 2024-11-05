#pragma once
#include "Item.h"

class Invincibility final : public Item
{
public:
	Invincibility(const Point2f& pos);
	~Invincibility() override = default;
	void Update(float elapsedSec, const Level& level, const Rectf& camera) override;
	void Draw() const override;
};
