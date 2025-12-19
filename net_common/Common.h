#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

enum class MessageType : uint8_t
{
    MOVE,
    SNAPSHOT,
    ATTACK,
    QUEST_COMPLETE
};

struct Vec2
{
    float x;
    float y;
};

#pragma pack(push, 1)

struct MoveMessage
{
    MessageType type;
    uint32_t playerId;
    Vec2 position;
    Vec2 velocity;
};

struct SnapshotMessage
{
    MessageType type;
    uint32_t playerId;
    Vec2 position;
};

struct AttackMessage
{
    MessageType type;
    uint32_t attackerId;
};

struct QuestCompleteMessage
{
    MessageType type;
    uint32_t playerId;
    uint32_t questId;
};

#pragma pack(pop)
#endif // COMMON_H
