#pragma once
#include "Item.h"

class SoundEffect;

namespace utils
{
	enum class Direction;
}

class Weapon : public Item
{
public:
	enum class WeaponType
	{
		spear,
		time,
		boomerang,
		knives,
		axe,
		bottle
	};

	Weapon(const Point2f& pos, const WeaponType& weapon, bool isThrown = false);
	Weapon(const Weapon& weapon) = default;
	Weapon(Weapon&& weapon) noexcept;
	~Weapon() override = default;
	Weapon& operator=(const Weapon& rhs);
	Weapon& operator=(Weapon&& rhs) noexcept;
	void Update(float elapsedSec, const Level& level, const Rectf& camera) override;
	void Draw() const override;
	WeaponType GetWeaponType() const;
	bool IsOutOfBound() const;
	bool HasBeenHit() const;
	float GetDamage() const;
	void SetIsThrown(bool isThrown);
	virtual void Reset();
	void SetDirection(const utils::Direction& direction);
	void Hit();
	unsigned GetRowFrame() const;
	unsigned GetStartColumn() const;

protected:
	bool m_HasHit;
	bool m_IsThrown;
	bool m_IsOutOfBounds;
	float m_Damage;
	utils::Direction m_Direction;

private:
	WeaponType m_WeaponType;
};
