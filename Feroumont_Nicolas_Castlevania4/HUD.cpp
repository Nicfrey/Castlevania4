#include "pch.h"
#include "HUD.h"

#include <iomanip>
#include <sstream>

#include "Player.h"
#include "Sprite.h"
#include "Texture.h"
#include "Level.h"
#include "utils.h"

#include <string>

const std::string HUD::filePathFont = "HUD/Castlevania_Font.ttf";

HUD::MemberHUD::MemberHUD() : bottomLeft{0, 0}, pSprite{nullptr}, timeDisplayed{}
{
}

HUD::MemberHUD::MemberHUD(const Point2f& pos): MemberHUD{}
{
	bottomLeft = pos;
}

HUD::MemberHUD::MemberHUD(const Point2f& pos, const std::string& fontPath, const std::string& text, int ptSize,
                          const Color4f& textColor, float timeDisplayed): bottomLeft{pos}, timeDisplayed{timeDisplayed}
{
	pSprite = new Sprite{fontPath, text, ptSize, textColor};
}

HUD::MemberHUD::MemberHUD(const Point2f& pos, const std::string& filePath, unsigned nrCols, unsigned nrRows,
                          unsigned startCol, unsigned endCol, unsigned framePerSec, unsigned rowFrame): bottomLeft{pos},
	timeDisplayed{0.f}
{
	pSprite = new Sprite{filePath, nrCols, nrRows, startCol, endCol, framePerSec, rowFrame};
}

HUD::MemberHUD::~MemberHUD()
{
	delete pSprite;
	pSprite = nullptr;
}

HUD::MemberHUD::MemberHUD(const MemberHUD& member) : bottomLeft{member.bottomLeft},
                                                     pSprite{new Sprite{*member.pSprite}},
                                                     timeDisplayed{member.timeDisplayed}
{
}

HUD::MemberHUD::MemberHUD(MemberHUD&& member) noexcept: bottomLeft{member.bottomLeft}, pSprite{member.pSprite},
                                                        timeDisplayed{member.timeDisplayed}
{
	member.pSprite = nullptr;
}

HUD::MemberHUD& HUD::MemberHUD::operator=(const MemberHUD& rhs)
{
	if (&rhs != this)
	{
		delete pSprite;
		pSprite = new Sprite{*rhs.pSprite};
		bottomLeft = rhs.bottomLeft;
		timeDisplayed = rhs.timeDisplayed;
	}
	return *this;
}

HUD::MemberHUD& HUD::MemberHUD::operator=(MemberHUD&& rhs) noexcept
{
	if (&rhs < this)
	{
		delete pSprite;
		pSprite = std::move(rhs.pSprite);
		bottomLeft = rhs.bottomLeft;
		timeDisplayed = rhs.timeDisplayed;

		rhs.timeDisplayed = 0.f;
		rhs.bottomLeft = Point2f{};
		rhs.pSprite = nullptr;
	}
	return *this;
}

bool HUD::MemberHUD::operator==(const MemberHUD& rhs) const
{
	return timeDisplayed == rhs.timeDisplayed && pSprite == rhs.pSprite && bottomLeft.x == rhs.bottomLeft.x &&
		bottomLeft.y == rhs.bottomLeft.y;
}

Rectf HUD::MemberHUD::GetShape() const
{
	return Rectf{bottomLeft.x, bottomLeft.y, pSprite->GetShape().width, pSprite->GetShape().height};
}

void HUD::MemberHUD::Draw(const float scaleX, const float scaleY) const
{
	glPushMatrix();
	glTranslatef(bottomLeft.x, bottomLeft.y, 0.f);
	glScalef(scaleX, scaleY, 1.f);
	pSprite->Draw();
	glPopMatrix();
}

void HUD::MemberHUD::UpdateSpriteFont(const std::string& newText) const
{
	pSprite->ChangeText(newText);
}

void HUD::MemberHUD::UpdateSpriteText(const unsigned columnFrame, const unsigned rowFrame) const
{
	pSprite->SetNrFrame(columnFrame);
	pSprite->SetRowFrame(rowFrame);
}

HUD::HUD(const Player& player, const Rectf& window) : m_Window{window}, m_Time{400.f}
{
	LoadMemberHUD(player);
}

