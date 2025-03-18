///------------------------------------------------------------------------------------------------
///  NetworkMessages.h
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 30/04/2024
///------------------------------------------------------------------------------------------------

#ifndef NetworkMessages_h
#define NetworkMessages_h

///------------------------------------------------------------------------------------------------

#if __has_include(<engine/utils/MathUtils.h>)
#include <engine/utils/MathUtils.h>
#else
#include "../util/MathUtils.h"
#endif

#include <string>

///------------------------------------------------------------------------------------------------

namespace networking
{

///------------------------------------------------------------------------------------------------

enum class MessagePriority
{
    NORMAL = 0,
    HIGH
};

///------------------------------------------------------------------------------------------------

enum class MessageType
{
    UNKNOWN_MESSAGE = 0,
    CS_LOGIN_REQUEST,
    SC_LOGIN_RESPONSE,
    CS_SPIN_REQUEST,
    SC_SPIN_RESPONSE
};

///-----------------------------------------------------------------------------------------------

struct ServerResponseData
{
    std::string mResponse;
    std::string mError;
    long long mPingMillis;
};

///-----------------------------------------------------------------------------------------------

inline void PopulateMessageHeader(nlohmann::json& messageJson, const MessageType messageType)
{
    messageJson["messageType"] = static_cast<int>(messageType);
}

///-----------------------------------------------------------------------------------------------

inline bool IsMessageOfType(const nlohmann::json& messageJson, const MessageType messageType)
{
    return messageJson.count("messageType") && static_cast<MessageType>(messageJson.at("messageType").get<int>()) == messageType;
}

///-----------------------------------------------------------------------------------------------

inline MessageType GetMessageType(const nlohmann::json& messageJson)
{
    if (messageJson.count("messageType"))
    {
        return static_cast<MessageType>(messageJson.at("messageType").get<int>());
    }
    
    return MessageType::UNKNOWN_MESSAGE;
}

///-----------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

#endif /* NetworkMessages_h */
