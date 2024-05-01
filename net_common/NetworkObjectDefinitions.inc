///------------------------------------------------------------------------------------------------
///  NetObjects.h
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 30/04/2024
///------------------------------------------------------------------------------------------------

///------------------------------------------------------------------------------------------------

BEGIN_SERIALIZABLE(WorldObjectData)
VALUE(long long, objectId)
VALUE(long long, parentObjectId)
VALUE(strutils::StringId, objectName)
VALUE(glm::vec3, objectPosition)
VALUE(glm::vec3, objectVelocity)
VALUE(float, color)
VALUE(int, objectType)
VALUE(bool, isLocal)
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