void HUD::Draw(const Player& player) const
{
	m_ScoreText.Draw();
	m_ScorePlayer.Draw();
	m_PlayerText.Draw();
	for (size_t i{}; i < m_MaxHealth; ++i)
	{
		const float scale{1.6f};
		if (i < player.GetHealth())
		{
			m_FullHealthSpritePlayer[i].Draw(1, scale);
		}
		else
		{
			m_EmptyHealthSpritePlayer[i].Draw(1, scale);
		}
	}
	m_SubWeaponText.Draw();
	m_CurrentWeaponSprite.Draw();
	m_BlockText.Draw();
	m_BlockLevel.Draw();
	m_AmountSubWeaponText.Draw();
	m_EqualSubWeaponText.Draw();
	m_AmountWeapon.Draw();
	m_LifeText.Draw();
	m_AmountLife.Draw();
	m_TimeText.Draw();
	m_TimeLeft.Draw();
}

void HUD::RunOutOfTime(Player& player)
{
	if (m_Time <= 0)
	{
		player.Die();
	}
}

void HUD::Update(float elapsedSec, const Level& level, Player& player)
{
	if (player.IsValueUpdated())
	{
		UpdateHud(elapsedSec, level, player);
	}
	UpdateTime(elapsedSec);
	UpdateBlock(level);
	RunOutOfTime(player);
}

void HUD::UpdateAfterDeath(Player& player)
{
	m_Time = m_StartTimer;
	UpdateLife(player);
}

void HUD::LoadMemberHUD(const Player& player)
{
	const std::string filePathHeartFont{"HUD/HeartFont.otf"};
	const std::string filePathPlaceholderWeapon{"HUD/WeaponHolder.png"};
	const std::string filePathHealth{"HUD/Health.png"};
	const std::string filePathItem{"Sprites/Items.png"};
	const int sizeFont{15};
	const Color4f colorFontStatic{utils::RgbIntToColor(50, 205, 50)};
	std::string textFont{"SCORE"};
	const float padding{20.f};
	Point2f posText{0, m_Window.height - padding};

	// Score

	m_ScoreText = MemberHUD{posText, filePathFont, textFont, sizeFont, colorFontStatic};
	const int desiredWidthScore{10};
	std::ostringstream oss;
	oss << std::setw(desiredWidthScore) << std::setfill('0') << player.GetScore();
	textFont = oss.str();
	posText.x += m_ScoreText.pSprite->GetShape().width + padding;
	m_ScorePlayer = MemberHUD{posText, filePathFont, textFont, sizeFont, colorFontStatic};

	posText.x -= m_ScoreText.pSprite->GetShape().width;
	posText.y -= m_ScoreText.pSprite->GetShape().height;

	// Player Health

	textFont = "PLAYER";
	m_PlayerText = MemberHUD{posText, filePathFont, textFont, sizeFont, colorFontStatic};
	posText.x += m_PlayerText.pSprite->GetShape().width;
	posText.y += 3.f;
	for (size_t i{}; i < m_MaxHealth; ++i)
	{
		m_EmptyHealthSpritePlayer[i] = MemberHUD{posText, filePathHealth, 2, 1, 1, 2};
		m_FullHealthSpritePlayer[i] = MemberHUD{posText, filePathHealth, 2, 1, 0, 1};
		posText.x += m_FullHealthSpritePlayer[i].pSprite->GetShape().width;
	}

	// Placeholder weapon

	posText = m_ScorePlayer.bottomLeft;
	posText.x += m_ScorePlayer.pSprite->GetShape().width + padding;
	posText.y -= padding;
	m_SubWeaponText = MemberHUD{posText, filePathPlaceholderWeapon, 1, 1, 0, 1};

	// Weapon

	const Rectf shapePlaceholder{
		m_SubWeaponText.bottomLeft.x, m_SubWeaponText.bottomLeft.y, m_SubWeaponText.pSprite->GetShape().width,
		m_SubWeaponText.pSprite->GetShape().height
	};
	m_CurrentWeaponSprite = MemberHUD{posText, filePathItem, 10, 4, 2, 3, 3, 4};
	const Rectf shapeCurrentWeapon{
		m_CurrentWeaponSprite.bottomLeft.x, m_CurrentWeaponSprite.bottomLeft.y,
		m_CurrentWeaponSprite.pSprite->GetShape().width, m_CurrentWeaponSprite.pSprite->GetShape().height
	};
	posText = utils::GetElementCenterOfRectangle(shapePlaceholder, shapeCurrentWeapon);
	m_CurrentWeaponSprite.bottomLeft = posText;

	// Current block

	posText = Point2f{
		m_SubWeaponText.bottomLeft.x + m_SubWeaponText.pSprite->GetShape().width + padding, m_Window.height - padding
	};
	textFont = "BLOCK";
	m_BlockText = MemberHUD{posText, filePathFont, textFont, sizeFont, colorFontStatic};

	posText.x += m_BlockText.pSprite->GetShape().width + padding;
	textFont = "1-1";
	const Color4f colorFontDynamic{utils::RgbIntToColor(255, 140, 0)};
	m_BlockLevel = MemberHUD{posText, filePathFont, textFont, sizeFont, colorFontDynamic};

	// Amount SubWeapon
	textFont = "<"; // the font translate this to a heart
	posText = m_BlockText.bottomLeft;
	posText.y -= m_BlockText.GetShape().height;
	m_AmountSubWeaponText = MemberHUD{posText, filePathHeartFont, textFont, sizeFont, colorFontStatic};
	posText.x += m_AmountSubWeaponText.GetShape().width;
	textFont = "=";
	m_EqualSubWeaponText = MemberHUD{posText, filePathFont, textFont, sizeFont, colorFontStatic};
	const int desiredWidthWeaponLife{2};
	oss.str("");
	oss << std::setw(desiredWidthWeaponLife) << std::setfill('0') << player.GetAmountOfSubWeapon();
	textFont = oss.str();
	posText.x += m_EqualSubWeaponText.GetShape().width;
	m_AmountWeapon = MemberHUD{posText, filePathFont, textFont, sizeFont, colorFontDynamic};

	// Life player
	posText = m_AmountWeapon.bottomLeft;
	posText.x += m_AmountWeapon.GetShape().width + padding;
	textFont = "P=";
	m_LifeText = MemberHUD{posText, filePathFont, textFont, sizeFont, colorFontStatic};

	posText.x += m_LifeText.GetShape().width;
	oss.str("");
	oss << std::setw(desiredWidthWeaponLife) << std::setfill('0') << player.GetNrLife();
	textFont = oss.str();
	m_AmountLife = MemberHUD{posText, filePathFont, textFont, sizeFont, colorFontDynamic};

	// Timer
	posText = m_AmountSubWeaponText.bottomLeft;
	posText.y -= m_AmountSubWeaponText.GetShape().height;
	textFont = "TIME";
	m_TimeText = MemberHUD{posText, filePathFont, textFont, sizeFont, colorFontStatic};

	const float paddingTime{5.f};
	posText.x += m_TimeText.GetShape().width + paddingTime;
	const int desiredWidthTime{3};
	oss.str("");
	oss << std::setw(desiredWidthTime) << std::setfill('0') << static_cast<int>(m_Time);
	textFont = oss.str();
	m_TimeLeft = MemberHUD{posText, filePathFont, textFont, sizeFont, colorFontDynamic};
}

