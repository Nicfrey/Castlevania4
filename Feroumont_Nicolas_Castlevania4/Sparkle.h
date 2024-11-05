#pragma once
#include "Item.h"

class Sparkle final : public Item
{
public:
	Sparkle(const Point2f& pos);
	Sparkle(const Sparkle& sparkle);
	Sparkle(Sparkle&& sparkle) noexcept;
	Sparkle& operator=(const Sparkle& rhs);
	Sparkle& operator=(Sparkle&& rhs) noexcept;
	~Sparkle() override;
	bool CanBeDrawn() const;
	void Update(float elapsedSec, const Level& level, const Rectf& camera) override;
	void Draw() const override;

private:
	static unsigned m_InstanceSparkle;
	float m_TimeDisplay;
	float m_CurrentTime;
};
