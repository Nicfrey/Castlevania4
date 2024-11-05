#include "pch.h"
#include "ItemManager.h"
#include "Item.h"
#include "Money.h"

#include <iostream>

#include "Axe.h"
#include "Bottle.h"
#include "Food.h"
#include "Game.h"
#include "Heart.h"
#include "Invincibility.h"
#include "Knife.h"
#include "Level.h"
#include "Player.h"
#include "SoundEffect.h"
#include "Sparkle.h"
#include "Weapon.h"
#include "HUD.h"
#include "SoundManagerItem.h"


ItemManager::ItemManager() : m_pSoundManager{new SoundManagerItem{}}, m_pCandles{std::vector<Candle*>{}},
                             m_pItems{std::vector<Item*>{}}
{
}

ItemManager::~ItemManager()
{
	for (const Candle* candle : m_pCandles)
	{
		delete candle;
	}
	for (const Item* item : m_pItems)
	{
		delete item;
	}
	for (const Candle* candle : m_pCandlesBack)
	{
		delete candle;
	}
	for (const Item* item : m_pItemsBack)
	{
		delete item;
	}
	for (const Sparkle* pSparkle : m_pSparkles)
	{
		delete pSparkle;
	}
	for (const Sparkle* pSparkle : m_pSparklesBack)
	{
		delete pSparkle;
	}
	delete m_pSoundManager;
	m_pSoundManager = nullptr;
	m_pCandles.clear();
	m_pItems.clear();
	m_pCandlesBack.clear();
	m_pItemsBack.clear();
	m_pSparkles.clear();
	m_pSparklesBack.clear();
	m_ScoreHud.clear();
}

void ItemManager::AddCandle(const Point2f& pos)
{
	m_pCandles.push_back(new Candle{pos, Heart{pos}});
}

void ItemManager::AddCandleBack(const Point2f& pos)
{
	m_pCandlesBack.push_back(new Candle{pos, Heart{pos}});
}

void ItemManager::Update(float elapsedSec, const Level& level, const Rectf& camera)
{
	UpdateFrontCandles(elapsedSec, level, camera);
	UpdateSparkles(elapsedSec, level, camera);
	if (level.GetCurrentBlock() == 2)
	{
		UpdateBackCandles(elapsedSec, level, camera);
		UpdateSparklesBack(elapsedSec, level, camera);
	}
}

void ItemManager::DrawFront() const
{
	for (const Candle* pCandle : m_pCandles)
	{
		pCandle->Draw();
	}
	for (const Item* pItem : m_pItems)
	{
		pItem->Draw();
	}
	for (const Sparkle* pSparkle : m_pSparkles)
	{
		pSparkle->Draw();
	}
	DisplayScore();
}

void ItemManager::DrawBack() const
{
	for (const Candle* pCandle : m_pCandlesBack)
	{
		pCandle->Draw();
	}
	for (const Item* pItem : m_pItemsBack)
	{
		pItem->Draw();
	}
	for (const Sparkle* pSparkle : m_pSparklesBack)
	{
		pSparkle->Draw();
	}
	DisplayScoreBack();
}

bool ItemManager::HitCandleFront(const Player& player)
{
	bool isHit{ false };
	size_t index{ 0 };
	while (!isHit && index < m_pCandles.size())
	{
		Candle* pCandle{ m_pCandles[index] };
		const Point2f intersectionHit{ GetIntersectionPointHit(player,pCandle) };
		if (intersectionHit.x > -1)
		{
			isHit = true;
			CreateSparkle(intersectionHit);
			// Check for every kind of item (temporary)
			if (CheckItemAndAddToItems<Money>(pCandle))
			{
				std::cout << "YES MONEY\n";
			}
			else if (CheckItemAndAddToItems<Heart>(pCandle))
			{
				std::cout << "YES HEART\n";
			}
			else if (CheckItemAndAddToItems<Food>(pCandle))
			{
				std::cout << "YES FOOD\n";
			}
			else if (CheckItemAndAddToItems<Weapon>(pCandle))
			{
				std::cout << "YES WEAPON\n";
			}
			else if (CheckItemAndAddToItems<Invincibility>(pCandle))
			{
				std::cout << "YES INVINCIBILITY\n";
			}
			else
			{
				Item* pItem{ new Item(*pCandle->GetItem()) };
				m_pItems.push_back(pItem);
				delete pCandle;
				pCandle = nullptr;
			}
			m_pCandles.erase(m_pCandles.begin() + index);
		}
		++index;
	}
	return isHit;
}

