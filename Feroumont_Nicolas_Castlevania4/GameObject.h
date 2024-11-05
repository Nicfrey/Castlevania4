#pragma once
class GameObject
{
public:
	GameObject(unsigned int id);
	GameObject(unsigned int id, const Point2f& pos);
	GameObject(const GameObject& gameObject);
	GameObject(GameObject&& gameObject) noexcept;
	GameObject& operator=(const GameObject& gameObject);
	GameObject& operator=(GameObject&& rhs) noexcept;
	virtual ~GameObject();
	unsigned int GetId() const;
	Point2f GetPosition() const;
	void SetPosition(const Point2f& position);
	void SetPosition(float x, float y);

private:
	unsigned int m_Id;
	static unsigned m_InstanceGameObject;

protected:
	Point2f m_Position;
};
