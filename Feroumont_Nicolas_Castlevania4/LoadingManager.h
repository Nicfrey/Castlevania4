#pragma once
class LoadingManager final
{
public:
	enum class LoadingState
	{
		fadingBlack,
		fadingTransparent,
		loading,
		waiting
	};

	LoadingManager();
	void Draw(const Rectf& window) const;
	void FadeBlack(const float elapsedSec);
	void FadeTransparent(const float elapsedSec);
	LoadingState GetCurrentState() const;

private:
	LoadingState m_CurrentState;
	Color4f m_ColorFade;
	float m_SpeedFading;
	float m_TimeBeforeFadeTransparent;
	float m_CurrentTime;
};
