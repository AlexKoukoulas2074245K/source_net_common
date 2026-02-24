///------------------------------------------------------------------------------------------------
///  NetworkCommon.inc
///  TinyMMOCommon
///
///  Created by Alex Koukoulas on 30/12/2025
///------------------------------------------------------------------------------------------------

#ifndef NETWORK_COMMON_H
#define NETWORK_COMMON_H

///------------------------------------------------------------------------------------------------

#if __has_include(<engine/utils/MathUtils.h>)
#include <engine/utils/MathUtils.h>
#else
#include "../util/MathUtils.h"
#endif

///------------------------------------------------------------------------------------------------

namespace network
{

///------------------------------------------------------------------------------------------------

inline const int NAVMAP_SIZE = 64;
inline const float MAP_TILE_SIZE = 0.0625f;
inline const float MAP_GAME_SCALE = 4.0f;

///------------------------------------------------------------------------------------------------

enum class ObjectType
{
    PLAYER,
    NPC,
    ATTACK,
    STATIC
};

enum class AttackType
{
    NONE,
    MELEE,
    PROJECTILE
};

enum class ProjectileType
{
    NONE,
    FIREBALL
};

enum class FacingDirection
{
    SOUTH = 0,
    NORTH,
    WEST,
    EAST,
    NORTH_WEST,
    NORTH_EAST,
    SOUTH_WEST,
    SOUTH_EAST
};

enum class ObjectState
{
    IDLE,
    RUNNING,
    BEGIN_MELEE,
    MELEE_ATTACK,
    CASTING
};

enum class ObjectFaction
{
    GOOD,
    EVIL,
    NEUTRAL,
    SPECIAL
};

enum class ColliderType
{
    CIRCLE,
    RECTANGLE
};

///------------------------------------------------------------------------------------------------

using objectId_t = uint64_t;

///------------------------------------------------------------------------------------------------

struct ObjectColliderData
{
    ColliderType colliderType;
    
