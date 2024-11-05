#pragma once
struct Matrix2x3;
class Texture;

class Sprite final
{
public:
	Sprite(const std::string& filePath, unsigned nrCols, unsigned nrRows, unsigned startCol, unsigned endCol,
	       unsigned framePerSec = 3, unsigned rowFrame = 1);
	Sprite(const std::string& filePath, Texture* texture, unsigned nrCols, unsigned nrRows, unsigned startCol,
	       unsigned endCol, unsigned framePerSec = 3, unsigned rowFrame = 1);
	Sprite(const std::string& fontPath, const std::string& text, int ptSize, const Color4f& textColor);
	~Sprite();
	Sprite(const Sprite& sprite);
	Sprite(Sprite&& sprite) noexcept;
	Sprite& operator=(const Sprite& rhs);
	Sprite& operator=(Sprite&& rhs) noexcept;
	void Draw(float scale = 1.f) const;
	void HandleFrames(float elapsedSec, float frameTime = 0);
	Rectf GetSrcRect() const;
	Rectf GetShape() const;
	void SetScale(float scale = 1.f);
	void SetRowFrame(unsigned rowFrame);
	void SetAnimFrame(unsigned animFrame = 0);
	void SetNrFrame(unsigned startCol, unsigned nrFrame = 1);
	unsigned GetAnimFrame() const;
	unsigned GetRowFrame() const;
	unsigned GetNrFrame() const;
	unsigned GetStartCol() const;
	float GetAnimTime() const;
	void ResetAnimTime();
	bool HasDoneLoop() const;
	void ChangeText(const std::string& newText);

private:
	void LoadTexture();
	void LoadTexture(const std::string& text, int size, const Color4f& color);
	Texture* m_pTexture;
	unsigned m_NrCols;
	unsigned m_NrRows;
	Rectf m_DestShape;
	Rectf m_Shape;

	std::string m_FilePath;
	unsigned m_NrOffFrames;
	unsigned m_RowFrame;
	unsigned m_StartCol;
	unsigned m_NrFramePerSec;
	unsigned m_AnimFrame;
	float m_AnimTime;
	bool m_LoopDone;
	bool m_TextureIsCreated;

	bool m_IsFont;
	std::string m_TextDisplayed;
	int m_SizeFont;
	Color4f m_ColorFont;
};