bool ItemManager::HitCandleBack(const Player& player)
{
	bool isHit{false};
	size_t index{0};
	while (!isHit && index < m_pCandlesBack.size())
	{
		Candle* pCandle{m_pCandlesBack[index]};
		const Point2f intersectionHit{ GetIntersectionPointHit(player,pCandle) };
		if (intersectionHit.x > -1)
		{
			isHit = true;
			CreateSparkleBack(intersectionHit);
			// Check for every kind of item (temporary)
			if (CheckItemAndAddToItemsBack<Money>(pCandle))
			{
				std::cout << "YES MONEY\n";
			}
			else if (CheckItemAndAddToItemsBack<Heart>(pCandle))
			{
				std::cout << "YES HEART\n";
			}
			else if (CheckItemAndAddToItemsBack<Food>(pCandle))
			{
				std::cout << "YES FOOD\n";
			}
			else if (CheckItemAndAddToItemsBack<Weapon>(pCandle))
			{
				std::cout << "YES WEAPON\n";
			}
			else
			{
				Item* pItem{ new Item(*pCandle->GetItem()) };
				m_pItemsBack.push_back(pItem);
				delete pCandle;
				pCandle = nullptr;
			}
			m_pCandlesBack.erase(m_pCandlesBack.begin() + index);
		}
		++index;
	}
	return isHit;
}

bool ItemManager::IsOverlappingItemFront(Player& player)
{
	bool isOverlapping{false};
	size_t index{0};
	while (!isOverlapping && index < m_pItems.size())
	{
		Item* pItem{m_pItems[index]};
		if (pItem && pItem->IsOverlapping(player.GetHitBoxAvatar()))
		{
			isOverlapping = true;
			const Money* pMoney{dynamic_cast<Money*>(pItem)};
			if (pMoney)
			{
				AddScoreDisplay(pMoney->GetPosition(), pMoney->GetScore());
				player.AddScore(pMoney->GetScore());
				m_pSoundManager->PlayMoney();
				//pMoney->PlaySound();
			}
			const Heart* pHeart{dynamic_cast<Heart*>(pItem)};
			if (pHeart)
			{
				player.AddAmountOfSubWeapon(pHeart->GetAmount());
				m_pSoundManager->PlayHeart(pHeart->GetAmount());
				//pHeart->PlaySound();
			}
			const Food* pFood{dynamic_cast<Food*>(pItem)};
			if (pFood)
			{
				player.AddHealth(pFood->GetAmount());
				m_pSoundManager->PlayFood();
			}
			const Weapon* pWeapon{dynamic_cast<Weapon*>(pItem)};
			if (pWeapon)
			{
				if (pWeapon->GetWeaponType() == Weapon::WeaponType::spear)
				{
					player.HasSpear();
					// pWeapon->PlaySound();
					m_pSoundManager->PlaySpear();
				}
				else
				{
					player.SetCurrentWeapon(*pWeapon);
					m_pSoundManager->PlaySubWeapon();
				}
			}
			const Invincibility* pInvincibility{dynamic_cast<Invincibility*>(pItem)};
			if (pInvincibility)
			{
				player.SetInvincibility(true);
				m_pSoundManager->PlayInvincibilityOn();
			}
			// Check which type of item it is
			// Food, Money, Heart or Weapon (The rest being just cosmetic)
			// Do something with the player if necessary
			// Delete the item from the list
			delete pItem;
			pItem = nullptr;
			m_pItems.erase(m_pItems.begin() + index);
		}
		++index;
	}
	return isOverlapping;
}

