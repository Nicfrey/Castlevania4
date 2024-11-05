#include "pch.h"
#include "Level.h"

#include <iostream>

#include "Hook.h"
#include "SoundStream.h"
#include "Stair.h"
#include "SVGParser.h"
#include "Texture.h"
#include "utils.h"

Level::Level(): GameObject{1}, m_PreviousBlock{0}, m_CurrentBlock{1}, m_CurrentLevel{0},
                m_PlayerInBack{false}, m_OffsetXFirstBackground{}, m_OffsetXSecondBackground{},
                m_OffsetXThirdBackground{}, m_OffsetXFourthBackground{}, m_StartOffsetY{}
{
	LoadLevel();
}

Level::Level(unsigned block) : GameObject{1}, m_PreviousBlock{block - 1}, m_CurrentBlock{block},
                               m_CurrentLevel{0},
                               m_PlayerInBack{false}, m_OffsetXFirstBackground{}, m_OffsetXSecondBackground{},
                               m_OffsetXThirdBackground{}, m_OffsetXFourthBackground{}, m_StartOffsetY{}
{
	LoadLevel();
}

Level::~Level()
{
	DeleteTextures();
}

void Level::Update(const Point2f& posCamera, const Rectf& boundariesCamera)
{
	HandleOffsetX(posCamera, boundariesCamera);
}

void Level::DrawBackground() const
{
	const Color4f colorBackground{utils::RgbIntToColor(16, 0, 32)};
	utils::SetColor(colorBackground);
	utils::FillRect(Rectf{0, 0, 10000, 10000});
	DrawTexturesBackground();
}

void Level::DrawForeground() const
{
	m_pCurrentForeground->Draw();
}

void Level::HandleCollisionActorsOnGround(Point2f& pos, const Rectf& actorHitBox, Vector2f& actorVelocity,
                                          bool onHook) const
{
	if (onHook)
	{
		return;
	}
	const Point2f rayP1Vertical{pos.x + actorHitBox.width / 2, pos.y};
	const Point2f rayP2Vertical{pos.x + actorHitBox.width / 2, pos.y + actorHitBox.height};

	const Point2f rayP1Horizontal{pos.x, pos.y + actorHitBox.height / 2};
	const Point2f rayP2Horizontal{pos.x + actorHitBox.width, pos.y + actorHitBox.height / 2};

	utils::HitInfo hitInfo;
	if (m_PlayerInBack)
	{
		for (const std::vector<Point2f>& vertices : m_VerticesGroundBack)
		{
			if (Raycast(vertices, rayP1Vertical, rayP2Vertical, hitInfo))
			{
				ClampVertical(pos, actorHitBox, hitInfo, actorVelocity);
			}
			if (Raycast(vertices, rayP1Horizontal, rayP2Horizontal, hitInfo))
			{
				ClampHorizontal(pos, actorHitBox, hitInfo, actorVelocity);
			}
		}
	}
	else
	{
		for (const std::vector<Point2f>& vertices : m_VerticesGround)
		{
			if (Raycast(vertices, rayP1Vertical, rayP2Vertical, hitInfo))
			{
				ClampVertical(pos, actorHitBox, hitInfo, actorVelocity);
			}
			if (Raycast(vertices, rayP1Horizontal, rayP2Horizontal, hitInfo))
			{
				ClampHorizontal(pos, actorHitBox, hitInfo, actorVelocity);
			}
		}
	}
}

void Level::HandleCollisionActorOnStairs(Point2f& pos, const Rectf& actorHitBox, Vector2f& actorVelocity) const
{
	const Point2f rayP1Vertical{pos.x + actorHitBox.width / 2, pos.y};
	const float heightDivided{actorHitBox.height};
	const Point2f rayP2Vertical{pos.x + actorHitBox.width / 2, pos.y + heightDivided};


	utils::HitInfo hitInfo;
	if (m_PlayerInBack)
	{
		for (const Stair* stair : m_pStairsObjectBack)
		{
			if (Raycast(stair->GetStairsVertices(), rayP1Vertical, rayP2Vertical, hitInfo))
			{
				ClampVertical(pos, actorHitBox, hitInfo, actorVelocity);
			}
		}
	}
	else
	{
		for (const Stair* stair : m_pStairsObject)
		{
			if (Raycast(stair->GetStairsVertices(), rayP1Vertical, rayP2Vertical, hitInfo))
			{
				ClampVertical(pos, actorHitBox, hitInfo, actorVelocity);
			}
		}
	}
}

