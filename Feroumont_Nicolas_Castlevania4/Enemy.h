#pragma once
#include "GameObject.h"

class Texture;
class Sprite;

class Enemy : public GameObject
{
public:
	enum class EnemyState
	{
		alive,
		dying,
		dead
	};

	Enemy(const Point2f& pos);
	~Enemy() override;
	Enemy(const Point2f& pos, float timeDying);
	Enemy(const Enemy& enemy);
	Enemy(Enemy&& enemy) noexcept;
	Enemy& operator=(const Enemy& rhs);
	Enemy& operator=(Enemy&& rhs) noexcept;
	virtual void Update(float elapsedSec);
	virtual void Draw() const;
	Rectf GetShape() const;
	bool IsDead() const;
	bool IsDying() const;
	void SetHealth(float damage);
	bool hasCollided(const Rectf& shape) const;
	float GetDamage() const;
	Point2f Hit(const Rectf& shapeWeapon, const float damage);
	bool isOverlapping(const Rectf& shape) const;
	EnemyState GetState() const;

protected:
	const static std::string m_FolderPath;
	static Texture* m_pTextureDeath;
	Sprite* m_pSpriteDeath;
	Sprite* m_pSpriteEnemy;
	EnemyState m_State;
	Rectf m_Shape;
	bool m_IsHurt;
	float m_Health;
	virtual void DrawAlive() const = 0;
	virtual void DrawDying() const;
	virtual void HandlePosition(float elapsedSec);
	float m_Damage;

private:
	static unsigned m_InstanceCounter;
	void HandleDying(float elapsedSec);
	void HandleHurt(float elapsedSec);
	float m_TimeDying;
	float m_TimeFreeze;
	float m_CurrentTime;
};
