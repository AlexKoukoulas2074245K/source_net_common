///------------------------------------------------------------------------------------------------
///  SymbolDataRepository.cpp
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 28/02/2025
///------------------------------------------------------------------------------------------------

#include "SymbolDataRepository.h"
#include "Board.h"

#include <cassert>

///------------------------------------------------------------------------------------------------

namespace slots
{

///------------------------------------------------------------------------------------------------

float SymbolDataRepository::GetSymbolWinMultiplier(const SymbolType symbolType, const int matchingCount) const
{
    assert(matchingCount < mSymbolWinMultipliers.size());
    return mSymbolWinMultipliers.at(matchingCount).at(symbolType);
}

///------------------------------------------------------------------------------------------------

SymbolDataRepository& SymbolDataRepository::GetInstance()
{
    static SymbolDataRepository instance;
    return instance;
}

///------------------------------------------------------------------------------------------------

SymbolDataRepository::SymbolDataRepository()
{
    InitWithDefaultConfig();
}

///------------------------------------------------------------------------------------------------

void SymbolDataRepository::InitWithDefaultConfig()
{
    mSymbolWinMultipliers.resize(BOARD_COLS);
    
    // 3x matches
    mSymbolWinMultipliers[2][SymbolType::BUTTER]          = 0.5f;
    mSymbolWinMultipliers[2][SymbolType::CAMP_FIRE]       = 0.5f;
    mSymbolWinMultipliers[2][SymbolType::CHICKEN]         = 2.0f;
    mSymbolWinMultipliers[2][SymbolType::CHOCOLATE]       = 1.0f;
    mSymbolWinMultipliers[2][SymbolType::COOKING_OIL]     = 0.5f;
    mSymbolWinMultipliers[2][SymbolType::EGGS]            = 0.5f;
    mSymbolWinMultipliers[2][SymbolType::FLOUR]           = 0.5f;
    mSymbolWinMultipliers[2][SymbolType::GARLICS]         = 0.5f;
    mSymbolWinMultipliers[2][SymbolType::LEMONS]          = 0.5f;
    mSymbolWinMultipliers[2][SymbolType::STRAWBERRIES]    = 0.5f;
    mSymbolWinMultipliers[2][SymbolType::SUGAR]           = 1.0f;
    mSymbolWinMultipliers[2][SymbolType::CHOCOLATE_CAKE]  = 10.0f;
    mSymbolWinMultipliers[2][SymbolType::STRAWBERRY_CAKE] = 10.0f;
    mSymbolWinMultipliers[2][SymbolType::ROAST_CHICKEN]   = 7.5f;
    mSymbolWinMultipliers[2][SymbolType::SCATTER]         = 1.0f;

    // 4x matches
    mSymbolWinMultipliers[3][SymbolType::BUTTER]          = 1.5f;
    mSymbolWinMultipliers[3][SymbolType::CAMP_FIRE]       = 1.5f;
    mSymbolWinMultipliers[3][SymbolType::CHICKEN]         = 4.5f;
    mSymbolWinMultipliers[3][SymbolType::CHOCOLATE]       = 3.0f;
    mSymbolWinMultipliers[3][SymbolType::COOKING_OIL]     = 1.5f;
    mSymbolWinMultipliers[3][SymbolType::EGGS]            = 1.5f;
    mSymbolWinMultipliers[3][SymbolType::FLOUR]           = 1.5f;
    mSymbolWinMultipliers[3][SymbolType::GARLICS]         = 1.5f;
    mSymbolWinMultipliers[3][SymbolType::LEMONS]          = 1.5f;
    mSymbolWinMultipliers[3][SymbolType::STRAWBERRIES]    = 1.5f;
    mSymbolWinMultipliers[3][SymbolType::SUGAR]           = 3.0f;
    mSymbolWinMultipliers[3][SymbolType::CHOCOLATE_CAKE]  = 30.0f;
    mSymbolWinMultipliers[3][SymbolType::STRAWBERRY_CAKE] = 30.0f;
    mSymbolWinMultipliers[3][SymbolType::ROAST_CHICKEN]   = 21.5f;
    mSymbolWinMultipliers[3][SymbolType::SCATTER]         = 3.0f;
    
    // 5x matches
    mSymbolWinMultipliers[4][SymbolType::BUTTER]          = 4.5f;
    mSymbolWinMultipliers[4][SymbolType::CAMP_FIRE]       = 4.5f;
    mSymbolWinMultipliers[4][SymbolType::CHICKEN]         = 12.0f;
    mSymbolWinMultipliers[4][SymbolType::CHOCOLATE]       = 6.0f;
    mSymbolWinMultipliers[4][SymbolType::COOKING_OIL]     = 4.5f;
    mSymbolWinMultipliers[4][SymbolType::EGGS]            = 4.5f;
    mSymbolWinMultipliers[4][SymbolType::FLOUR]           = 4.5f;
    mSymbolWinMultipliers[4][SymbolType::GARLICS]         = 4.5f;
    mSymbolWinMultipliers[4][SymbolType::LEMONS]          = 4.5f;
    mSymbolWinMultipliers[4][SymbolType::STRAWBERRIES]    = 4.5f;
    mSymbolWinMultipliers[4][SymbolType::SUGAR]           = 9.0f;
    mSymbolWinMultipliers[4][SymbolType::CHOCOLATE_CAKE]  = 90.0f;
    mSymbolWinMultipliers[4][SymbolType::STRAWBERRY_CAKE] = 90.0f;
    mSymbolWinMultipliers[4][SymbolType::ROAST_CHICKEN]   = 60.0f;
    mSymbolWinMultipliers[4][SymbolType::SCATTER]         = 9.0f;
}

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------


