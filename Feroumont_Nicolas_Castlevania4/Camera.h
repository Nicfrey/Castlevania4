#pragma once
class Camera
{
public:
	Camera();
	Camera(float width, float height);
	void SetLevelBoundaries(const Rectf& levelBoundaries);
	Point2f GetPosition(const Rectf& target) const;
	Rectf GetRectCamera() const;
	void SetCameraPosition(const Rectf& target);
	Rectf GetBoundariesCamera() const;

private:
	float m_Width;
	float m_Height;
	Rectf m_LevelBoundaries;
	Point2f m_Position;

	Point2f Track(const Rectf& target) const;
	void Clamp(Point2f& bottomLeftPos) const;
};