void HUD::UpdateHud(const float elapsedSec, const Level& level, Player& player) const
{
	// Update images
	if (player.HasSubWeapon())
	{
		m_CurrentWeaponSprite.UpdateSpriteText(player.GetStartColumnWeapon(), player.GetRowFrameWeapon());
	}
	// Update font

	std::ostringstream oss;
	const int desiredWidthScore{10};
	oss << std::setw(desiredWidthScore) << std::setfill('0') << player.GetScore();
	m_ScorePlayer.UpdateSpriteFont(oss.str());
	oss.str("");
	const int desiredWidthWeaponLife{2};
	oss << std::setw(desiredWidthWeaponLife) << std::setfill('0') << player.GetAmountOfSubWeapon();
	m_AmountWeapon.UpdateSpriteFont(oss.str());
	oss.str("");
	oss << std::setw(desiredWidthWeaponLife) << std::setfill('0') << player.GetNrLife();
	m_AmountLife.UpdateSpriteFont(oss.str());
	player.SetValueUpdated(false);
}

void HUD::UpdateTime(const float elapsedSec)
{
	const int desiredWidthTime{3};
	std::ostringstream oss;
	oss << std::setw(desiredWidthTime) << std::setfill('0') << static_cast<int>(m_Time);
	m_Time -= elapsedSec;
	m_TimeLeft.UpdateSpriteFont(oss.str());
}

void HUD::UpdateBlock(const Level& level) const
{
	m_BlockLevel.UpdateSpriteFont("1-" + std::to_string(level.GetCurrentBlock()));
}

void HUD::UpdateLife(Player& player) const
{
	const int desiredWidthLife{2};
	std::ostringstream oss;
	oss.str("");
	oss << std::setw(desiredWidthLife) << std::setfill('0') << player.GetNrLife();
	m_AmountLife.UpdateSpriteFont(oss.str());
	player.SetValueUpdated(false);
}
