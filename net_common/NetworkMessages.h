///------------------------------------------------------------------------------------------------
///  NetworkMessages.h
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 30/04/2024
///------------------------------------------------------------------------------------------------

#ifndef NetworkMessages_h
#define NetworkMessages_h

///------------------------------------------------------------------------------------------------

#if __has_include(<nlohmann/json.hpp>)
#include <nlohmann/json.hpp>
#else
#include "../util/Json.h"
#endif

///------------------------------------------------------------------------------------------------

namespace networking
{

///------------------------------------------------------------------------------------------------

enum class MessageType
{
    UNKNOWN_MESSAGE           = 0,
    CS_PLAYER_STATE           = 1,
    SC_PLAYER_STATE_RESPONSE  = 2,
    CS_REQUEST_LOGIN          = 3,
    SC_REQUEST_LOGIN_RESPONSE = 4
};

///-----------------------------------------------------------------------------------------------

void PopulateMessageHeader(nlohmann::json& messageJson, const MessageType messageType);
bool IsMessageOfType(const nlohmann::json& messageJson, const MessageType messageType);
MessageType GetMessageType(const nlohmann::json& messageJson);

///-----------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

#endif /* NetworkMessages_h */