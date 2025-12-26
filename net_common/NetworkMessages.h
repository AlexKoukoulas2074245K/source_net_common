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
#include <engine/utils/MathUtils.h>
#include <net_common/Version.h>

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
    char version[16];
    MessageType type;
};

#define BEGIN_MESSAGE(messageName) struct messageName { MessageHeader header { NET_COMMON_VERSION, MessageType::messageName };
#define FIELD(name, type) type name;
#define END_MESSAGE() };

#include <net_common/NetworkMessages.inc>

#pragma pack(pop)
#endif // NETWORK_MESSAGES_H
