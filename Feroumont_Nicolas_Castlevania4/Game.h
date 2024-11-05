#pragma once
#include "BaseGame.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "ItemManager.h"
#include "Level.h"
#include "LoadingManager.h"
#include "MusicManager.h"
#include "Player.h"
#include "Menu.h"

class Game : public BaseGame
{
public:
	enum class GameState
	{
		title,
		play,
		gameOver
	};

	explicit Game(const Window& window);
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update(float elapsedSec, bool& quit) override;
	void Draw() const override;

	// Event handling
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e) override;
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e) override;
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e) override;
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e) override;
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e) override;

private:
	GameState m_GameState;
	Player m_Player;
	Level m_Level;
	ItemManager m_ItemManager;
	EnemyManager m_EnemyManager;
	Camera m_Camera;
	HUD m_HUD;
	MusicManager m_MusicManager;
	LoadingManager m_LoadingManager;
	Menu m_MainMenu;
	Menu m_GameOverMenu;

	// FUNCTIONS
	void Initialize();
	void Cleanup();
	void ClearBackground() const;

	void InitializeGame();
	void UpdateMainMenu(const float elapsedSec, bool& quit);
	void UpdateGameOverMenu(const float elapsedSec, bool& quit);
	void UpdateGame(const float elapsedSec);
	void DrawMainMenu() const;
	void DrawGameOverMenu() const;
	void DrawGame() const;
	void IsHit();
	void InitCandles();
	void InitEnemies();
	void HandleReachLevel(const float elapsedSec);

	// Candles
	void InitCandleFirstBlock();
	void InitCandleSecondBlock();
	void InitCandleThirdBlock();
	void InitCandleFourthBlock();
	void InitCandleFifthBlock();
	void InitCandleSixthBlock();
	void InitCandleSeventhBlock();

	// Enemies
	void InitEnemiesSecondBlock();
	void InitEnemiesThirdBlock();
	void InitEnemiesFourthBlock();
	void InitEnemiesFifthBlock();
	void InitEnemiesSixthBlock();
	void InitEnemiesSeventhBlock();

	void DisplayInputInfo() const;
	void ResetLevel();
	void ResetAfterGameOver(const float elapsedSec);
	void HandleDeadPlayer(const float elapsedSec);
	void HandleGameOver();
	void ChangeBlock();
};
