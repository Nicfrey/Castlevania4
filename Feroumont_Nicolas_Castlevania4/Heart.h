#pragma once
#include "Item.h"

class SoundEffect;
/**
 * \brief Item that gives the player a amount of uses for the secondary weapon
 */
class Heart final : public Item
{
public:
	/**
	 * \brief Determine the type of the heart
	 */
	enum class HeartType
	{
		big = 0,
		small = 1
	};

	/**
	 * \brief Constructor of class Heart
	 * \param pos - The position of the heart
	 * \param type - the type of the heart
	 */
	Heart(const Point2f& pos, HeartType type = HeartType::small);
	~Heart() override = default;
	Heart(const Heart& heart) = default;
	Heart(Heart&& heart) noexcept;
	Heart& operator=(const Heart& rhs);
	Heart& operator=(Heart&& rhs) noexcept;
	/**
	 * \brief Update the class heart at every frame
	 * \param elapsedSec - the time between the last update
	 */
	void Update(float elapsedSec, const Level& level, const Rectf& camera) override;

	/**
	 * \brief Draw the class in the window every frame
	 */
	void Draw() const override;
	/**
	 * \brief Get the amount that gives the heart
	 * \return the amount that gives the heart
	 */
	unsigned int GetAmount() const;

	static unsigned int GetAmountMax();

private:
	/**
	 * \brief Say the amount of weapon uses when the heart is big
	 */
	static const unsigned m_AmountWhenSmall;
	/**
	 * \brief Say the amount of weapon uses when the heart is small
	 */
	static const unsigned m_AmountWhenBig;
	/**
	 * \brief The amount of weapon uses
	 */
	unsigned int m_Amount;
};