    // All floats proportional to SO's scale.
    // For circle colliders x and y are set to the diameters.
    glm::vec2 colliderRelativeDimensions;
};

///------------------------------------------------------------------------------------------------

struct ObjectData
{
    objectId_t objectId;
    objectId_t parentObjectId;
    ObjectType objectType;
    AttackType attackType;
    ProjectileType projectileType;
    FacingDirection facingDirection;
    ObjectState objectState;
    ObjectColliderData colliderData;
    ObjectFaction objectFaction;
    glm::vec3 position;
    glm::vec3 velocity;
    float speed;
    float objectScale;
    float actionTimer;
    char currentMap[64] = {};
};

///------------------------------------------------------------------------------------------------

struct DebugQuadtreeRequestData
{
    glm::vec3 debugRectPositions[512] = {};
    glm::vec3 debugRectDimensions[512] = {};
    size_t debugRectCount;
};

///------------------------------------------------------------------------------------------------

struct DebugObjectPathRequestData
{
    glm::vec3 debugPathPositions[128] = {};
    size_t debugPathPositionsCount;
};

///------------------------------------------------------------------------------------------------

inline bool RectToRectIntersectionCheck(const ObjectData& lhs, const ObjectData& rhs)
{
    glm::vec2 lhsTopLeft(lhs.position.x - (lhs.objectScale * lhs.colliderData.colliderRelativeDimensions.x)/2.0f, lhs.position.y + (lhs.objectScale * lhs.colliderData.colliderRelativeDimensions.y)/2.0f);
    glm::vec2 lhsBotRight(lhs.position.x + (lhs.objectScale * lhs.colliderData.colliderRelativeDimensions.x)/2.0f, lhs.position.y - (lhs.objectScale * lhs.colliderData.colliderRelativeDimensions.y)/2.0f);
    
    glm::vec2 rhsTopLeft(rhs.position.x - (rhs.objectScale * rhs.colliderData.colliderRelativeDimensions.x)/2.0f, rhs.position.y + (rhs.objectScale * rhs.colliderData.colliderRelativeDimensions.y)/2.0f);
    glm::vec2 rhsBotRight(rhs.position.x + (rhs.objectScale * rhs.colliderData.colliderRelativeDimensions.x)/2.0f, rhs.position.y - (rhs.objectScale * rhs.colliderData.colliderRelativeDimensions.y)/2.0f);
    
    return !(lhsTopLeft.x > rhsBotRight.x || lhsBotRight.x < rhsTopLeft.x || lhsTopLeft.y < rhsBotRight.y || lhsBotRight.y > rhsTopLeft.y);
}

///------------------------------------------------------------------------------------------------

inline bool RectToCircleIntersectionCheck(const ObjectData& lhs, const ObjectData& rhs)
{
    glm::vec2 lhsTopLeft(lhs.position.x - (lhs.objectScale * lhs.colliderData.colliderRelativeDimensions.x)/2.0f, lhs.position.y + (lhs.objectScale * lhs.colliderData.colliderRelativeDimensions.y)/2.0f);
    
    auto clamp = [](float v, float min, float max) { return math::Max(min, math::Min(v, max)); };
    
    glm::vec2 clamped(clamp(rhs.position.x, lhsTopLeft.x, lhsTopLeft.x + (lhs.objectScale * lhs.colliderData.colliderRelativeDimensions.x)),
                 clamp(rhs.position.y, lhsTopLeft.y - (lhs.objectScale * lhs.colliderData.colliderRelativeDimensions.y), lhsTopLeft.y));
    
    glm::vec2 delta(rhs.position.x - clamped.x, rhs.position.y - clamped.y);
    return delta.x * delta.x + delta.y * delta.y < (rhs.objectScale * rhs.colliderData.colliderRelativeDimensions.x/2.0f) * (rhs.objectScale * rhs.colliderData.colliderRelativeDimensions.y/2.0f);
}

///------------------------------------------------------------------------------------------------

inline bool CircleToCircleIntersectionCheck(const ObjectData& lhs, const ObjectData& rhs)
{
    const auto& centerDistanceSquared = ((lhs.position.x - rhs.position.x) * (lhs.position.x - rhs.position.x) + (lhs.position.y - rhs.position.y) * (lhs.position.y - rhs.position.y));
    const auto& radiusSumSquared = ((lhs.objectScale * lhs.colliderData.colliderRelativeDimensions.x)/2.0f + (rhs.objectScale * rhs.colliderData.colliderRelativeDimensions.x)/2.0f) * ((lhs.objectScale * lhs.colliderData.colliderRelativeDimensions.x)/2.0f + (rhs.objectScale * rhs.colliderData.colliderRelativeDimensions.x)/2.0f);
    return centerDistanceSquared < radiusSumSquared;
}

///------------------------------------------------------------------------------------------------

inline bool CollidersIntersect(const ObjectData& lhs, const ObjectData& rhs)
{
    if (lhs.colliderData.colliderType == ColliderType::RECTANGLE)
    {
        return rhs.colliderData.colliderType == ColliderType::RECTANGLE ? RectToRectIntersectionCheck(lhs, rhs) : RectToCircleIntersectionCheck(lhs, rhs);
    }
    else // Circle
    {
        return rhs.colliderData.colliderType == ColliderType::RECTANGLE ? RectToCircleIntersectionCheck(rhs, lhs) : CircleToCircleIntersectionCheck(lhs, rhs);
    }
}

///------------------------------------------------------------------------------------------------

inline std::string GetCurrentMapString(const ObjectData& objectData)
{
    return std::string(objectData.currentMap);
}

///------------------------------------------------------------------------------------------------

inline void SetCurrentMap(ObjectData& objectData, const std::string& map)
{
    std::strcpy(objectData.currentMap, map.c_str());
}

///------------------------------------------------------------------------------------------------

inline FacingDirection VecToFacingDirection(const glm::vec3& vec)
{
    // make sure dir is not zero-length
    if (glm::length(vec) < 1e-6f) {
        // default or handle error
        return FacingDirection::SOUTH;
    }
    
    // angle in radians: atan2 returns angle from -pi to pi
    float angle = std::atan2(vec.y, vec.x);
    
    // convert to degrees (optional, but easier to reason about)
    float degrees = glm::degrees(angle);
    
    // normalize to [0, 360)
    if (degrees < 0.0f)
        degrees += 360.0f;
    
    // angular sectors: 360/8 = 45 degrees each
    if      (degrees >= 337.5f || degrees < 22.5f) return FacingDirection::EAST;
    else if (degrees < 67.5f)                      return FacingDirection::NORTH_EAST;
    else if (degrees < 112.5f)                     return FacingDirection::NORTH;
    else if (degrees < 157.5f)                     return FacingDirection::NORTH_WEST;
    else if (degrees < 202.5f)                     return FacingDirection::WEST;
    else if (degrees < 247.5f)                     return FacingDirection::SOUTH_WEST;
    else if (degrees < 292.5f)                     return FacingDirection::SOUTH;
    else                                           return FacingDirection::SOUTH_EAST;
}

///------------------------------------------------------------------------------------------------

inline const char* GetFacingDirectionString(const FacingDirection facingDirection)
{
    switch (facingDirection)
    {
        case FacingDirection::SOUTH: return "SOUTH";
        case FacingDirection::NORTH: return "NORTH";
        case FacingDirection::WEST: return "WEST";
        case FacingDirection::EAST: return "EAST";
        case FacingDirection::NORTH_WEST: return "NORTH_WEST";
        case FacingDirection::NORTH_EAST: return "NORTH_EAST";
        case FacingDirection::SOUTH_WEST: return "SOUTH_WEST";
        case FacingDirection::SOUTH_EAST: return "SOUTH_EAST";
    }
}


///------------------------------------------------------------------------------------------------

inline const char* GetObjectTypeString(const ObjectType objectType)
{
    switch (objectType)
    {
        case ObjectType::PLAYER: return "PLAYER";
        case ObjectType::NPC: return "NPC";
        case ObjectType::ATTACK: return "ATTACK";
        case ObjectType::STATIC: return "STATIC";
    }
}

///------------------------------------------------------------------------------------------------

inline const char* GetObjectStateString(const ObjectState objectState)
{
    switch (objectState)
    {
        case ObjectState::IDLE: return "IDLE";
        case ObjectState::RUNNING: return "RUNNING";
        case ObjectState::BEGIN_MELEE: return "BEGIN_MELEE";
        case ObjectState::MELEE_ATTACK: return "MELEE_ATTACK";
        case ObjectState::CASTING: return "CASTING";
    }
}

///------------------------------------------------------------------------------------------------

inline const char* GetObjectFactionString(const ObjectFaction objectFaction)
{
    switch (objectFaction)
    {
        case ObjectFaction::GOOD: return "GOOD";
        case ObjectFaction::EVIL: return "EVIL";
        case ObjectFaction::NEUTRAL: return "NEUTRAL";
        case ObjectFaction::SPECIAL: return "SPECIAL";
    }
}

///------------------------------------------------------------------------------------------------

inline const char* GetAttackTypeString(const AttackType attackType)
{
    switch (attackType)
    {
        case AttackType::NONE: return "NONE";
        case AttackType::MELEE: return "MELEE";
        case AttackType::PROJECTILE: return "PROJECTILE";
    }
}

///------------------------------------------------------------------------------------------------

inline const char* GetProjectileTypeString(const ProjectileType projectileType)
{
    switch (projectileType)
    {
        case ProjectileType::NONE: return "NONE";
        case ProjectileType::FIREBALL: return "FIREBALL";
    }
}

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

#endif // NETWORK_COMMON_H
