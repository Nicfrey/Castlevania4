#pragma once
#include "HUD.h"

class SoundManagerMenu;

class Menu final
{
public:
	enum class MenuType
	{
		title,
		gameOver
	};

	enum class Selection
	{
		start,
		quit
	};

	explicit Menu(const Rectf& window, const MenuType& menuType = MenuType::title);
	~Menu();
	Menu(const Menu& other) = delete;
	Menu(Menu&& other) noexcept = delete;
	Menu& operator=(const Menu& rhs) = delete;
	Menu& operator=(Menu&& rhs) noexcept = delete;
	MenuType GetMenuType() const;
	void Draw() const;
	void Update();
	bool GetHasSelected() const;
	void SetHasSelected(const bool hasSelected);
	Selection GetCurrentSelection() const;

private:
	SoundManagerMenu* m_pSoundManagerMenu;
	Sprite* m_pBackground;
	HUD::MemberHUD* m_pTitleMenu;
	HUD::MemberHUD* m_pStartOrContinue;
	HUD::MemberHUD* m_pQuit;
	HUD::MemberHUD* m_pSelectionHUD;
	MenuType m_MenuType;
	Selection m_CurrentSelection;
	bool m_HasSelected;

	void LoadSelection();
	void LoadGameOverMenu(const Rectf& window);
	void LoadTitleMenu(const Rectf& window);
};
