#pragma once
class Level;
class Sprite;
class Texture;
class Player;

class HUD final
{
public:
	const static std::string filePathFont;

	struct MemberHUD
	{
		Point2f bottomLeft;
		Sprite* pSprite;
		float timeDisplayed;
		MemberHUD();
		MemberHUD(const Point2f& pos);
		MemberHUD(const Point2f& pos, const std::string& fontPath, const std::string& text, int ptSize,
		          const Color4f& textColor, float timeDisplayed = 1.f);
		MemberHUD(const Point2f& pos, const std::string& filePath, unsigned nrCols, unsigned nrRows, unsigned startCol,
		          unsigned endCol, unsigned framePerSec = 3, unsigned rowFrame = 1);
		~MemberHUD();
		MemberHUD(const MemberHUD& member);
		MemberHUD(MemberHUD&& member) noexcept;
		MemberHUD& operator=(const MemberHUD& rhs);
		MemberHUD& operator=(MemberHUD&& rhs) noexcept;
		bool operator==(const MemberHUD& rhs) const;
		Rectf GetShape() const;
		void Draw(const float scaleX = 1.f, const float scaleY = 1.f) const;
		void UpdateSpriteFont(const std::string& newText) const;
		void UpdateSpriteText(const unsigned columnFrame, const unsigned rowFrame) const;
	};

	HUD() = delete;
	HUD(const Player& player, const Rectf& window);
	~HUD() = default;
	HUD(const HUD& hud) = delete;
	HUD(HUD&& hud) noexcept = delete;
	HUD& operator=(const HUD& rhs) = delete;
	HUD& operator=(HUD&& rhs) noexcept = delete;
	void Draw(const Player& player) const;
	void RunOutOfTime(Player& player);
	void Update(float elapsedSec, const Level& level, Player& player);
	void UpdateAfterDeath(Player& player);

private:
	const size_t m_MaxHealth{16};
	const float m_StartTimer{400.f};

	MemberHUD m_ScoreText;
	MemberHUD m_PlayerText;
	MemberHUD m_BlockText;
	MemberHUD m_SubWeaponText;
	MemberHUD m_AmountSubWeaponText;
	MemberHUD m_EqualSubWeaponText;
	MemberHUD m_LifeText;
	MemberHUD m_TimeText;

	MemberHUD m_CurrentWeaponSprite;
	MemberHUD m_EmptyHealthSpritePlayer[16];
	MemberHUD m_FullHealthSpritePlayer[16];
	MemberHUD m_ScorePlayer;
	MemberHUD m_BlockLevel;
	MemberHUD m_AmountWeapon;
	MemberHUD m_AmountLife;
	MemberHUD m_TimeLeft;

	Rectf m_Window;
	float m_Time;
	void LoadMemberHUD(const Player& player);
	void UpdateHud(const float elapsedSec, const Level& level, Player& player) const;
	void UpdateTime(const float elapsedSec);
	void UpdateBlock(const Level& level) const;
	void UpdateLife(Player& player) const;
};
