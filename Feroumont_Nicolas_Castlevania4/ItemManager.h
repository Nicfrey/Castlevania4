#pragma once
#include <vector>
#include <type_traits>

#include "Candle.h"
#include "HUD.h"
#include "Invincibility.h"

class SoundManagerItem;
class SoundEffect;
class Sparkle;
class Player;
class Weapon;
class Food;
class Heart;
class Money;
class Candle;
class Item;

/**
 * \brief Manage all the items of a level
 */
class ItemManager final
{
public:
	/**
	 * \brief Constructor of class ItemManager
	 * Initialize the vector of pointer of items
	 */
	explicit ItemManager();
	/**
	 * \brief Destructor of ItemManager
	 * Delete all the raw pointer inside of m_pItems and m_pCandles and clear the vector
	 */
	~ItemManager();

	ItemManager(const ItemManager& other) = delete;
	ItemManager(ItemManager&& other) noexcept = delete;
	ItemManager& operator=(const ItemManager& rhs) = delete;
	ItemManager& operator=(ItemManager&& rhs) noexcept = delete;

	template <class T>
	void AddCandle(const Point2f& pos, const T& item);
	template <class T>
	void AddCandleBack(const Point2f pos, const T& item);

	/**
	 * \brief Add an item inside of m_pCandles. The item added is a candle with a little heart by default
	 * \param pos - the position which will the item be
	 */
	void AddCandle(const Point2f& pos);

	void AddCandleBack(const Point2f& pos);

	/**
	 * \brief Update the state of all the items every frame
	 * \param elapsedSec - the time between the previous frame and the current frame
	 * \param level the current level
	 * \param camera the current camera
	 */
	void Update(float elapsedSec, const Level& level, const Rectf& camera);
	/**
	 * \brief Draw all the items at their position on the window
	 */
	void DrawFront() const;
	/**
	 * \brief Draw all items which are in the background (only for the second block)
	 */
	void DrawBack() const;

	/**
	 * \brief Check if at the current position, one of the items is being hit by the player
	 * \param shapeWeapon the shape of the main weapon
	 * \param thrownWeapon if the player has thrown a sub weapon or not
	 * \param shapeSubWeapon the shape of the weapon
	 * \return true if one of the item has been hit
	 */
	bool HitCandleFront(const Player& player);

	/**
	 * \brief Check if at the current position, one of the items is being hit by the player
	 * \param shapeWeapon the shape of the main weapon
	 * \param thrownWeapon if the player has thrown a sub weapon or not
	 * \param shapeSubWeapon the shape of the weapon
	 * \return true if one of the item has been hit
	 */
	bool HitCandleBack(const Player& player);

	/**
	 * \brief Check if one of the item is overlapping with the shape
	 * \param player - the shape that is overlapping one of the items
	 * \return true if one of the item is overlapping with the shape
	 */
	bool IsOverlappingItemFront(Player& player);

	/**
	 * \brief Check if one of the item is overlapping with the shape
	 * \param player - the shape that is overlapping one of the items
	 * \return true if one of the item is overlapping with the shape
	 */
	bool IsOverlappingItemBack(Player& player);

	void Clear();

private:
	SoundManagerItem* m_pSoundManager;
	/**
	 * \brief The vector that contains every candles of the level.
	 */
	std::vector<Candle*> m_pCandles;

	std::vector<Candle*> m_pCandlesBack;
	/**
	 * \brief The vector that contains every items that are showing in a limited amount of time
	 */
	std::vector<Item*> m_pItems;

	std::vector<Item*> m_pItemsBack;

	std::vector<Sparkle*> m_pSparkles;
	std::vector<Sparkle*> m_pSparklesBack;

	std::vector<HUD::MemberHUD> m_ScoreHud;
	std::vector<HUD::MemberHUD> m_ScoreHudBack;


	void UpdateFrontCandles(float elapsedSec, const Level& level, const Rectf& camera);
	void UpdateBackCandles(float elapsedSec, const Level& level, const Rectf& camera);
	void CreateSparkle(const Point2f& intersectionPoint);
	void CreateSparkleBack(const Point2f& intersectionPoint);
	void UpdateSparkles(float elapsedSec, const Level& level, const Rectf& camera);
	void UpdateSparklesBack(float elapsedSec, const Level& level, const Rectf& camera);
	void AddScoreDisplay(const Point2f& position, int score);
	void AddScoreDisplayBack(const Point2f& position, int score);
	void UpdateScoreDisplay(float elapsedSec);
	void UpdateScoreDisplayBack(float elapsedSec);
	void DisplayScore() const;
	void DisplayScoreBack() const;
	Point2f GetIntersectionPointHit(const Player& player, Candle* pCandle);
	void AddWeaponCandle(const Point2f& pos, const Weapon& weapon);

	template <class T>
	bool CheckItemAndAddToItems(Candle* pCandle);

	template <class T>
	bool CheckItemAndAddToItemsBack(Candle* pCandle);
};


template <class T>
void ItemManager::AddCandle(const Point2f& pos, const T& item)
{
	if (std::is_base_of_v<Item, T>)
	{
		if (std::is_same_v<T, Weapon>)
		{
			const Weapon* pWeapon = dynamic_cast<const Weapon*>(&item);
			AddWeaponCandle(pos, *pWeapon);
		}
		else
		{
			m_pCandles.push_back(new Candle{pos, item});
		}
	}
}

template <class T>
void ItemManager::AddCandleBack(const Point2f pos, const T& item)
{
	if (std::is_base_of_v<Item, T>)
	{
		m_pCandlesBack.push_back(new Candle{pos, item});
	}
}

template <class T>
bool ItemManager::CheckItemAndAddToItems(Candle* pCandle)
{
	bool exist{false};
	T* pItem{pCandle->GetItem<T>()};
	if (pItem)
	{
		T* pTempMoney = new T{*pItem};
		m_pItems.push_back(pTempMoney);
		delete pCandle;
		pCandle = nullptr;
		exist = true;
	}
	return exist;
}

template <class T>
bool ItemManager::CheckItemAndAddToItemsBack(Candle* pCandle)
{
	bool exist{false};
	T* pItem{pCandle->GetItem<T>()};
	if (pItem)
	{
		T* pTempMoney = new T{*pItem};
		m_pItemsBack.push_back(pTempMoney);
		delete pCandle;
		pCandle = nullptr;
		exist = true;
	}
	return exist;
}
