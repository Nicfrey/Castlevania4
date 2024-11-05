#include "pch.h"
#include "GameObject.h"

unsigned GameObject::m_InstanceGameObject = 0;

GameObject::GameObject(unsigned id) : m_Id{id}, m_Position{Point2f{0.f, 0.f}}
{
	++m_InstanceGameObject;
}

GameObject::GameObject(unsigned id, const Point2f& pos): GameObject{id}
{
	m_Position = pos;
}

GameObject::GameObject(const GameObject& gameObject): m_Id{gameObject.m_Id}, m_Position{gameObject.m_Position}
{
	++m_InstanceGameObject;
}

GameObject::GameObject(GameObject&& gameObject) noexcept: m_Id{gameObject.m_Id}, m_Position{gameObject.m_Position}
{
	gameObject.m_Position = Point2f{-100.f, -100.f};
	gameObject.m_Id = 0;
}

GameObject& GameObject::operator=(const GameObject& gameObject) = default;

GameObject& GameObject::operator=(GameObject&& rhs) noexcept
{
	if (&rhs != this)
	{
		m_Position = rhs.m_Position;
		m_Id = rhs.m_Id;

		rhs.m_Position = Point2f{-100.f, -100.f};
		rhs.m_Id = 0;
	}
	return *this;
}

GameObject::~GameObject()
{
	--m_InstanceGameObject;
};

unsigned GameObject::GetId() const
{
	return m_Id;
}

Point2f GameObject::GetPosition() const
{
	return m_Position;
}

void GameObject::SetPosition(const Point2f& position)
{
	m_Position = position;
}

void GameObject::SetPosition(float x, float y)
{
	SetPosition(Point2f{x, y});
}
