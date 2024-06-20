///------------------------------------------------------------------------------------------------
///  NetworkObjectDefinitions.inc
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 30/04/2024
///------------------------------------------------------------------------------------------------

///-----------------------------------------------------------------------------------------------

BEGIN_SERIALIZABLE(LoginRequest)
END_SERIALIZABLE()

///-----------------------------------------------------------------------------------------------

BEGIN_SERIALIZABLE(LoginResponse)
VALUE(long long, playerId)
VALUE(strutils::StringId, playerName)
VALUE(bool, allowed)
END_SERIALIZABLE()

///-----------------------------------------------------------------------------------------------

BEGIN_SERIALIZABLE(WordRequest)
VALUE(std::string, sourceLanguge)
VALUE(std::string, targetLanguage)
END_SERIALIZABLE()

///-----------------------------------------------------------------------------------------------

BEGIN_SERIALIZABLE(WordResponse)
VALUE(std::string, sourceWord)
VALUE(std::vector<std::string>, choices)
VALUE(bool, allowed)
END_SERIALIZABLE()

///-----------------------------------------------------------------------------------------------
