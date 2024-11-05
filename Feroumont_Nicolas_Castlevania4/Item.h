#pragma once
#include "GameObject.h"

class Texture;
class Sprite;
class Level;

class Item : public GameObject
{
public:
	Item();
	Item(const Point2f& pos);
	Item(unsigned startRow, unsigned startCol, unsigned nrFrame, bool isLimited = true);
	Item(const Point2f& pos, unsigned startRow, unsigned startCol, unsigned nrFrame, bool isLimited = true);
	Item(const Item& item);
	Item(Item&& item) noexcept;
	Item& operator=(const Item& rhs);
	Item& operator=(Item&& rhs) noexcept;
	~Item() override;
	virtual void Update(float elapsedSec, const Level& level, const Rectf& camera);
	virtual void Draw() const;
	bool IsOverlapping(const Rectf& shape) const;
	virtual Rectf GetShape() const;
	bool IsLimited() const;
	bool IsTimeExpired() const;

protected:
	static float m_TimeBeforeSelfDestruct;
	static float m_TimeFlashingBeforeSelfDestruct;
	static Texture* m_pTextureItem;
	Sprite* m_pSpriteItem;
	float m_AccTime;
	bool m_IsLimited;
	float m_Acceleration{-981.f};
	float m_SpeedFalling;
	static unsigned int m_InstanceCounter;
};
