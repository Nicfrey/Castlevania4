#include "pch.h"
#include "utils.h"
#include "Menu.h"

#include "SoundManagerMenu.h"
#include "Sprite.h"
#include "Texture.h"

Menu::Menu(const Rectf& window, const MenuType& menuType) : m_pSoundManagerMenu{new SoundManagerMenu()}, m_pTitleMenu{nullptr},
                                                            m_pStartOrContinue{nullptr}, m_pQuit{nullptr},
                                                            m_pSelectionHUD{nullptr}, m_MenuType{menuType},
                                                            m_CurrentSelection{Selection::start}, m_HasSelected{false}
{
	const std::string filePath{"Menu/Background.png"};
	m_pBackground = new Sprite{filePath, 1, 1, 0, 1};
	switch (menuType)
	{
	case MenuType::title:
		LoadTitleMenu(window);
		break;
	case MenuType::gameOver:
		LoadGameOverMenu(window);
		break;
	}
	LoadSelection();
}

Menu::~Menu()
{
	delete m_pTitleMenu;
	m_pTitleMenu = nullptr;
	delete m_pStartOrContinue;
	m_pStartOrContinue = nullptr;
	delete m_pQuit;
	m_pQuit = nullptr;
	delete m_pSelectionHUD;
	m_pSelectionHUD = nullptr;
	delete m_pBackground;
	m_pBackground = nullptr;
	delete m_pSoundManagerMenu;
	m_pSoundManagerMenu = nullptr;
}

Menu::MenuType Menu::GetMenuType() const
{
	return m_MenuType;
}

void Menu::Draw() const
{
	m_pBackground->Draw(1.25f);
	m_pSelectionHUD->Draw();
	m_pTitleMenu->Draw();
	m_pStartOrContinue->Draw();
	m_pQuit->Draw();
}

void Menu::Update()
{
	if (m_HasSelected)
	{
		return;
	}
	const float padding{20.f};
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_UP])
	{
		if (m_CurrentSelection == Selection::quit)
		{
			m_CurrentSelection = Selection::start;
			Point2f posText{m_pStartOrContinue->bottomLeft};
			posText.x -= padding;
			m_pSelectionHUD->bottomLeft = posText;
			m_pSoundManagerMenu->PlayChangeUI();
		}
	}
	if (pStates[SDL_SCANCODE_DOWN])
	{
		if (m_CurrentSelection == Selection::start)
		{
			m_CurrentSelection = Selection::quit;
			Point2f posText{m_pQuit->bottomLeft};
			posText.x -= padding;
			m_pSelectionHUD->bottomLeft = posText;
			m_pSoundManagerMenu->PlayChangeUI();
		}
	}
	if (pStates[SDL_SCANCODE_SPACE])
	{
		m_HasSelected = true;
		m_pSoundManagerMenu->PlayChoose();
	}
}

bool Menu::GetHasSelected() const
{
	return m_HasSelected;
}

void Menu::SetHasSelected(const bool hasSelected)
{
	m_HasSelected = hasSelected;
}

Menu::Selection Menu::GetCurrentSelection() const
{
	return m_CurrentSelection;
}

void Menu::LoadSelection()
{
	const std::string filePathHeart{"HUD/HeartFont.otf"};
	const Color4f whiteColor{1, 1, 1, 1};
	const int sizeFont{10};
	const float padding{20.f};
	const std::string textFont{"<"};
	Point2f posText{m_pStartOrContinue->bottomLeft};
	posText.x -= padding;
	m_pSelectionHUD = new HUD::MemberHUD{posText, filePathHeart, textFont, sizeFont, whiteColor};
	m_CurrentSelection = Selection::start;
}

void Menu::LoadGameOverMenu(const Rectf& window)
{
	const Color4f whiteColor{1, 1, 1, 1};
	int sizeFont{30};
	std::string textFont{"GAME OVER"};
	m_pTitleMenu = new HUD::MemberHUD{Point2f{}, HUD::filePathFont, textFont, sizeFont, whiteColor};
	Point2f posText{utils::GetElementCenterOfRectangle(window, m_pTitleMenu->GetShape())};
	m_pTitleMenu->bottomLeft = posText;

	const float padding{20.f};
	sizeFont = 15;
	textFont = "RESTART";
	m_pStartOrContinue = new HUD::MemberHUD{Point2f{}, HUD::filePathFont, textFont, sizeFont, whiteColor};
	posText = utils::GetElementCenterOfRectangle(window, m_pStartOrContinue->GetShape());
	posText.y -= padding;
	m_pStartOrContinue->bottomLeft = posText;

	textFont = "QUIT";
	m_pQuit = new HUD::MemberHUD{Point2f{}, HUD::filePathFont, textFont, sizeFont, whiteColor};
	posText = utils::GetElementCenterOfRectangle(window, m_pQuit->GetShape());
	posText.y -= padding + m_pStartOrContinue->GetShape().height;
	m_pQuit->bottomLeft = posText;
}

void Menu::LoadTitleMenu(const Rectf& window)
{
	const Color4f whiteColor{1, 1, 1, 1};
	int sizeFont{30};
	std::string filePathTexture{"Menu/MainTitle.png"};
	m_pTitleMenu = new HUD::MemberHUD{Point2f{}, filePathTexture, 1, 1, 0, 1};
	Point2f posText{utils::GetElementCenterOfRectangle(window, m_pTitleMenu->GetShape())};
	m_pTitleMenu->bottomLeft = posText;

	const float padding{20.f};
	sizeFont = 15;
	std::string textFont{"START"};
	m_pStartOrContinue = new HUD::MemberHUD{Point2f{}, HUD::filePathFont, textFont, sizeFont, whiteColor};
	posText = utils::GetElementCenterOfRectangle(window, m_pStartOrContinue->GetShape());
	posText.y = m_pTitleMenu->bottomLeft.y - padding;
	m_pStartOrContinue->bottomLeft = posText;

	textFont = "QUIT";
	m_pQuit = new HUD::MemberHUD{Point2f{}, HUD::filePathFont, textFont, sizeFont, whiteColor};
	posText = utils::GetElementCenterOfRectangle(window, m_pQuit->GetShape());
	posText.y = m_pStartOrContinue->bottomLeft.y - padding;
	m_pQuit->bottomLeft = posText;
}
