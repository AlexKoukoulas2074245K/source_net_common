///------------------------------------------------------------------------------------------------
///  NetworkObjectDefinitions.inc
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 30/04/2024
///------------------------------------------------------------------------------------------------

///-----------------------------------------------------------------------------------------------

BEGIN_SERIALIZABLE(PlayRequest)
END_SERIALIZABLE()

///-----------------------------------------------------------------------------------------------

BEGIN_SERIALIZABLE(PlayResponse)
VALUE(long long, playerId)
VALUE(long long, tableId)
VALUE(bool, allowed)
END_SERIALIZABLE()

///-----------------------------------------------------------------------------------------------

BEGIN_SERIALIZABLE(TableStateRequest)
VALUE(long long, playerId)
VALUE(long long, tableId)
END_SERIALIZABLE()

///-----------------------------------------------------------------------------------------------

BEGIN_SERIALIZABLE(TableStateResponse)
VALUE(std::string, roundStateName)
VALUE(std::string, communityCards)
VALUE(std::vector<long long>, holeCardPlayerIds)
VALUE(std::vector<std::string>, holeCards)
END_SERIALIZABLE()

///-----------------------------------------------------------------------------------------------
