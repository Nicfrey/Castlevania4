#include "pch.h"
#include "Item.h"
#include "utils.h"
#include "Texture.h"
#include "Level.h"

#include <iostream>

#include "Sprite.h"

float Item::m_TimeBeforeSelfDestruct = 5.f;
float Item::m_TimeFlashingBeforeSelfDestruct = 2.f;
unsigned Item::m_InstanceCounter = 0;
Texture* Item::m_pTextureItem;

Item::Item(): GameObject{++m_InstanceCounter}, m_AccTime{0.f}, m_IsLimited{true}, m_SpeedFalling{}
{
	const std::string filePath{"Sprites/Items.png"};
	if (!m_pTextureItem)
	{
		m_pTextureItem = new Texture{filePath};
	}
	if (!m_pTextureItem->IsCreationOk())
	{
		std::cout << "Can't load texture at path : " << filePath << "\n";
		exit(0);
	}
	m_pSpriteItem = new Sprite{filePath, m_pTextureItem, 10, 4, 1, 1, 6};
}

Item::Item(const Point2f& pos): GameObject{++m_InstanceCounter, pos}, m_AccTime{0.f}, m_IsLimited{true},
                                m_SpeedFalling{}
{
	const std::string filePath{"Sprites/Items.png"};
	if (!m_pTextureItem)
	{
		m_pTextureItem = new Texture{filePath};
	}
	if (!m_pTextureItem->IsCreationOk())
	{
		std::cout << "Can't load texture at path : " << filePath << "\n";
		exit(0);
	}
	m_pSpriteItem = new Sprite{filePath, m_pTextureItem, 10, 4, 1, 1, 6};
}

Item::Item(unsigned startRow, unsigned startCol, unsigned nrFrame, bool isLimited) : Item{}
{
	m_pSpriteItem->SetNrFrame(startCol, nrFrame);
	m_pSpriteItem->SetRowFrame(startRow);
	m_IsLimited = isLimited;
}

Item::Item(const Point2f& pos, unsigned startRow, unsigned startCol, unsigned nrFrame, bool isLimited) : Item{
	startRow, startCol, nrFrame, isLimited
}
{
	m_Position = pos;
}

Item::Item(const Item& item): GameObject{item},
                              m_AccTime{item.m_AccTime}, m_IsLimited{item.m_IsLimited},
                              m_SpeedFalling{item.m_SpeedFalling}
{
	const std::string filePath{"Sprites/Items.png"};
	m_pSpriteItem = new Sprite{*item.m_pSpriteItem};
	m_pSpriteItem->SetNrFrame(item.m_pSpriteItem->GetStartCol(), item.m_pSpriteItem->GetNrFrame());
	m_pSpriteItem->SetRowFrame(item.m_pSpriteItem->GetRowFrame());
	++m_InstanceCounter;
}

Item::Item(Item&& item) noexcept : GameObject{std::move(item)}, m_pSpriteItem{item.m_pSpriteItem},
                                   m_AccTime{item.m_AccTime}, m_IsLimited{item.m_IsLimited},
                                   m_SpeedFalling{item.m_SpeedFalling}
{
	item.m_pSpriteItem = nullptr;
	item.m_AccTime = -1.f;
	item.m_SpeedFalling = -1.f;
}

Item& Item::operator=(const Item& rhs)
{
	if (&rhs != this)
	{
		GameObject::operator=(rhs);
		delete m_pSpriteItem;
		const std::string filePath{"Sprites/Items.png"};
		m_pSpriteItem = new Sprite{*rhs.m_pSpriteItem};
		m_pSpriteItem->SetAnimFrame(rhs.m_pSpriteItem->GetAnimFrame());
		m_pSpriteItem->SetRowFrame(rhs.m_pSpriteItem->GetRowFrame());
		m_Position = rhs.m_Position;
		m_AccTime = rhs.m_AccTime;
		m_SpeedFalling = rhs.m_SpeedFalling;
		m_IsLimited = rhs.m_IsLimited;
	}
	return *this;
}

Item& Item::operator=(Item&& rhs) noexcept
{
	if (&rhs != this)
	{
		GameObject::operator=(std::move(rhs));
		m_pSpriteItem = rhs.m_pSpriteItem;
		m_Position = rhs.m_Position;
		m_AccTime = rhs.m_AccTime;
		m_SpeedFalling = rhs.m_SpeedFalling;
		m_IsLimited = rhs.m_IsLimited;

		m_pSpriteItem = nullptr;
	}
	return *this;
}

Item::~Item()
{
	if (m_InstanceCounter <= 1)
	{
		delete m_pTextureItem;
		m_pTextureItem = nullptr;
	}
	delete m_pSpriteItem;
	m_pSpriteItem = nullptr;
	--m_InstanceCounter;
}

void Item::Update(float elapsedSec, const Level& level, const Rectf& camera)
{
	m_pSpriteItem->HandleFrames(elapsedSec);
	if (m_IsLimited)
	{
		m_AccTime += elapsedSec;
		m_SpeedFalling += m_Acceleration * elapsedSec;
		m_Position.y += m_SpeedFalling * elapsedSec;
		level.HandleCollisionItems(m_Position, m_pSpriteItem->GetShape(), m_SpeedFalling);
	}
}

void Item::Draw() const
{
	if (!IsTimeExpired())
	{
		glPushMatrix();
		glTranslatef(m_Position.x, m_Position.y, 0.f);
		m_pSpriteItem->Draw();
		glPopMatrix();
	}
}

bool Item::IsOverlapping(const Rectf& shape) const
{
	return utils::IsOverlapping(Rectf{
		                            m_Position.x, m_Position.y, m_pSpriteItem->GetShape().width,
		                            m_pSpriteItem->GetShape().height
	                            }, shape);
}

Rectf Item::GetShape() const
{
	return Rectf{m_Position.x, m_Position.y, m_pSpriteItem->GetShape().width, m_pSpriteItem->GetShape().height};
}

bool Item::IsLimited() const
{
	return m_IsLimited;
}

bool Item::IsTimeExpired() const
{
	return m_AccTime > m_TimeBeforeSelfDestruct;
}
