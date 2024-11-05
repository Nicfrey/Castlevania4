#include "pch.h"
#include "Game.h"

#include <iomanip>
#include <iostream>

#include "Axe.h"
#include "Food.h"
#include "Heart.h"
#include "utils.h"
#include "Money.h"
#include "Skeleton.h"
#include "Weapon.h"
#include "Turret.h"
#include "Bat.h"
#include "Boomerang.h"
#include "Bottle.h"
#include "Invincibility.h"
#include "Knife.h"
#include "Worm.h"
#include "Unicorn.h"
#include "Spirit.h"
#include "Medusa.h"

Game::Game(const Window& window)
	: BaseGame{window}, m_GameState{GameState::title}, m_HUD{m_Player, GetViewPort()},
	  m_MainMenu{GetViewPort()},
	  m_GameOverMenu{GetViewPort(), Menu::MenuType::gameOver}
{
	Initialize();
}

Game::~Game()
{
	Cleanup();
}

void Game::Initialize()
{
	m_Camera = Camera{GetViewPort().width, GetViewPort().height};
	m_Camera.SetLevelBoundaries(m_Level.GetBoundaries());
	SetViewPort(1024, 768);
}

void Game::Cleanup()
{
}

void Game::Update(float elapsedSec, bool& quit)
{
	switch (m_GameState)
	{
	case GameState::title:
		UpdateMainMenu(elapsedSec, quit);
		break;
	case GameState::play:
		UpdateGame(elapsedSec);
		break;
	case GameState::gameOver:
		UpdateGameOverMenu(elapsedSec, quit);
		break;
	}
}

void Game::Draw() const
{
	ClearBackground();
	switch (m_GameState)
	{
	case GameState::title:
		DrawMainMenu();
		break;
	case GameState::play:
		DrawGame();
		break;
	case GameState::gameOver:
		DrawGameOverMenu();
		break;
	}
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
	case SDLK_i:
		DisplayInputInfo();
		break;
	case SDLK_UP:
		if (m_Level.GetCurrentBlock() == 2)
		{
			m_Level.HandleDoors(m_Player.GetHitBoxAvatar());
		}
		break;
	}
}

void Game::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
}

void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
}

void Game::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
}

