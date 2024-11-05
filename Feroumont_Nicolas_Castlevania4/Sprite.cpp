#include "pch.h"
#include "Sprite.h"

#include <iostream>

#include "Texture.h"
#include <Matrix2x3.h>

Sprite::Sprite(const std::string& filePath, unsigned nrCols, unsigned nrRows, unsigned startCol, unsigned endCol,
               unsigned framePerSec, unsigned rowFrame) : m_pTexture{nullptr}, m_NrCols{nrCols},
                                                          m_NrRows{nrRows},
                                                          m_FilePath{filePath},
                                                          m_NrOffFrames{endCol - startCol}, m_RowFrame{rowFrame},
                                                          m_StartCol{startCol}, m_NrFramePerSec{framePerSec},
                                                          m_AnimFrame{0},
                                                          m_AnimTime{0.f}, m_LoopDone{false}, m_TextureIsCreated{true},
                                                          m_IsFont{false}, m_SizeFont{}
{
	if (m_NrOffFrames == 0)
	{
		m_NrOffFrames = 1;
	}
	LoadTexture();
	m_Shape = Rectf{0, 0, m_pTexture->GetWidth() / m_NrCols, m_pTexture->GetHeight() / m_NrRows};
	m_DestShape = m_Shape;
}

Sprite::Sprite(const std::string& filePath, Texture* texture, unsigned nrCols, unsigned nrRows, unsigned startCol,
               unsigned endCol,
               unsigned framePerSec, unsigned rowFrame): m_pTexture{texture}, m_NrCols{nrCols},
                                                         m_NrRows{nrRows},
                                                         m_FilePath{filePath},
                                                         m_NrOffFrames{endCol - startCol}, m_RowFrame{rowFrame},
                                                         m_StartCol{startCol}, m_NrFramePerSec{framePerSec},
                                                         m_AnimFrame{0},
                                                         m_AnimTime{0.f}, m_LoopDone{false}, m_TextureIsCreated{false},
                                                         m_IsFont{false}, m_SizeFont{}
{
	if (m_NrOffFrames == 0)
	{
		m_NrOffFrames = 1;
	}
	m_Shape = Rectf{0, 0, m_pTexture->GetWidth() / m_NrCols, m_pTexture->GetHeight() / m_NrRows};
	m_DestShape = m_Shape;
}

Sprite::Sprite(const std::string& fontPath, const std::string& text, int ptSize,
               const Color4f& textColor) : m_pTexture{nullptr}, m_NrCols{}, m_NrRows{},
                                           m_FilePath{fontPath}, m_NrOffFrames{}, m_RowFrame{},
                                           m_StartCol{}, m_NrFramePerSec{}, m_AnimFrame{}, m_AnimTime{},
                                           m_LoopDone{false},
                                           m_TextureIsCreated{true}, m_IsFont{true}, m_TextDisplayed{text},
                                           m_SizeFont{ptSize}, m_ColorFont{textColor}

{
	LoadTexture(text, ptSize, textColor);
	m_Shape = Rectf{0, 0, m_pTexture->GetWidth(), m_pTexture->GetHeight()};
	m_DestShape = m_Shape;
}

Sprite::~Sprite()
{
	if (m_TextureIsCreated || m_IsFont)
	{
		delete m_pTexture;
	}
	m_pTexture = nullptr;
}

Sprite::Sprite(const Sprite& sprite) : m_NrCols{sprite.m_NrCols}, m_NrRows{sprite.m_NrRows},
                                       m_DestShape{sprite.m_DestShape}, m_Shape{sprite.m_Shape},
                                       m_FilePath{sprite.m_FilePath},
                                       m_NrOffFrames{sprite.m_NrOffFrames}, m_RowFrame{sprite.m_RowFrame},
                                       m_StartCol{sprite.m_StartCol}, m_NrFramePerSec{sprite.m_NrFramePerSec},
                                       m_AnimFrame{sprite.m_AnimFrame}, m_AnimTime{sprite.m_AnimTime},
                                       m_LoopDone{sprite.m_LoopDone}, m_TextureIsCreated{sprite.m_TextureIsCreated},
                                       m_IsFont{sprite.m_IsFont}, m_TextDisplayed{m_TextDisplayed},
                                       m_SizeFont{sprite.m_SizeFont}, m_ColorFont{sprite.m_ColorFont}
{
	if (m_TextureIsCreated || m_IsFont)
	{
		LoadTexture();
	}
	else
	{
		m_pTexture = sprite.m_pTexture;
	}
}

