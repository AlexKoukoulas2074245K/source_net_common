///------------------------------------------------------------------------------------------------
///  NetworkMessages.cpp
///  TinyMMOCommon
///
///  Created by Alex Koukoulas on 26/12/2025
///------------------------------------------------------------------------------------------------

#include <net_common/NetworkMessages.h>
#include <cstdlib>

///------------------------------------------------------------------------------------------------

MessageValidityEnum GetMessageValidity(unsigned char* rawMessageData)
{
    char incomingVersion[16];
    memcpy(rawMessageData[1], incomingVersion, 16);
    
    auto cmpResult = strcmp(incomingVersion, NET_COMMON_VERSION);
    
    if (!cmpResult)
    {
        return MessageValidityEnum::VALID;
    }
    else if (cmpResult < 0)
    {
        return MessageValidityEnum::INCOMING_MESSAGE_BEHIND_IN_VERSION;
    }
    else
    {
        return MessageValidityEnum::INCOMING_MESSAGE_AHEAD_IN_VERSION;
    }
}

///------------------------------------------------------------------------------------------------

const char* GetMessageValidityString(const MessageValidityEnum value)
{
    switch (value)
    {
        case MessageValidityEnum::VALID:
        {
            static const char * VALID_STRING = "VALID";
            return VALID_STRING;
        } break;
        
        case MessageValidityEnum::INCOMING_MESSAGE_BEHIND_IN_VERSION:
        {
            static const char * INCOMING_MESSAGE_BEHIND_IN_VERSION_STRING = "INCOMING_MESSAGE_BEHIND_IN_VERSION";
            return INCOMING_MESSAGE_BEHIND_IN_VERSION_STRING;
        } break;
            
        case MessageValidityEnum::INCOMING_MESSAGE_AHEAD_IN_VERSION:
        {
            static const char * INCOMING_MESSAGE_AHEAD_IN_VERSION_STRING = "INCOMING_MESSAGE_AHEAD_IN_VERSION";
            return INCOMING_MESSAGE_AHEAD_IN_VERSION_STRING;
        } break;
    }
}

///------------------------------------------------------------------------------------------------
