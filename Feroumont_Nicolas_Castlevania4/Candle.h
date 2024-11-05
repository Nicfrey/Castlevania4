#pragma once
#include "Item.h"

class Heart;
class Money;

class Candle : public Item
{
public:
	Candle(const Point2f& pos);
	template <class T>
	Candle(const Point2f& pos, const T& item);
	~Candle() override;
	Candle(const Candle& other);
	Candle(Candle&& other) noexcept;
	Candle& operator=(const Candle& rhs);
	Candle& operator=(Candle&& rhs) noexcept;
	bool GetIsDestroyed() const;
	Point2f IsHit(const Rectf& shapeWeapon);
	void Update(float elapsedSec, const Level& level, const Rectf& camera) override;
	void Draw() const override;

	template <class T>
	T* GetItem() const;

	Item* GetItem() const;

private:
	static float m_TimeBeforeSelfDestruct;
	static float m_TimeFlashingBeforeSelfDestruct;
	bool m_IsDestroyed;
	Item* m_pItem;
};

template <class T>
Candle::Candle(const Point2f& pos, const T& item): Candle{pos}
{
	delete m_pItem;
	Item* pItem = new T{item};
	m_pItem = pItem;
}

template <class T>
T* Candle::GetItem() const
{
	return dynamic_cast<T*>(m_pItem);
}
