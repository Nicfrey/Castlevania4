#include "pch.h"
#include "Invincibility.h"

Invincibility::Invincibility(const Point2f& pos): Item{2, 3, 1}
{
	m_Position = pos;
}

void Invincibility::Update(float elapsedSec, const Level& level, const Rectf& camera)
{
	Item::Update(elapsedSec, level, camera);
}

void Invincibility::Draw() const
{
	Item::Draw();
}
