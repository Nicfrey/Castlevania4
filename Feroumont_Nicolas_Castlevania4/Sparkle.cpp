#include "pch.h"
#include "Sparkle.h"

#include "Sprite.h"
#include "Texture.h"

unsigned Sparkle::m_InstanceSparkle = 0;

Sparkle::Sparkle(const Point2f& pos) : Item{pos}, m_TimeDisplay{0.5f}, m_CurrentTime{0.f}
{
	m_pSpriteItem->SetRowFrame(4);
	m_pSpriteItem->SetNrFrame(0);
}

Sparkle::Sparkle(const Sparkle& sparkle): Item{sparkle}, m_TimeDisplay{sparkle.m_TimeDisplay},
                                          m_CurrentTime{sparkle.m_CurrentTime}
{
	++m_InstanceSparkle;
}

Sparkle::Sparkle(Sparkle&& sparkle) noexcept: Item{std::move(sparkle)}, m_TimeDisplay{sparkle.m_TimeDisplay},
                                              m_CurrentTime{sparkle.m_CurrentTime}
{
	sparkle.m_TimeDisplay = 0.f;
	sparkle.m_CurrentTime = 0.f;
}

Sparkle& Sparkle::operator=(const Sparkle& rhs)
{
	if (&rhs != this)
	{
		Item::operator=(rhs);
		m_TimeDisplay = rhs.m_TimeDisplay;
		m_CurrentTime = rhs.m_CurrentTime;
	}
	return *this;
}

Sparkle& Sparkle::operator=(Sparkle&& rhs) noexcept
{
	if (&rhs != this)
	{
		Item::operator=(std::move(rhs));
		m_TimeDisplay = rhs.m_TimeDisplay;
		m_CurrentTime = rhs.m_CurrentTime;

		rhs.m_TimeDisplay = 0.f;
		rhs.m_CurrentTime = 0.f;
	}
	return *this;
}

Sparkle::~Sparkle()
{
	--m_InstanceSparkle;
}

void Sparkle::Update(float elapsedSec, const Level& level, const Rectf& camera)
{
	m_CurrentTime += elapsedSec;
}

void Sparkle::Draw() const
{
	if (CanBeDrawn())
	{
		glPushMatrix();
		glTranslatef(m_Position.x, m_Position.y, 0);
		m_pSpriteItem->Draw();
		glPopMatrix();
	}
}

bool Sparkle::CanBeDrawn() const
{
	return m_CurrentTime <= m_TimeDisplay;
}