void Level::HandleCollisionItems(Point2f& pos, const Rectf& shapeItem, float& speed) const
{
	const Point2f rayP1{pos.x + shapeItem.width / 2, pos.y};
	const Point2f rayP2{pos.x + shapeItem.width / 2, pos.y + shapeItem.height};
	utils::HitInfo hitInfo;
	if (m_PlayerInBack)
	{
		for (const std::vector<Point2f>& vertices : m_VerticesGroundBack)
		{
			if (Raycast(vertices, rayP1, rayP2, hitInfo))
			{
				pos.y = hitInfo.intersectPoint.y;
				speed = 0.f;
			}
		}
	}
	else
	{
		for (const std::vector<Point2f>& vertices : m_VerticesGround)
		{
			if (Raycast(vertices, rayP1, rayP2, hitInfo))
			{
				pos.y = hitInfo.intersectPoint.y;
				speed = 0.f;
			}
		}
	}
}

bool Level::HandleCollisionProjectile(const Rectf& projectileShape) const
{
	const Point2f rayP1{projectileShape.left + projectileShape.width / 2, projectileShape.bottom};
	const Point2f rayP2{
		projectileShape.left + projectileShape.width / 2, projectileShape.bottom + projectileShape.height
	};
	utils::HitInfo hitInfo;
	for (const std::vector<Point2f>& vertices : m_VerticesGround)
	{
		if (Raycast(vertices, rayP1, rayP2, hitInfo))
		{
			return true;
		}
	}
	return false;
}

void Level::HasHitGround(const Rectf& projectileShape, bool& outHasHit) const
{
	const Point2f rayP1Vertical{projectileShape.left + projectileShape.width / 2, projectileShape.bottom};
	const Point2f rayP2Vertical{
		projectileShape.left + projectileShape.width / 2, projectileShape.bottom + projectileShape.height
	};

	const Point2f rayP1Horizontal{projectileShape.left, projectileShape.bottom + projectileShape.height / 2};
	const Point2f rayP2Horizontal{
		projectileShape.left + projectileShape.width, projectileShape.bottom + projectileShape.height / 2
	};

	utils::HitInfo hitInfo;
	if (m_PlayerInBack)
	{
		for (const std::vector<Point2f>& vertices : m_VerticesGroundBack)
		{
			if (Raycast(vertices, rayP1Vertical, rayP2Vertical, hitInfo)
				|| Raycast(vertices, rayP1Horizontal, rayP2Horizontal, hitInfo))
			{
				outHasHit = true;
			}
		}
	}
	else
	{
		for (const std::vector<Point2f>& vertices : m_VerticesGround)
		{
			if (Raycast(vertices, rayP1Vertical, rayP2Vertical, hitInfo)
				|| Raycast(vertices, rayP1Horizontal, rayP2Horizontal, hitInfo))
			{
				outHasHit = true;
			}
		}
	}
}

void Level::IsOutOfBounds(const Rectf& projectileShape, bool& outIsOutOfBounds) const
{
	outIsOutOfBounds = !utils::IsPointInRect(Point2f{projectileShape.left, projectileShape.bottom},
	                                         Rectf{0.f, 0.f, m_Boundaries.width, 30000.f});
}

bool Level::IsOnGround(const Rectf& actorHitBox, bool isOnStairs) const
{
	const Point2f rayP1{actorHitBox.left + actorHitBox.width / 2, actorHitBox.bottom - 1.f};
	Point2f rayP2{actorHitBox.left + actorHitBox.width / 2, actorHitBox.bottom + actorHitBox.height};
	if (isOnStairs)
	{
		const float heightDivided{actorHitBox.height / 1.25f};
		rayP2 = Point2f{actorHitBox.left + actorHitBox.width / 2, actorHitBox.bottom + heightDivided};
	}
	utils::HitInfo hitInfo;
	if (m_PlayerInBack)
	{
		for (const std::vector<Point2f>& vertices : m_VerticesGroundBack)
		{
			if (Raycast(vertices, rayP1, rayP2, hitInfo))
			{
				return true;
			}
		}
	}
	else
	{
		for (const std::vector<Point2f>& vertices : m_VerticesGround)
		{
			if (Raycast(vertices, rayP1, rayP2, hitInfo))
			{
				return true;
			}
		}
	}
	return false;
}

