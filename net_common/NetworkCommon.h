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
    NORTH_WEST,
    NORTH_EAST,
    SOUTH_WEST,
    SOUTH_EAST,
    NORTH,
    SOUTH,
    WEST,
    EAST
};

enum class AnimationType
{
    IDLE,
    RUNNING
};

///------------------------------------------------------------------------------------------------

using objectId_t = uint64_t;

///------------------------------------------------------------------------------------------------

struct ObjectData
{
    objectId_t objectId;
    ObjectType objectType;
    AttackType attackType;
    ProjectileType projectileType;
    FacingDirection facingDirection;
    AnimationType currentAnimation;
    glm::vec3 position;
    glm::vec3 velocity;
    float speed;
};

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

#endif // NETWORK_COMMON_H
