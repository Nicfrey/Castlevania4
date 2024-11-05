#include "pch.h"
#include "Camera.h"

#include <iostream>

#include "utils.h"

Camera::Camera() : m_Width{0.f}, m_Height{0.f}, m_LevelBoundaries{0.f, 0.f, 0.f, 0.f}, m_Position{0, 0}
{
}

Camera::Camera(float width, float height) : m_Width{width}, m_Height{height}, m_LevelBoundaries{0.f, 0.f, width, height}
{
}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}

Point2f Camera::GetPosition(const Rectf& target) const
{
	Point2f posCamera{Track(target)};
	Clamp(posCamera);
	return posCamera;
}

Rectf Camera::GetRectCamera() const
{
	return Rectf{m_Position.x, m_Position.y, m_Width, m_Height};
}

void Camera::SetCameraPosition(const Rectf& target)
{
	Point2f posCamera{Track(target)};
	Clamp(posCamera);
	m_Position = posCamera;
}

Rectf Camera::GetBoundariesCamera() const
{
	Point2f maxPoint{1000000.f, 1000000.f};
	Clamp(maxPoint);
	return Rectf{0, 0, maxPoint.x, maxPoint.y};
}


Point2f Camera::Track(const Rectf& target) const
{
	const Point2f centerOfTarget{utils::GetCenterOfRectangle(target)};
	return Point2f{centerOfTarget.x - m_Width / 2, centerOfTarget.y - m_Height / 2};
}

void Camera::Clamp(Point2f& bottomLeftPos) const
{
	const float minValueX{m_LevelBoundaries.left};
	const float maxValueX{m_LevelBoundaries.left + m_LevelBoundaries.width};
	const float minValueY{m_LevelBoundaries.bottom};
	const float maxValueY{m_LevelBoundaries.bottom + m_LevelBoundaries.height};
	if (bottomLeftPos.x < minValueX)
	{
		bottomLeftPos.x = minValueX;
	}
	if (bottomLeftPos.x + m_Width > maxValueX)
	{
		bottomLeftPos.x = maxValueX - m_Width;
	}
	if (bottomLeftPos.y < minValueY)
	{
		bottomLeftPos.y = minValueY;
	}
	if (bottomLeftPos.y + m_Height > maxValueY)
	{
		bottomLeftPos.y = maxValueY - m_Height;
		if (bottomLeftPos.y < 0)
		{
			bottomLeftPos.y = 0.f;
		}
	}
}
