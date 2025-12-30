///------------------------------------------------------------------------------------------------
///  NetworkMessages.inc
///  TinyMMOCommon
///
///  Created by Alex Koukoulas on 26/12/2025
///------------------------------------------------------------------------------------------------

#ifndef NETWORK_MESSAGES_H
#define NETWORK_MESSAGES_H

///------------------------------------------------------------------------------------------------

#include <cstdint>
#include <enet/enet.h>
#include <net_common/Version.h>

#if __has_include(<engine/utils/MathUtils.h>)
#include <engine/utils/MathUtils.h>
#else
#include "../util/MathUtils.h"
#endif

///------------------------------------------------------------------------------------------------
/// Define Message Types
#define BEGIN_MESSAGE(messageName) messageName,
#define FIELD(name, type)
#define END_MESSAGE()

enum class MessageType : uint8_t
{
#include <net_common/NetworkMessages.inc>
    UNUSED
};

#undef BEGIN_MESSAGE
#undef FIELD
#undef END_MESSAGE

#pragma pack(push, 1)
struct MessageHeader {
    MessageType type;
    char version[16];
};
#pragma pack(pop)

#define BEGIN_MESSAGE(messageName) struct messageName { MessageHeader __header { MessageType::messageName, NET_COMMON_VERSION };
#define FIELD(name, type) type name;
#define END_MESSAGE() };

#include <net_common/NetworkMessages.inc>

namespace channels
{
    static constexpr enet_uint32 UNRELIABLE = 0;
    static constexpr enet_uint32 RELIABLE = 1;
};

inline void SendMessage(ENetPeer* toPeer, const void* message, const size_t messageSize, const enet_uint32 channel)
{
    ENetPacket* enetPacket = enet_packet_create(message, messageSize, channel);
    enet_peer_send(toPeer, channel, enetPacket);
}

inline void BroadcastMessage(ENetHost* server, const void* message, const size_t messageSize, const enet_uint32 channel)
{
    ENetPacket* enetPacket = enet_packet_create(message, messageSize, channel);
    enet_host_broadcast(server, channel, enetPacket);
}

enum class MessageVersionValidityEnum
{
    VALID,
    INCOMING_MESSAGE_BEHIND_IN_VERSION,
    INCOMING_MESSAGE_AHEAD_IN_VERSION
};

inline MessageVersionValidityEnum GetMessageVersionValidity(unsigned char* rawMessageData)
{
    char incomingVersion[16];
    memcpy(incomingVersion, &rawMessageData[1], 16);
    
    auto cmpResult = strcmp(incomingVersion, NET_COMMON_VERSION);
    
    if (!cmpResult)
    {
        return MessageVersionValidityEnum::VALID;
    }
    else if (cmpResult < 0)
    {
        return MessageVersionValidityEnum::INCOMING_MESSAGE_BEHIND_IN_VERSION;
    }
    else
    {
        return MessageVersionValidityEnum::INCOMING_MESSAGE_AHEAD_IN_VERSION;
    }
}

inline const char* GetMessageVersionValidityString(const MessageVersionValidityEnum value)
{
    switch (value)
    {
        case MessageVersionValidityEnum::VALID:
        {
            return "VALID";
        } break;
        
        case MessageVersionValidityEnum::INCOMING_MESSAGE_BEHIND_IN_VERSION:
        {
            return "INCOMING_MESSAGE_BEHIND_IN_VERSION";
        } break;
            
        case MessageVersionValidityEnum::INCOMING_MESSAGE_AHEAD_IN_VERSION:
        {
            return "INCOMING_MESSAGE_AHEAD_IN_VERSION";
        } break;
    }
}

#endif // NETWORK_MESSAGES_H