bool Level::IsOnStairs(const Rectf& actorShape, const Vector2f& actorVelocity) const
{
	if (actorVelocity.y > 0)
	{
		return false;
	}
	const Point2f rayP1{actorShape.left + actorShape.width / 2, actorShape.bottom - 1.f};
	const Point2f rayP2{actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height};

	utils::HitInfo hitInfo;
	if (m_PlayerInBack)
	{
		for (const std::vector<Point2f>& vertices : m_StairsBack)
		{
			if (Raycast(vertices, rayP1, rayP2, hitInfo))
			{
				return true;
			}
		}
	}
	else
	{
		for (const std::vector<Point2f>& vertices : m_Stairs)
		{
			if (Raycast(vertices, rayP1, rayP2, hitInfo))
			{
				return true;
			}
		}
	}
	return false;
}

utils::Direction Level::GetDirectionStairOnActor(const Rectf& actorHitBox, const utils::Direction& directionActor) const
{
	const Point2f rayP1{actorHitBox.left + actorHitBox.width / 2, actorHitBox.bottom - 1.f};
	const Point2f rayP2{actorHitBox.left + actorHitBox.width / 2, actorHitBox.bottom + actorHitBox.height};

	utils::HitInfo hitInfo;
	if (m_PlayerInBack)
	{
		for (const Stair* stair : m_pStairsObjectBack)
		{
			if (Raycast(stair->GetStairsVertices(), rayP1, rayP2, hitInfo))
			{
				return stair->GetDirectionUpStair();
			}
		}
	}
	else
	{
		for (const Stair* stair : m_pStairsObject)
		{
			if (Raycast(stair->GetStairsVertices(), rayP1, rayP2, hitInfo))
			{
				return stair->GetDirectionUpStair();
			}
		}
	}
	return utils::Direction::down;
}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}

bool Level::HasReachEnd(const Rectf& actorHitBox) const
{
	const Point2f rayP1Vertical{actorHitBox.left + actorHitBox.width / 2, actorHitBox.bottom};
	const Point2f rayP2Vertical{actorHitBox.left + actorHitBox.width / 2, actorHitBox.bottom + actorHitBox.height};

	const Point2f rayP1Horizontal{actorHitBox.left, actorHitBox.bottom + actorHitBox.height / 2};
	const Point2f rayP2Horizontal{actorHitBox.left + actorHitBox.width, actorHitBox.bottom + actorHitBox.height / 2};
	utils::HitInfo hitInfo;
	for (const std::vector<Point2f>& vertices : m_EndBoundaries)
	{
		if (Raycast(vertices, rayP1Vertical, rayP2Vertical, hitInfo) || Raycast(
			vertices, rayP1Horizontal, rayP2Horizontal, hitInfo))
		{
			return true;
		}
	}
	return false;
}

void Level::ChangeBlock()
{
	// Last block reload the previous one
	m_PreviousBlock = m_CurrentBlock;
	if (m_CurrentBlock == 7)
	{
		--m_CurrentBlock;
	}
	else
	{
		++m_CurrentBlock;
	}

	ClearLevel();
	LoadLevel();
}

void Level::HandleDoors(const Rectf& actorHitBox)
{
	const Point2f centerActor{utils::GetCenterOfRectangle(actorHitBox)};
	for (const std::vector<Point2f>& doors : m_Doors)
	{
		if (utils::IsPointInPolygon(centerActor, doors))
		{
			m_PlayerInBack = !m_PlayerInBack;
		}
	}
}

unsigned Level::GetCurrentBlock() const
{
	return m_CurrentBlock;
}

bool Level::IsPLayerInBack() const
{
	return m_PlayerInBack;
}


void Level::ClearLevel()
{
	// Clear all textures
	DeleteTextures();

	// Clear all vertices
	ClearVertices();

	// Clear offset of background
	m_OffsetXFirstBackground = 0;
	m_OffsetXSecondBackground = 0;
	m_OffsetXThirdBackground = 0;
	m_OffsetXFourthBackground = 0;
	m_PlayerInBack = false;
}

void Level::ResetCurrentBlock()
{
	m_CurrentBlock = 2;
	ClearLevel();
	LoadLevel();
}

Point2f Level::HitHooks(const Rectf& weaponShape) const
{
	for (Hook* hooks : m_pHooks)
	{
		if (hooks->IsHit(weaponShape))
		{
			return hooks->GetCenterHook();
		}
	}
	return Point2f{-1, -1};
}

