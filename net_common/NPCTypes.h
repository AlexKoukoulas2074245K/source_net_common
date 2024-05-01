///------------------------------------------------------------------------------------------------
///  NPCTypes.h
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 01/05/2024
///------------------------------------------------------------------------------------------------

#ifndef NPCTypes_h
#define NPCTypes_h

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

inline constexpr int NPC_TYPE_UNKNOWN_NPC = 0;
inline constexpr int NPC_TYPE_SHURIKEN    = 1;

///-----------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

#endif /* NPCTypes_h */
