#include "pch.h"
#include "LoadingManager.h"

#include "utils.h"

LoadingManager::LoadingManager() : m_CurrentState{LoadingState::waiting}, m_ColorFade{0, 0, 0, 0},
                                   m_SpeedFading{1.5f}, m_TimeBeforeFadeTransparent{1.5f}, m_CurrentTime{}
{
}

void LoadingManager::Draw(const Rectf& window) const
{
	utils::SetColor(m_ColorFade);
	utils::FillRect(window);
}

void LoadingManager::FadeBlack(const float elapsedSec)
{
	if (m_CurrentState == LoadingState::waiting || m_CurrentState == LoadingState::fadingBlack)
	{
		m_CurrentState = LoadingState::fadingBlack;
		m_ColorFade.a += elapsedSec;
		if (m_ColorFade.a > 1.f)
		{
			m_ColorFade.a = 1.f;
			m_CurrentState = LoadingState::loading;
		}
	}
}

void LoadingManager::FadeTransparent(const float elapsedSec)
{
	if (m_CurrentState == LoadingState::fadingTransparent || m_CurrentState == LoadingState::loading)
	{
		m_CurrentState = LoadingState::fadingTransparent;
		m_ColorFade.a -= elapsedSec;
		if (m_ColorFade.a < 0.f)
		{
			m_ColorFade.a = 0.f;
			m_CurrentState = LoadingState::waiting;
		}
	}
}

LoadingManager::LoadingState LoadingManager::GetCurrentState() const
{
	return m_CurrentState;
}
