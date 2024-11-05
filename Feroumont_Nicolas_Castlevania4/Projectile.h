#pragma once
#include "GameObject.h"

class Sprite;
class Level;

namespace utils
{
	enum class Direction;
}

class Texture;

class Projectile final : public GameObject
{
public:
	Projectile(const Point2f& pos, const utils::Direction& direction, const std::string& filePath);
	Projectile(const Projectile& projectile);
	Projectile(Projectile&& projectile) noexcept;
	Projectile& operator=(const Projectile& rhs);
	Projectile& operator=(Projectile&& rhs) noexcept;
	~Projectile() override;
	void Update(float elapsedSec, const Level& level);
	void Draw() const;
	Rectf GetShape() const;
	bool GetHasCollided() const;
	bool IsOverlapping(const Rectf& shape) const;

private:
	static unsigned m_InstanceProjectile;
	static Texture* m_pTextureProjectile;
	Sprite* m_pSpriteProjectile;
	float m_Speed;
	bool m_HasCollided;
};