Sprite::Sprite(Sprite&& sprite) noexcept: m_pTexture{sprite.m_pTexture}, m_NrCols{sprite.m_NrCols},
                                          m_NrRows{sprite.m_NrRows}, m_DestShape{sprite.m_DestShape},
                                          m_Shape{sprite.m_Shape}, m_FilePath{sprite.m_FilePath},
                                          m_NrOffFrames{sprite.m_NrOffFrames}, m_RowFrame{sprite.m_RowFrame},
                                          m_StartCol{sprite.m_StartCol}, m_NrFramePerSec{sprite.m_NrFramePerSec},
                                          m_AnimFrame{sprite.m_AnimFrame}, m_AnimTime{sprite.m_AnimTime},
                                          m_LoopDone{sprite.m_LoopDone}, m_TextureIsCreated{sprite.m_TextureIsCreated},
                                          m_IsFont{sprite.m_IsFont}, m_TextDisplayed{m_TextDisplayed},
                                          m_SizeFont{sprite.m_SizeFont}, m_ColorFont{sprite.m_ColorFont}
{
	sprite.m_pTexture = nullptr;
}

Sprite& Sprite::operator=(const Sprite& rhs)
{
	if (&rhs != this)
	{
		m_FilePath = rhs.m_FilePath;
		if (rhs.m_TextureIsCreated)
		{
			delete m_pTexture;
			LoadTexture();
		}
		else if (m_IsFont)
		{
			delete m_pTexture;
			LoadTexture(rhs.m_TextDisplayed, rhs.m_SizeFont, rhs.m_ColorFont);
		}
		else
		{
			m_pTexture = rhs.m_pTexture;
		}
		m_NrCols = rhs.m_NrCols;
		m_NrRows = rhs.m_NrRows;
		m_Shape = rhs.m_Shape;
		m_AnimFrame = rhs.m_AnimFrame;
		m_NrFramePerSec = rhs.m_NrFramePerSec;
		m_AnimTime = rhs.m_AnimTime;
		m_AnimFrame = rhs.m_AnimFrame;
		m_NrOffFrames = rhs.m_NrOffFrames;
		m_RowFrame = rhs.m_RowFrame;
		m_LoopDone = rhs.m_LoopDone;
		m_StartCol = rhs.m_StartCol;
		m_TextureIsCreated = rhs.m_TextureIsCreated;
		m_DestShape = rhs.m_DestShape;
		m_IsFont = rhs.m_IsFont;
		m_TextDisplayed = rhs.m_TextDisplayed;
		m_SizeFont = rhs.m_SizeFont;
		m_ColorFont = rhs.m_ColorFont;
	}
	return *this;
}

Sprite& Sprite::operator=(Sprite&& rhs) noexcept
{
	if (&rhs != this)
	{
		m_pTexture = rhs.m_pTexture;
		m_FilePath = rhs.m_FilePath;
		m_NrCols = rhs.m_NrCols;
		m_NrRows = rhs.m_NrRows;
		m_Shape = rhs.m_Shape;
		m_AnimFrame = rhs.m_AnimFrame;
		m_NrFramePerSec = rhs.m_NrFramePerSec;
		m_AnimTime = rhs.m_AnimTime;
		m_AnimFrame = rhs.m_AnimFrame;
		m_NrOffFrames = rhs.m_NrOffFrames;
		m_RowFrame = rhs.m_RowFrame;
		m_LoopDone = rhs.m_LoopDone;
		m_StartCol = rhs.m_StartCol;
		m_TextureIsCreated = rhs.m_TextureIsCreated;
		m_DestShape = rhs.m_DestShape;
		m_IsFont = rhs.m_IsFont;
		m_TextDisplayed = rhs.m_TextDisplayed;
		m_SizeFont = rhs.m_SizeFont;
		m_ColorFont = rhs.m_ColorFont;

		rhs.m_pTexture = nullptr;
	}
	return *this;
}

