#include "pch.h"
#include "Hook.h"

#include <iostream>

#include "utils.h"

unsigned Hook::m_InstanceHook = 0;

Hook::Hook(std::vector<Point2f> vertices): GameObject{++m_InstanceHook}, m_Vertices{std::move(vertices)}
{
}

Hook::~Hook()
{
	--m_InstanceHook;
	m_Vertices.clear();
}

Hook::Hook(const Hook& other): GameObject{other}, m_Vertices{other.m_Vertices}
{
	++m_InstanceHook;
}

Hook::Hook(Hook&& other) noexcept: GameObject{std::move(other)}, m_Vertices{std::move(other.m_Vertices)}
{
}

Hook& Hook::operator=(const Hook& rhs)
{
	if (&rhs != this)
	{
		m_Vertices = rhs.m_Vertices;
	}
	return *this;
}

Hook& Hook::operator=(Hook&& rhs) noexcept
{
	if (&rhs != this)
	{
		m_Vertices = std::move(rhs.m_Vertices);
	}
	return *this;
}

bool Hook::IsHit(const Rectf& weaponAvatar)
{
	const Point2f rayP1Horizontal{weaponAvatar.left, weaponAvatar.bottom + weaponAvatar.height / 2};
	const Point2f rayP2Horizontal{
		weaponAvatar.left + weaponAvatar.width, weaponAvatar.bottom + weaponAvatar.height / 2
	};


	utils::HitInfo hitInfo;
	if (utils::Raycast(m_Vertices, rayP1Horizontal, rayP2Horizontal, hitInfo))
	{
		return true;
	}
	return false;
}

Point2f Hook::GetCenterHook() const
{
	const Point2f vertice1{m_Vertices[0]};
	const Point2f vertice2{m_Vertices[1]};
	const Point2f vertice3{m_Vertices[2]};
	const Point2f vertice4{m_Vertices[3]};
	std::vector<Point2f> firstLine;
	firstLine.push_back(vertice2);
	firstLine.push_back(vertice4);
	utils::HitInfo hitInfo;
	if (utils::Raycast(firstLine, vertice1, vertice3, hitInfo))
	{
		return hitInfo.intersectPoint;
	}
	return Point2f{-1, -1};
}