float Level::GetAngleFromHook(const Point2f& centerActor, const Point2f& centerHook) const
{
	if (m_CurrentBlock != 4)
	{
		return -1.f;
	}
	const Vector2f vectorActorToHook{centerHook, centerActor};
	const float angle{atan2f(vectorActorToHook.y, vectorActorToHook.x)};
	return angle;
}

unsigned Level::GetPreviousBlock() const
{
	return m_PreviousBlock;
}

void Level::LoadHooks()
{
	if (m_CurrentBlock == 4 && !m_Hooks.empty())
	{
		for (const std::vector<Point2f>& hooks : m_Hooks)
		{
			m_pHooks.push_back(new Hook{hooks});
		}
	}
}

void Level::LoadLevel()
{
	// Load Textures
	LoadBackgroundTextures();
	LoadForegroundTexture();
	LoadOffsetY();
	// Load vertices
	LoadVerticesLevel();
	// Add to object stairs
	LoadStairs();
	LoadHooks();
	// Set boundaries
	SetBoundaries();
}

void Level::LoadBackgroundTextures()
{
	switch (m_CurrentBlock)
	{
	case 1:
	case 2:
		m_pFirstBackground = new Texture{
			"Levels/" + std::to_string(m_CurrentLevel) + "/" + std::to_string(m_CurrentBlock) + "/Background.png"
		};
		break;
	case 3:
		LoadMultipleBackgroundTextures(2);
		break;
	case 4:
		LoadMultipleBackgroundTextures(4);
		break;
	case 5:
		LoadMultipleBackgroundTextures(2);
		break;
	case 6:
		LoadMultipleBackgroundTextures(2);
		break;
	case 7:
		LoadMultipleBackgroundTextures(2);
		break;
	}
}

void Level::LoadMultipleBackgroundTextures(size_t size)
{
	for (size_t i{1}; i <= size; ++i)
	{
		switch (i)
		{
		case 1:
			m_pFirstBackground = new Texture{
				"Levels/" + std::to_string(m_CurrentLevel) + "/" + std::to_string(m_CurrentBlock) + "/Background" +
				std::to_string(i) + ".png"
			};
			break;
		case 2:
			m_pSecondBackground = new Texture{
				"Levels/" + std::to_string(m_CurrentLevel) + "/" + std::to_string(m_CurrentBlock) + "/Background" +
				std::to_string(i) + ".png"
			};
			break;
		case 3:
			m_pThirdBackground = new Texture{
				"Levels/" + std::to_string(m_CurrentLevel) + "/" + std::to_string(m_CurrentBlock) + "/Background" +
				std::to_string(i) + ".png"
			};
			break;
		case 4:
			m_pFourthBackground = new Texture{
				"Levels/" + std::to_string(m_CurrentLevel) + "/" + std::to_string(m_CurrentBlock) + "/Background" +
				std::to_string(i) + ".png"
			};
			break;
		}
	}
}

void Level::LoadStairs()
{
	if (m_CurrentBlock == 2)
	{
		for (const std::vector<Point2f>& stairs : m_Stairs)
		{
			m_pStairsObject.push_back(new Stair{stairs});
		}
		for (const std::vector<Point2f>& stairs : m_StairsBack)
		{
			m_pStairsObjectBack.push_back(new Stair{stairs});
		}
	}
	else
	{
		for (const std::vector<Point2f>& stairs : m_Stairs)
		{
			m_pStairsObject.push_back(new Stair{stairs});
		}
	}
}

void Level::ClampVertical(Point2f& bottomLeftShape, const Rectf& actorShape, const utils::HitInfo& hitInfo,
                          Vector2f& actorVelocity) const
{
	const Point2f hitPoint{hitInfo.intersectPoint};
	const Point2f middlePoint{bottomLeftShape.x + actorShape.width / 2, bottomLeftShape.y + actorShape.height / 2};
	if (hitPoint.y < middlePoint.y)
	{
		bottomLeftShape.y = hitPoint.y;
		actorVelocity.y = 0.f;
	}
	if (hitPoint.y > middlePoint.y)
	{
		bottomLeftShape.y = hitPoint.y - actorShape.height;
	}
}

