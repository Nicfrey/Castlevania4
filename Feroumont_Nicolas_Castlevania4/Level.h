#pragma once
#include <vector>

#include "GameObject.h"

class Hook;
class Sparkle;
class Stair;
class SoundStream;
class EnemyManager;
class ItemManager;
class Player;
struct Vector2f;
class Texture;

namespace utils
{
	struct HitInfo;
	enum class Direction;
}


class Level final : public GameObject
{
public:
	explicit Level();
	explicit Level(unsigned block);
	Level(const Level& level) = delete;
	Level(Level&& level) noexcept = delete;
	Level& operator=(const Level& rhs) = delete;
	Level& operator=(Level&& rhs) noexcept = delete;
	~Level() override;
	void Update(const Point2f& posCamera, const Rectf& boundariesCamera);
	void DrawBackground() const;
	void DrawForeground() const;
	void HandleCollisionActorsOnGround(Point2f& pos, const Rectf& actorHitBox, Vector2f& actorVelocity,
	                                   bool onHook = false) const;
	void HandleCollisionActorOnStairs(Point2f& pos, const Rectf& actorHitBox, Vector2f& actorVelocity) const;
	void HandleCollisionItems(Point2f& pos, const Rectf& shapeItem, float& speed) const;
	bool HandleCollisionProjectile(const Rectf& projectileShape) const;
	void HasHitGround(const Rectf& projectileShape, bool& outHasHit) const;
	void IsOutOfBounds(const Rectf& projectileShape, bool& outIsOutOfBounds) const;
	bool IsOnGround(const Rectf& actorHitBox, bool isOnStairs = false) const;
	bool IsOnStairs(const Rectf& actorHitBox, const Vector2f& actorVelocity) const;
	utils::Direction GetDirectionStairOnActor(const Rectf& actorHitBox, const utils::Direction& directionActor) const;
	Rectf GetBoundaries() const;
	bool HasReachEnd(const Rectf& actorHitBox) const;
	void ChangeBlock();
	void HandleDoors(const Rectf& actorHitBox);
	unsigned GetCurrentBlock() const;
	bool IsPLayerInBack() const;
	void ClearLevel();
	void ResetCurrentBlock();
	Point2f HitHooks(const Rectf& weaponShape) const;
	float GetAngleFromHook(const Point2f& centerActor, const Point2f& centerHook) const;
	unsigned GetPreviousBlock() const;

private:
	Texture* m_pFirstBackground{};
	Texture* m_pSecondBackground{};
	Texture* m_pThirdBackground{};
	Texture* m_pFourthBackground{};
	Texture* m_pCurrentForeground{};
	unsigned m_PreviousBlock;
	unsigned m_CurrentBlock;
	unsigned m_CurrentLevel;
	bool m_PlayerInBack;

	std::vector<Stair*> m_pStairsObject;
	std::vector<Stair*> m_pStairsObjectBack;
	std::vector<Hook*> m_pHooks;
	std::vector<std::vector<Point2f>> m_VerticesGround;
	std::vector<std::vector<Point2f>> m_VerticesGroundBack; // for the second block
	std::vector<std::vector<Point2f>> m_Stairs;
	std::vector<std::vector<Point2f>> m_StairsBack; // for the second block
	std::vector<std::vector<Point2f>> m_EndBoundaries;
	std::vector<std::vector<Point2f>> m_Doors;
	std::vector<std::vector<Point2f>> m_Hooks;

	Rectf m_Boundaries;
	float m_OffsetXFirstBackground;
	float m_OffsetXSecondBackground;
	float m_OffsetXThirdBackground;
	float m_OffsetXFourthBackground;
	float m_StartOffsetY;
	void LoadHooks();
	void LoadLevel();
	void LoadBackgroundTextures();
	void LoadMultipleBackgroundTextures(size_t size);
	void LoadStairs();
	void ClampVertical(Point2f& bottomLeftShape, const Rectf& actorShape, const utils::HitInfo& hitInfo,
	                   Vector2f& actorVelocity) const;
	void ClampHorizontal(Point2f& bottomLeftShape, const Rectf& actorShape, const utils::HitInfo& hitInfo,
	                     Vector2f& actorVelocity) const;
	void LoadOffsetY();
	void HandleOffsetX(const Point2f& posCamera, const Rectf& boundariesCamera);
	void DrawTexturesBackground() const;
	void LoadVerticesLevel();
	void DeleteTextures();
	void ClearVertices();
	void SetBoundaries();
	void LoadForegroundTexture();
};