void Sprite::Draw(float scale) const
{
	glPushMatrix();
	if (!m_IsFont)
	{
		const Rectf srcRect{GetSrcRect()};
		glScalef(scale, scale, 1.f);
		m_pTexture->Draw(m_DestShape, srcRect);
	}
	else
	{
		m_pTexture->Draw(m_DestShape);
	}
	glPopMatrix();
}

void Sprite::HandleFrames(float elapsedSec, float frameTime)
{
	float currentFrameTime{};
	if (frameTime == 0)
	{
		currentFrameTime = 1.f / m_NrFramePerSec;
	}
	else
	{
		currentFrameTime = frameTime;
	}
	m_AnimTime += elapsedSec;
	if (m_AnimTime > currentFrameTime)
	{
		if (!m_LoopDone && m_AnimFrame + 1 == m_NrOffFrames)
		{
			m_LoopDone = true;
		}
		++m_AnimFrame %= m_NrOffFrames;
		m_AnimTime -= currentFrameTime;
	}
}

Rectf Sprite::GetSrcRect() const
{
	Rectf srcRect;
	srcRect.width = m_Shape.width;
	srcRect.height = m_Shape.height;
	srcRect.left = (m_AnimFrame + m_StartCol) % (m_NrOffFrames + m_StartCol) * srcRect.width;
	srcRect.bottom = m_pTexture->GetHeight() + m_RowFrame * srcRect.height;
	return srcRect;
}

Rectf Sprite::GetShape() const
{
	return m_DestShape;
}

void Sprite::SetScale(float scale)
{
	m_DestShape.width = m_Shape.width * scale;
	m_DestShape.height = m_Shape.height * scale;
}

void Sprite::SetRowFrame(unsigned rowFrame)
{
	m_RowFrame = rowFrame;
}

void Sprite::SetAnimFrame(unsigned animFrame)
{
	m_AnimFrame = animFrame;
	m_AnimTime = 0.f;
	m_LoopDone = false;
}

void Sprite::SetNrFrame(unsigned startCol, unsigned nrFrame)
{
	m_StartCol = startCol;
	m_AnimFrame = 0;
	m_NrOffFrames = nrFrame;
	ResetAnimTime();
}

unsigned Sprite::GetAnimFrame() const
{
	return m_AnimFrame;
}

unsigned Sprite::GetRowFrame() const
{
	return m_RowFrame;
}

unsigned Sprite::GetNrFrame() const
{
	return m_NrOffFrames;
}

unsigned Sprite::GetStartCol() const
{
	return m_StartCol;
}

float Sprite::GetAnimTime() const
{
	return m_AnimTime;
}

void Sprite::ResetAnimTime()
{
	m_LoopDone = false;
	m_AnimTime = 0.f;
}

bool Sprite::HasDoneLoop() const
{
	return m_LoopDone;
}

void Sprite::ChangeText(const std::string& newText)
{
	if (!m_IsFont)
	{
		return;
	}
	delete m_pTexture;
	m_pTexture = new Texture{newText, m_FilePath, m_SizeFont, m_ColorFont};
}


void Sprite::LoadTexture()
{
	m_pTexture = new Texture{m_FilePath};
	if (!m_pTexture->IsCreationOk())
	{
		std::cout << "Can't load file at path: '" + m_FilePath << "\n";
		exit(0);
	}
}

void Sprite::LoadTexture(const std::string& text, int size, const Color4f& color)
{
	m_pTexture = new Texture{text, m_FilePath, size, color};
	if (!m_pTexture->IsCreationOk())
	{
		std::cout << "Can't load file at path: '" + m_FilePath << "\n";
		exit(0);
	}
}