void Game::ClearBackground() const
{
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Game::InitializeGame()
{
	InitCandles();
	InitEnemies();
	m_MusicManager.PlayMusicLevel(m_Level.GetCurrentBlock());
}

void Game::UpdateMainMenu(const float elapsedSec, bool& quit)
{
	m_MainMenu.Update();
	if (const bool result = m_MainMenu.GetHasSelected())
	{
		m_LoadingManager.FadeBlack(elapsedSec);
		if (m_LoadingManager.GetCurrentState() == LoadingManager::LoadingState::loading)
		{
			m_MainMenu.SetHasSelected(!result);
			switch (m_MainMenu.GetCurrentSelection())
			{
			case Menu::Selection::start:
				m_GameState = GameState::play;
				InitializeGame();
				break;
			case Menu::Selection::quit:
				quit = true;
			}
		}
	}
}

void Game::UpdateGameOverMenu(const float elapsedSec, bool& quit)
{
	m_LoadingManager.FadeTransparent(elapsedSec);
	m_GameOverMenu.Update();
	if (const bool result = m_GameOverMenu.GetHasSelected())
	{
		m_LoadingManager.FadeBlack(elapsedSec);
		if (m_LoadingManager.GetCurrentState() == LoadingManager::LoadingState::loading)
		{
			m_GameOverMenu.SetHasSelected(!result);
			switch (m_GameOverMenu.GetCurrentSelection())
			{
			case Menu::Selection::start:
				m_GameState = GameState::play;
				m_Level.ResetCurrentBlock();
				ResetAfterGameOver(elapsedSec);
				break;
			case Menu::Selection::quit:
				quit = true;
			}
		}
	}
}

void Game::UpdateGame(const float elapsedSec)
{
	HandleDeadPlayer(elapsedSec);
	HandleGameOver();
	HandleReachLevel(elapsedSec);
	if (m_LoadingManager.GetCurrentState() == LoadingManager::LoadingState::waiting)
	{
		m_Level.Update(m_Camera.GetPosition(m_Player.GetHitBoxAvatar()), m_Camera.GetBoundariesCamera());
		IsHit();
		m_Camera.SetCameraPosition(m_Player.GetHitBoxAvatar());
		m_ItemManager.Update(elapsedSec, m_Level, m_Camera.GetRectCamera());
		m_EnemyManager.Update(elapsedSec, m_Level, m_Camera.GetRectCamera(), m_Player);
		m_Player.Update(elapsedSec, m_Level, m_Camera.GetRectCamera());
		m_ItemManager.IsOverlappingItemFront(m_Player);
		m_EnemyManager.IsOverlappingEnemy(m_Player);
		if (m_Level.IsPLayerInBack())
		{
			m_EnemyManager.IsOverlappingEnemyBack(m_Player);
			m_ItemManager.IsOverlappingItemBack(m_Player);
		}
		m_HUD.Update(elapsedSec, m_Level, m_Player);
	}
	m_LoadingManager.FadeTransparent(elapsedSec);
}

void Game::DrawMainMenu() const
{
	m_MainMenu.Draw();
	m_LoadingManager.Draw(GetViewPort());
}

void Game::DrawGameOverMenu() const
{
	m_GameOverMenu.Draw();
	m_LoadingManager.Draw(GetViewPort());
}

void Game::DrawGame() const
{
	glPushMatrix();
	utils::SetColor(Color4f{1.f, 0.f, 0.f, 1.f});
	glTranslatef(-m_Camera.GetPosition(m_Player.GetHitBoxAvatar()).x,
	             -m_Camera.GetPosition(m_Player.GetHitBoxAvatar()).y, 0.f);
	m_Level.DrawBackground();
	if (m_Level.IsPLayerInBack())
	{
		m_Player.Draw();
	}
	if (m_Level.GetCurrentBlock() == 2)
	{
		m_ItemManager.DrawBack();
		m_EnemyManager.DrawBack();
	}
	m_Level.DrawForeground();
	m_ItemManager.DrawFront();
	m_EnemyManager.DrawFront();
	if (!m_Level.IsPLayerInBack())
	{
		m_Player.Draw();
	}
	glPopMatrix();
	m_LoadingManager.Draw(GetViewPort());
	m_HUD.Draw(m_Player);
}

void Game::IsHit()
{
	if (m_Level.IsPLayerInBack())
	{
		m_ItemManager.HitCandleBack(m_Player);
		m_EnemyManager.HitEnemyBack(m_Player);
	}
	else
	{
		m_ItemManager.HitCandleFront(m_Player);
		m_EnemyManager.HitEnemy(m_Player);
	}
}

void Game::InitCandles()
{
	m_ItemManager.Clear();
	switch (m_Level.GetCurrentBlock())
	{
	case 1:
		InitCandleFirstBlock();
		break;
	case 2:
		InitCandleSecondBlock();
		break;
	case 3:
		InitCandleThirdBlock();
		break;
	case 4:
		InitCandleFourthBlock();
		break;
	case 5:
		InitCandleFifthBlock();
		break;
	case 6:
		InitCandleSixthBlock();
		break;
	case 7:
		InitCandleSeventhBlock();
		break;
	}
}

void Game::InitEnemies()
{
	m_EnemyManager.Clear();
	switch (m_Level.GetCurrentBlock())
	{
	case 2:
		InitEnemiesSecondBlock();
		break;
	case 3:
		InitEnemiesThirdBlock();
		break;
	case 4:
		InitEnemiesFourthBlock();
		break;
	case 5:
		InitEnemiesFifthBlock();
		break;
	case 6:
		InitEnemiesSixthBlock();
		break;
	case 7:
		InitEnemiesSeventhBlock();
		break;
	}
}

void Game::HandleReachLevel(const float elapsedSec)
{
	if (m_Level.HasReachEnd(m_Player.GetHitBoxAvatar()))
	{
		m_LoadingManager.FadeBlack(elapsedSec);
		if (m_LoadingManager.GetCurrentState() == LoadingManager::LoadingState::loading)
		{
			m_Level.ChangeBlock();
			ChangeBlock();
		}
	}
}

void Game::InitCandleFirstBlock()
{
	m_ItemManager.AddCandle(Point2f{119, 70.f}, Weapon{Point2f{119, 70.f}, Weapon::WeaponType::spear});
	m_ItemManager.AddCandle(Point2f{190, 70.f});
	m_ItemManager.AddCandle(Point2f{362, 162}, Knife{Point2f{362, 162}});
	m_ItemManager.AddCandle(Point2f{334, 70.f});
	m_ItemManager.AddCandle(Point2f{386, 70.f});
	m_ItemManager.AddCandle(Point2f{570, 70.f}, Heart{Point2f{570, 70.f}, Heart::HeartType::big});
}

void Game::InitCandleSecondBlock()
{
	// Add front
	m_ItemManager.AddCandle(Point2f{77, 97}, Heart{Point2f{77, 97}, Heart::HeartType::big});
	m_ItemManager.AddCandle(Point2f{140, 70}, Heart{Point2f{140, 70}, Heart::HeartType::big});
	m_ItemManager.AddCandle(Point2f{222, 97}, Weapon{Point2f{222, 97}, Weapon::WeaponType::spear});
	m_ItemManager.AddCandle(Point2f{336, 154});
	m_ItemManager.AddCandle(Point2f{343, 50});
	m_ItemManager.AddCandle(Point2f{508, 110}, Money{Point2f{508, 110}, 300});
	m_ItemManager.AddCandle(Point2f{623, 79}, Money{Point2f{623, 79}, 500});
	m_ItemManager.AddCandle(Point2f{1004, 112});
	m_ItemManager.AddCandle(Point2f{1166, 77});
	m_ItemManager.AddCandle(Point2f{1309, 74}, Food{Point2f{1309, 74}, Food::FoodSize::small});
	m_ItemManager.AddCandle(Point2f{1356, 74}, Heart{Point2f{1356, 74}, Heart::HeartType::big});

	// Add back
	m_ItemManager.AddCandleBack(Point2f{573, 76});
	m_ItemManager.AddCandleBack(Point2f{588, 76});
	m_ItemManager.AddCandleBack(Point2f{842, 66});
	m_ItemManager.AddCandleBack(Point2f{907, 66});
	m_ItemManager.AddCandleBack(Point2f{1244, 156});
	m_ItemManager.AddCandleBack(Point2f{1309, 146}, Money{Point2f{1309, 146}, 500});
	m_ItemManager.AddCandleBack(Point2f{1483, 146});
	m_ItemManager.AddCandleBack(Point2f{1611, 49});
	m_ItemManager.AddCandleBack(Point2f{1771, 76}, Money{Point2f{1771, 76}, 500});
	m_ItemManager.AddCandleBack(Point2f{1892, 85}, Money{Point2f{1892, 85}, 300});
	m_ItemManager.AddCandleBack(Point2f{1916, 85});
}

void Game::InitCandleThirdBlock()
{
	m_ItemManager.AddCandle(Point2f{70, 91});
	m_ItemManager.AddCandle(Point2f{152, 91}, Boomerang{Point2f{156, 91}});
	m_ItemManager.AddCandle(Point2f{232, 114});
	m_ItemManager.AddCandle(Point2f{410, 140});
	m_ItemManager.AddCandle(Point2f{485, 52}, Axe{Point2f{485, 52}});
	m_ItemManager.AddCandle(Point2f{546, 140}, Heart{Point2f{546, 114}, Heart::HeartType::big});
	m_ItemManager.AddCandle(Point2f{586, 55}, Invincibility{Point2f{586, 55}});
	m_ItemManager.AddCandle(Point2f{668, 72});
	m_ItemManager.AddCandle(Point2f{668, 161});
	m_ItemManager.AddCandle(Point2f{780, 66});
	m_ItemManager.AddCandle(Point2f{843, 90}, Money{Point2f{843, 90}, 100});
	m_ItemManager.AddCandle(Point2f{959, 159});
	m_ItemManager.AddCandle(Point2f{971, 66}, Knife{Point2f{971, 66}});
	m_ItemManager.AddCandle(Point2f{1085, 76});
}

void Game::InitCandleFourthBlock()
{
	m_ItemManager.AddCandle(Point2f{9, 94});
	m_ItemManager.AddCandle(Point2f{104, 94});
	m_ItemManager.AddCandle(Point2f{168, 123}, Money{Point2f{168, 123}, 100});
	m_ItemManager.AddCandle(Point2f{360, 133});
	m_ItemManager.AddCandle(Point2f{642, 145}, Food{Point2f{642, 145}, Food::FoodSize::small});
	m_ItemManager.AddCandle(Point2f{809, 167}, Money{Point2f{809, 167}, 300});
	m_ItemManager.AddCandle(Point2f{809, 74});
	m_ItemManager.AddCandle(Point2f{973, 167});
}

void Game::InitCandleFifthBlock()
{
	m_ItemManager.AddCandle(Point2f{61, 169}, Invincibility{Point2f{61, 169}});
	m_ItemManager.AddCandle(Point2f{173, 74});
	m_ItemManager.AddCandle(Point2f{232, 74}, Money{Point2f{232, 74}, 300});
	m_ItemManager.AddCandle(Point2f{297, 74});
	m_ItemManager.AddCandle(Point2f{350, 100}, Money{Point2f{350, 100}, 500});
	m_ItemManager.AddCandle(Point2f{420, 144});
	m_ItemManager.AddCandle(Point2f{523, 74});
	m_ItemManager.AddCandle(Point2f{523, 144}, Money{Point2f{523, 144}, 500});
	m_ItemManager.AddCandle(Point2f{617, 144});
	m_ItemManager.AddCandle(Point2f{617, 74}, Money{Point2f{617, 144}, 500});
}

void Game::InitCandleSixthBlock()
{
	m_ItemManager.AddCandle(Point2f{94, 62});
	m_ItemManager.AddCandle(Point2f{221, 48}, Bottle{Point2f{221, 48}});
	m_ItemManager.AddCandle(Point2f{351, 65}, Money{Point2f{351, 65}, 100});
	m_ItemManager.AddCandle(Point2f{415, 65});
	m_ItemManager.AddCandle(Point2f{448, 65}, Money{Point2f{448, 65}, 300});
	m_ItemManager.AddCandle(Point2f{490, 113}, Heart{Point2f{490, 113}, Heart::HeartType::big});
	m_ItemManager.AddCandle(Point2f{535, 65}, Money{Point2f{535, 65}, 500});
	m_ItemManager.AddCandle(Point2f{563, 65});
	m_ItemManager.AddCandle(Point2f{599, 65});
	m_ItemManager.AddCandle(Point2f{632, 65}, Money{Point2f{632, 65}, 500});
	m_ItemManager.AddCandle(Point2f{843, 62});
	m_ItemManager.AddCandle(Point2f{1066, 58});
	m_ItemManager.AddCandle(Point2f{1262, 65});
	m_ItemManager.AddCandle(Point2f{1262, 160});
	m_ItemManager.AddCandle(Point2f{1412, 90});
	m_ItemManager.AddCandle(Point2f{1130, 160}, Money{Point2f{1130, 160}, 500});
	m_ItemManager.AddCandle(Point2f{1005, 176});
	m_ItemManager.AddCandle(Point2f{876, 176});
	m_ItemManager.AddCandle(Point2f{746, 176}, Money{Point2f{746, 176}, 500});
	m_ItemManager.AddCandle(Point2f{1604, 163});
	m_ItemManager.AddCandle(Point2f{1546, 87});
	m_ItemManager.AddCandle(Point2f{1774, 87}, Food{Point2f{1774, 87}, Food::FoodSize::small});
}

void Game::InitCandleSeventhBlock()
{
	Point2f pos{44, 100};
	m_ItemManager.AddCandle(pos);
	pos = Point2f{213, 100};
	m_ItemManager.AddCandle(pos);
	pos = Point2f{377, 100};
	m_ItemManager.AddCandle(pos);
	pos = Point2f{695, 100};
	m_ItemManager.AddCandle(pos, Food{pos, Food::FoodSize::small});
	pos = Point2f{855, 100};
	m_ItemManager.AddCandle(pos);
	pos = Point2f{1017, 100};
	m_ItemManager.AddCandle(pos, Knife{pos});
	pos = Point2f{1438, 165};
	m_ItemManager.AddCandle(pos, Heart{pos, Heart::HeartType::big});
	pos = Point2f{1497, 100};
	m_ItemManager.AddCandle(pos);
}

void Game::InitEnemiesSecondBlock()
{
	// Add front enemies
	Point2f pos{370, 101};
	m_EnemyManager.AddEnemy(Skeleton{pos});
	pos = Point2f{490, 73};
	m_EnemyManager.AddEnemy(Skeleton{pos});
	pos = Point2f{1171, 66};
	m_EnemyManager.AddEnemyBack(Skeleton{pos});
	pos = Point2f{1450, 131};
	m_EnemyManager.AddEnemyBack(Skeleton{pos});
	pos = Point2f{1592, 139};
	m_EnemyManager.AddEnemyBack(Bat{pos});
	pos = Point2f{1740, 51};
	m_EnemyManager.AddEnemyBack(Skeleton{pos});
	pos = Point2f{1882, 72};
	m_EnemyManager.AddEnemyBack(Bat{pos});
	pos = Point2f{1910, 78};
	m_EnemyManager.AddEnemyBack(Bat{pos});
}

void Game::InitEnemiesThirdBlock()
{
	m_EnemyManager.AddEnemy(Turret{Point2f{225, 29}});
	m_EnemyManager.AddEnemy(Skeleton{Point2f{419, 29}});
	m_EnemyManager.AddEnemy(Bat{Point2f{899, 48}});
	m_EnemyManager.AddEnemy(Bat{Point2f{955, 78}});
	m_EnemyManager.AddEnemy(Skeleton{Point2f{1050, 146}});
	m_EnemyManager.AddEnemy(Turret{Point2f{1110, 27}});
}

void Game::InitEnemiesFourthBlock()
{
	m_EnemyManager.AddEnemy(Skeleton{Point2f{225, 101}});
	m_EnemyManager.AddEnemy(Bat{Point2f{521, 94}});
	m_EnemyManager.AddEnemy(Turret{Point2f{899, 130}});
}

void Game::InitEnemiesFifthBlock()
{
	m_EnemyManager.AddEnemy(Skeleton{Point2f{170, 35}});
	m_EnemyManager.AddEnemy(Skeleton{Point2f{493, 130}});
	m_EnemyManager.AddEnemy(Turret{Point2f{722, 31}});
}

void Game::InitEnemiesSixthBlock()
{
	m_EnemyManager.AddEnemy(Worm{Point2f{333, 189}});
	m_EnemyManager.AddEnemy(Worm{Point2f{433, 189}});
	m_EnemyManager.AddEnemy(Worm{Point2f{735, 111}});
	m_EnemyManager.AddEnemy(Unicorn{Point2f{951, 25}});
	m_EnemyManager.AddEnemy(Unicorn{Point2f{1086, 25}});
	m_EnemyManager.AddEnemy(Unicorn{Point2f{1153, 25}});
	m_EnemyManager.AddEnemy(Worm{Point2f{1661, 143}});
}

void Game::InitEnemiesSeventhBlock()
{
	m_EnemyManager.AddEnemy(Medusa{Point2f{254, 61}});
	m_EnemyManager.AddEnemy(Medusa{Point2f{354, 61}});
	m_EnemyManager.AddEnemy(Medusa{Point2f{410, 61}});
	m_EnemyManager.AddEnemy(Medusa{Point2f{550, 180}});
	m_EnemyManager.AddEnemy(Medusa{Point2f{720, 61}});
	m_EnemyManager.AddEnemy(Medusa{Point2f{870, 61}});
	m_EnemyManager.AddEnemy(Spirit{Point2f{1177, 163}});
	m_EnemyManager.AddEnemy(Spirit{Point2f{1266, 165}});
	m_EnemyManager.AddEnemy(Worm{Point2f{1302, 108}});
}

void Game::DisplayInputInfo() const
{
	std::cout << "----- INPUT CASTLEVANIA 4 -----\n";
	std::cout << "\n---IN MENU---\n";
	std::cout << "ARROW KEYS" << std::setw(5) << ": CHOOSE\n";
	std::cout << "SPACE BAR" << std::setw(5) << ": SELECT\n";
	std::cout << "\n---IN GAME---\n";
	std::cout << "W " << ": JUMP\n";
	std::cout << "X " << ": ATTACK MAIN WEAPON\n";
	std::cout << "C " << ": ATTACK SUB WEAPON\n";
	std::cout << "ARROW KEYS" << std::setw(5) << ": MOVEMENTS\n";
}

void Game::ResetLevel()
{
	ChangeBlock();
	m_HUD.UpdateAfterDeath(m_Player);
}

void Game::ResetAfterGameOver(const float elapsedSec)
{
	m_LoadingManager.FadeBlack(elapsedSec);
	if (m_LoadingManager.GetCurrentState() == LoadingManager::LoadingState::loading)
	{
		m_Player.ResetAfterGameOver();
		ResetLevel();
	}
}

void Game::HandleDeadPlayer(const float elapsedSec)
{
	if (m_Player.IsDead())
	{
		if (m_MusicManager.GetCurrentMusic() != MusicManager::MusicType::dead)
		{
			m_MusicManager.PlayDeadMusic();
		}
		if (!MusicManager::IsPlaying())
		{
			m_LoadingManager.FadeBlack(elapsedSec);
			if (m_LoadingManager.GetCurrentState() == LoadingManager::LoadingState::loading)
			{
				m_Player.ResetAfterDead();
				if (!m_Player.IsGameOver())
				{
					ResetLevel();
				}
			}
		}
	}
}

void Game::HandleGameOver()
{
	if (m_Player.IsGameOver())
	{
		if (m_MusicManager.GetCurrentMusic() != MusicManager::MusicType::gameOver)
		{
			m_GameState = GameState::gameOver;
			m_MusicManager.PlayGameOverMusic();
		}
	}
}

void Game::ChangeBlock()
{
	m_Player.ChangeBlock(m_Level.GetCurrentBlock(), m_Level.GetPreviousBlock());
	m_Camera.SetLevelBoundaries(m_Level.GetBoundaries());
	InitCandles();
	InitEnemies();
	m_MusicManager.PlayMusicLevel(m_Level.GetCurrentBlock());
}
