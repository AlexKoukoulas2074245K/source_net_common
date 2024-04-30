///------------------------------------------------------------------------------------------------
///  NetObjects.h
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 30/04/2024
///------------------------------------------------------------------------------------------------

///------------------------------------------------------------------------------------------------

BEGIN_SERIALIZABLE(PlayerData)
VALUE(strutils::StringId, playerName)
VALUE(glm::vec3, playerPosition)
VALUE(glm::vec3, playerVelocity)
VALUE(float, color)
VALUE(bool, isLocal)
VALUE(bool, invalidated)
END_SERIALIZABLE()

///-----------------------------------------------------------------------------------------------

BEGIN_SERIALIZABLE(LoginRequest)
END_SERIALIZABLE()

///-----------------------------------------------------------------------------------------------

BEGIN_SERIALIZABLE(LoginResponse)
VALUE(strutils::StringId, playerName)
VALUE(glm::vec3, playerPosition)
VALUE(float, color)
VALUE(bool, allowed)
END_SERIALIZABLE()

///-----------------------------------------------------------------------------------------------
