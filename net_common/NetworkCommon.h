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
    SOUTH,
    NORTH,
    WEST,
    EAST,
    NORTH_WEST,
    NORTH_EAST,
    SOUTH_WEST,
    SOUTH_EAST
};

enum class AnimationType
{
    IDLE,
    RUNNING
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
    ObjectType objectType;
    AttackType attackType;
    ProjectileType projectileType;
    FacingDirection facingDirection;
    AnimationType currentAnimation;
    ObjectColliderData colliderData;
    glm::vec3 position;
    glm::vec3 velocity;
    float speed;
    char currentMap[64] = {};
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

}

///------------------------------------------------------------------------------------------------

#endif // NETWORK_COMMON_H