void Level::ClampHorizontal(Point2f& bottomLeftShape, const Rectf& actorShape, const utils::HitInfo& hitInfo,
                            Vector2f& actorVelocity) const
{
	const Point2f hitPoint{hitInfo.intersectPoint};
	const Point2f middlePoint{bottomLeftShape.x + actorShape.width / 2, bottomLeftShape.y + actorShape.height / 2};
	// divide in 3 parts 
	if (hitPoint.x < middlePoint.x)
	{
		bottomLeftShape.x = hitPoint.x;
		actorVelocity.x = 0.f;
	}
	if (hitPoint.x > middlePoint.x)
	{
		bottomLeftShape.x = hitPoint.x - actorShape.width;
	}
}

void Level::LoadOffsetY()
{
	switch (m_CurrentBlock)
	{
	case 1:
		m_StartOffsetY = 27.f;
		break;
	case 2:
		m_StartOffsetY = 16.f;
		break;
	case 3:
	case 5:
		m_StartOffsetY = 20.f;
		break;
	case 4:
		m_StartOffsetY = 0.f;
		break;
	case 6:
		m_StartOffsetY = 40.f;
		break;
	case 7:
		m_StartOffsetY = 75.f;
		break;
	}
}

void Level::HandleOffsetX(const Point2f& posCamera, const Rectf& boundariesCamera)
{
	const float differenceWidthFirst{m_Boundaries.width - m_pFirstBackground->GetWidth()};
	const float percentageProgressXFirst = posCamera.x / boundariesCamera.width;
	const float newXValueBackgroundFirst = differenceWidthFirst * percentageProgressXFirst;
	m_OffsetXFirstBackground = newXValueBackgroundFirst;

	if (m_pSecondBackground != nullptr)
	{
		const float differenceWidthSecond{m_Boundaries.width - m_pSecondBackground->GetWidth()};
		const float percentageProgressXSecond = posCamera.x / boundariesCamera.width;
		const float newXValueBackgroundSecond = differenceWidthSecond * percentageProgressXSecond;
		m_OffsetXSecondBackground = newXValueBackgroundSecond;
	}
	if (m_pThirdBackground != nullptr)
	{
		const float differenceWidthThird{m_Boundaries.width - m_pThirdBackground->GetWidth()};
		const float percentageProgressXThird = posCamera.x / boundariesCamera.width;
		const float newXValueBackgroundThird = differenceWidthThird * percentageProgressXThird;
		m_OffsetXThirdBackground = newXValueBackgroundThird;
	}
	if (m_pFourthBackground != nullptr)
	{
		const float differenceWidthFourth{m_Boundaries.width - m_pFourthBackground->GetWidth()};
		const float percentageProgressXFourth = posCamera.x / boundariesCamera.width;
		const float newXValueBackgroundFourth = percentageProgressXFourth * differenceWidthFourth;
		m_OffsetXFourthBackground = newXValueBackgroundFourth;
	}
}

void Level::DrawTexturesBackground() const
{
	float offsetY{m_StartOffsetY};
	const Rectf destRectFirst{
		m_OffsetXFirstBackground, offsetY, m_pFirstBackground->GetWidth(), m_pFirstBackground->GetHeight()
	};
	m_pFirstBackground->Draw(destRectFirst);
	if (m_pSecondBackground != nullptr)
	{
		offsetY += m_pFirstBackground->GetHeight();
		const Rectf destRectSecond{
			m_OffsetXSecondBackground, offsetY, m_pSecondBackground->GetWidth(), m_pSecondBackground->GetHeight()
		};
		m_pSecondBackground->Draw(destRectSecond);
	}
	if (m_pThirdBackground != nullptr)
	{
		offsetY += m_pSecondBackground->GetHeight();
		const Rectf destRectSecond{
			m_OffsetXThirdBackground, offsetY, m_pThirdBackground->GetWidth(), m_pThirdBackground->GetHeight()
		};
		m_pThirdBackground->Draw(destRectSecond);
	}
	if (m_pFourthBackground != nullptr)
	{
		offsetY += m_pThirdBackground->GetHeight();
		const Rectf destRectSecond{
			m_OffsetXFourthBackground, offsetY, m_pFourthBackground->GetWidth(), m_pFourthBackground->GetHeight()
		};
		m_pFourthBackground->Draw(destRectSecond);
	}
}

