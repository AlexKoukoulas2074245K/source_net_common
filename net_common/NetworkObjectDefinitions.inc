///------------------------------------------------------------------------------------------------
///  NetObjects.h
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 30/04/2024
///------------------------------------------------------------------------------------------------

///------------------------------------------------------------------------------------------------

BEGIN_SERIALIZABLE(PlayerData)
VALUE(long long, playerId)
VALUE(strutils::StringId, playerName)
VALUE(glm::vec3, playerPosition)
VALUE(glm::vec3, playerVelocity)
VALUE(float, color)
VALUE(bool, isLocal)
VALUE(bool, invalidated)
END_SERIALIZABLE()

///-----------------------------------------------------------------------------------------------

BEGIN_SERIALIZABLE(NPCData)
VALUE(long long, npcId)
VALUE(long long, playerParentId)
VALUE(glm::vec3, npcPosition)
VALUE(glm::vec3, npcVelocity)
VALUE(int, npcType)
VALUE(bool, invalidated)
END_SERIALIZABLE()

///-----------------------------------------------------------------------------------------------

BEGIN_SERIALIZABLE(LoginRequest)
END_SERIALIZABLE()

///-----------------------------------------------------------------------------------------------

BEGIN_SERIALIZABLE(LoginResponse)
VALUE(long long, playerId)
VALUE(strutils::StringId, playerName)
VALUE(glm::vec3, playerPosition)
VALUE(float, color)
VALUE(bool, allowed)
END_SERIALIZABLE()

///-----------------------------------------------------------------------------------------------

BEGIN_SERIALIZABLE(ThrowRangedWeaponRequest)
VALUE(long long, playerId)
VALUE(glm::vec3, targetPosition)
END_SERIALIZABLE()

///-----------------------------------------------------------------------------------------------

BEGIN_SERIALIZABLE(ThrowRangedWeaponResponse)
VALUE(bool, allowed)
END_SERIALIZABLE()

///-----------------------------------------------------------------------------------------------