bool ItemManager::IsOverlappingItemBack(Player& player)
{
	bool isOverlapping{false};
	size_t index{0};
	while (!isOverlapping && index < m_pItemsBack.size())
	{
		Item* pItem{m_pItemsBack[index]};
		if (pItem && pItem->IsOverlapping(player.GetHitBoxAvatar()))
		{
			// Check which type of item it is
			// Food, Money, Heart or Weapon (The rest being just cosmetic)
			// Do something with the player if necessary
			// Delete the item from the list
			isOverlapping = true;
			const Money* pMoney{dynamic_cast<Money*>(pItem)};
			if (pMoney)
			{
				AddScoreDisplayBack(pMoney->GetPosition(), pMoney->GetScore());
				player.AddScore(pMoney->GetScore());
				m_pSoundManager->PlayMoney();
			}
			const Heart* pHeart{dynamic_cast<Heart*>(pItem)};
			if (pHeart)
			{
				player.AddAmountOfSubWeapon(pHeart->GetAmount());
				m_pSoundManager->PlayHeart(pHeart->GetAmount());
			}
			const Food* pFood{dynamic_cast<Food*>(pItem)};
			if (pFood)
			{
				player.AddHealth(pFood->GetAmount());
				m_pSoundManager->PlayFood();
			}
			const Weapon* pWeapon{dynamic_cast<Weapon*>(pItem)};
			if (pWeapon)
			{
				if (pWeapon->GetWeaponType() == Weapon::WeaponType::spear)
				{
					player.HasSpear();
					m_pSoundManager->PlaySpear();
				}
				else
				{
					player.SetCurrentWeapon(*pWeapon);
					m_pSoundManager->PlaySubWeapon();
				}
			}
			delete pItem;
			pItem = nullptr;
			m_pItemsBack.erase(m_pItemsBack.begin() + index);
		}
		++index;
	}
	return isOverlapping;
}

void ItemManager::Clear()
{
	for (const Candle* pCandle : m_pCandles)
	{
		delete pCandle;
		pCandle = nullptr;
	}
	m_pCandles.clear();
	for (const Candle* pCandle : m_pCandlesBack)
	{
		delete pCandle;
		pCandle = nullptr;
	}
	m_pCandlesBack.clear();

	for (const Item* pItem : m_pItems)
	{
		delete pItem;
		pItem = nullptr;
	}
	m_pItems.clear();
	for (const Item* pItem : m_pItemsBack)
	{
		delete pItem;
		pItem = nullptr;
	}
	m_pItems.clear();
	m_ScoreHud.clear();
	m_ScoreHudBack.clear();
}

void ItemManager::UpdateFrontCandles(float elapsedSec, const Level& level, const Rectf& camera)
{
	for (Candle* pCandle : m_pCandles)
	{
		if (pCandle->IsOverlapping(camera))
		{
			pCandle->Update(elapsedSec, level, camera);
		}
	}
	size_t indexLimitedItem{0};
	for (Item* pItem : m_pItems)
	{
		// Money* pMoney{ dynamic_cast<Money*>(pItem) };
		// if(pMoney)
		// {
		// 	pMoney->Update(elapsedSec);
		// }
		pItem->Update(elapsedSec, level, camera);
		if (pItem->IsTimeExpired())
		{
			delete pItem;
			pItem = nullptr;
			m_pItems.erase(m_pItems.begin() + indexLimitedItem);
		}
		else
		{
			++indexLimitedItem;
		}
	}
	UpdateScoreDisplay(elapsedSec);
}

void ItemManager::UpdateBackCandles(float elapsedSec, const Level& level, const Rectf& camera)
{
	for (Candle* pCandle : m_pCandlesBack)
	{
		if (pCandle->IsOverlapping(camera))
		{
			pCandle->Update(elapsedSec, level, camera);
		}
	}
	size_t indexLimitedItem{0};
	for (Item* pItem : m_pItemsBack)
	{
		// Money* pMoney{ dynamic_cast<Money*>(pItem) };
		// if(pMoney)
		// {
		// 	pMoney->Update(elapsedSec);
		// }
		pItem->Update(elapsedSec, level, camera);
		if (pItem->IsTimeExpired())
		{
			delete pItem;
			pItem = nullptr;
			m_pItemsBack.erase(m_pItemsBack.begin() + indexLimitedItem);
		}
		else
		{
			++indexLimitedItem;
		}
	}
	UpdateScoreDisplayBack(elapsedSec);
}

void ItemManager::CreateSparkle(const Point2f& intersectionPoint)
{
	m_pSparkles.push_back(new Sparkle{intersectionPoint});
}

void ItemManager::CreateSparkleBack(const Point2f& intersectionPoint)
{
	m_pSparklesBack.push_back(new Sparkle{intersectionPoint});
}

void ItemManager::UpdateSparkles(float elapsedSec, const Level& level, const Rectf& camera)
{
	size_t index{};
	for (Sparkle* pSparkle : m_pSparkles)
	{
		pSparkle->Update(elapsedSec, level, camera);
		if (!pSparkle->CanBeDrawn())
		{
			m_pSparkles.erase(m_pSparkles.begin() + index);
			delete pSparkle;
			pSparkle = nullptr;
		}
		++index;
	}
}

