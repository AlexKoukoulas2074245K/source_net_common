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
    // For circle colliders x and y are set to the radius.
    glm::vec2 colliderRelativeDimentions;
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

}

///------------------------------------------------------------------------------------------------

#endif // NETWORK_COMMON_H
