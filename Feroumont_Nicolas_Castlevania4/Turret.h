#pragma once
#include <vector>

#include "Enemy.h"

class Texture;
class Sprite;
class Level;
class Projectile;

class Turret final : public Enemy
{
public:
	enum class StateTurret
	{
		idle,
		prepare,
		shoot
	};

	Turret(const Point2f& pos);
	Turret(const Turret& turret);
	Turret(Turret&& turret) noexcept;
	Turret& operator=(const Turret& rhs);
	Turret& operator=(Turret&& rhs) noexcept;
	~Turret() override;
	void Update(float elapsedSec) override;
	void UpdateProjectiles(float elapsedSec, const Level& level, const Rectf& camera);
	void Draw() const override;
	bool ProjectileHitShape(const Rectf& shape);

private:
	void DrawAlive() const override;
	void DrawDying() const override;
	void DrawProjectiles() const;
	void HandleState(float elapsedSec);
	static unsigned m_CounterTurret;
	static Texture* m_pTextureTurret;
	std::vector<Projectile*> m_Projectiles;
	const size_t m_NrProjectilesMax;
	unsigned m_ProjectileShot;
	float m_StateTime;
	StateTurret m_StateTurret;
	float m_TimeBeforePrepare;
	float m_TimeBeforeShoot;
	float m_TimeBetweenShot;
};