void Level::LoadVerticesLevel()
{
	m_VerticesGround.clear();
	m_Stairs.clear();
	if (m_CurrentBlock == 2)
	{
		m_VerticesGroundBack.clear();
		m_StairsBack.clear();
		SVGParser::GetVerticesFromSvgFile(
			"Levels/" + std::to_string(m_CurrentLevel) + "/" + std::to_string(m_CurrentBlock) + "/ForegroundGround.svg",
			m_VerticesGround);
		SVGParser::GetVerticesFromSvgFile(
			"Levels/" + std::to_string(m_CurrentLevel) + "/" + std::to_string(m_CurrentBlock) + "/ForegroundStairs.svg",
			m_Stairs);
		SVGParser::GetVerticesFromSvgFile(
			"Levels/" + std::to_string(m_CurrentLevel) + "/" + std::to_string(m_CurrentBlock) + "/BackgroundGround.svg",
			m_VerticesGroundBack);
		SVGParser::GetVerticesFromSvgFile(
			"Levels/" + std::to_string(m_CurrentLevel) + "/" + std::to_string(m_CurrentBlock) + "/BackgroundStairs.svg",
			m_StairsBack);
		SVGParser::GetVerticesFromSvgFile(
			"Levels/" + std::to_string(m_CurrentLevel) + "/" + std::to_string(m_CurrentBlock) + "/Doors.svg", m_Doors);
	}
	else if (m_CurrentBlock == 4)
	{
		SVGParser::GetVerticesFromSvgFile(
			"Levels/" + std::to_string(m_CurrentLevel) + "/" + std::to_string(m_CurrentBlock) + "/Foreground.svg",
			m_VerticesGround);
		SVGParser::GetVerticesFromSvgFile(
			"Levels/" + std::to_string(m_CurrentLevel) + "/" + std::to_string(m_CurrentBlock) + "/Stairs.svg",
			m_Stairs);
		SVGParser::GetVerticesFromSvgFile(
			"Levels/" + std::to_string(m_CurrentLevel) + "/" + std::to_string(m_CurrentBlock) + "/Hook.svg",
			m_Hooks);
	}
	else
	{
		SVGParser::GetVerticesFromSvgFile(
			"Levels/" + std::to_string(m_CurrentLevel) + "/" + std::to_string(m_CurrentBlock) + "/Foreground.svg",
			m_VerticesGround);
		SVGParser::GetVerticesFromSvgFile(
			"Levels/" + std::to_string(m_CurrentLevel) + "/" + std::to_string(m_CurrentBlock) + "/Stairs.svg",
			m_Stairs);
	}
}

void Level::DeleteTextures()
{
	delete m_pFirstBackground;
	m_pFirstBackground = nullptr;
	delete m_pSecondBackground;
	m_pSecondBackground = nullptr;
	delete m_pThirdBackground;
	m_pThirdBackground = nullptr;
	delete m_pFourthBackground;
	m_pFourthBackground = nullptr;

	for (const Stair* stair : m_pStairsObject)
	{
		delete stair;
		stair = nullptr;
	}
	for (const Stair* stair : m_pStairsObjectBack)
	{
		delete stair;
		stair = nullptr;
	}
	for (const Hook* hook : m_pHooks)
	{
		delete hook;
		hook = nullptr;
	}
	m_pHooks.clear();
	m_pStairsObject.clear();
	m_pStairsObjectBack.clear();
	delete m_pCurrentForeground;
	m_pCurrentForeground = nullptr;
}

void Level::ClearVertices()
{
	m_VerticesGroundBack.clear();
	m_Stairs.clear();
	m_StairsBack.clear();
	m_VerticesGround.clear();
	m_EndBoundaries.clear();
	m_Doors.clear();
}

void Level::SetBoundaries()
{
	m_Boundaries = Rectf{0.f, 0.f, m_pCurrentForeground->GetWidth(), m_pCurrentForeground->GetHeight()};
	SVGParser::GetVerticesFromSvgFile(
		"Levels/" + std::to_string(m_CurrentLevel) + "/" + std::to_string(m_CurrentBlock) + "/End.svg",
		m_EndBoundaries);
}

void Level::LoadForegroundTexture()
{
	m_pCurrentForeground = new Texture{
		"Levels/" + std::to_string(m_CurrentLevel) + "/" + std::to_string(m_CurrentBlock) + "/Foreground.png"
	};
	if (!m_pCurrentForeground->IsCreationOk())
	{
		std::cout << "Can't load levels\n";
		exit(0);
	}
}
