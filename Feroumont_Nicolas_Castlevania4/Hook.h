#pragma once
#include <vector>

#include "GameObject.h"

class Hook final : public GameObject
{
public:
	Hook(std::vector<Point2f> vertices);
	~Hook() override;
	Hook(const Hook& other);
	Hook(Hook&& other) noexcept;
	Hook& operator=(const Hook& rhs);
	Hook& operator=(Hook&& rhs) noexcept;
	bool IsHit(const Rectf& weaponAvatar);
	Point2f GetCenterHook() const;

private:
	static unsigned m_InstanceHook;
	std::vector<Point2f> m_Vertices;
};