void ItemManager::UpdateSparklesBack(float elapsedSec, const Level& level, const Rectf& camera)
{
	size_t index{};
	for (Sparkle* pSparkle : m_pSparklesBack)
	{
		pSparkle->Update(elapsedSec, level, camera);
		if (!pSparkle->CanBeDrawn())
		{
			delete pSparkle;
			pSparkle = nullptr;
			m_pSparklesBack.erase(m_pSparklesBack.begin() + index);
		}
		++index;
	}
}

void ItemManager::AddScoreDisplay(const Point2f& position, int score)
{
	const int sizeFont{10};
	const Color4f colorFont{1, 1, 1, 1};
	const float timeDisplayed{2.5f};
	m_ScoreHud.push_back(HUD::MemberHUD{
		position, HUD::filePathFont, std::to_string(score), sizeFont, colorFont, timeDisplayed
	});
}

void ItemManager::AddScoreDisplayBack(const Point2f& position, int score)
{
	const int sizeFont{10};
	const Color4f colorFont{1, 1, 1, 1};
	const float timeDisplayed{2.5f};
	m_ScoreHudBack.push_back(HUD::MemberHUD{
		position, HUD::filePathFont, std::to_string(score), sizeFont, colorFont, timeDisplayed
	});
}

void ItemManager::UpdateScoreDisplay(float elapsedSec)
{
	size_t i = m_ScoreHud.size();
	while (i > 0)
	{
		--i;
		HUD::MemberHUD& memberHUD = m_ScoreHud[i];
		memberHUD.timeDisplayed -= elapsedSec;
		if (memberHUD.timeDisplayed <= 0.f)
		{
			m_ScoreHud.erase(m_ScoreHud.begin() + i);
		}
	}
}

void ItemManager::UpdateScoreDisplayBack(float elapsedSec)
{
	size_t index{};
	for (HUD::MemberHUD& memberHUD : m_ScoreHudBack)
	{
		memberHUD.timeDisplayed -= elapsedSec;
		if (memberHUD.timeDisplayed <= 0.f)
		{
			m_ScoreHudBack.erase(m_ScoreHudBack.begin() + index);
		}
		++index;
	}
}

void ItemManager::DisplayScore() const
{
	for (const HUD::MemberHUD& memberHUD : m_ScoreHud)
	{
		memberHUD.Draw();
	}
}

void ItemManager::DisplayScoreBack() const
{
	for (const HUD::MemberHUD& memberHUD : m_ScoreHudBack)
	{
		memberHUD.Draw();
	}
}

Point2f ItemManager::GetIntersectionPointHit(const Player& player, Candle* pCandle)
{
	Point2f intersectionHit{ -1,-1 };
	if (pCandle)
	{
		intersectionHit = pCandle->IsHit(player.GetShapeMainWeapon());
		if (intersectionHit.x < 0 && player.IsThrownSubWeapon())
		{
			intersectionHit = pCandle->IsHit(player.GetShapeSubWeapon());
			if (intersectionHit.x > -1.f)
			{
				player.SubWeaponHit();
			}
		}
	}
	return intersectionHit;
}

void ItemManager::AddWeaponCandle(const Point2f& pos, const Weapon& weapon)
{
	switch (weapon.GetWeaponType())
	{
	case Weapon::WeaponType::time:
		m_pCandles.push_back(new Candle{pos, weapon});
		break;
	case Weapon::WeaponType::boomerang:
		m_pCandles.push_back(new Candle{pos, weapon});
		break;
	case Weapon::WeaponType::knives:
		{
			const Knife knife{weapon.GetPosition()};
			m_pCandles.push_back(new Candle{pos, knife});
		}
		break;
	case Weapon::WeaponType::axe:
		{
			const Axe axe{weapon.GetPosition()};
			m_pCandles.push_back(new Candle{pos, axe});
		}
		break;
	case Weapon::WeaponType::bottle:
		{
			const Bottle bottle{weapon.GetPosition()};
			m_pCandles.push_back(new Candle{pos, bottle});
		}
		break;
	case Weapon::WeaponType::spear:
		m_pCandles.push_back(new Candle{pos, weapon});
		break;
	}
	// m_pCandles.push_back(new Candle{ pos, weapon });
}
