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

int SymbolDataRepository::GetSymbolWinMultiplier(const SymbolType symbolType, const int matchingCount) const
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
    
    // 1x matches
    mSymbolWinMultipliers[0][SymbolType::ROAST_CHICKEN]   = 100;
    mSymbolWinMultipliers[0][SymbolType::CHOCOLATE_CAKE]  = 90;
    mSymbolWinMultipliers[0][SymbolType::STRAWBERRY_CAKE] = 90;
    
    // 3x matches
    mSymbolWinMultipliers[2][SymbolType::BUTTER]          = 1;
    mSymbolWinMultipliers[2][SymbolType::CAMP_FIRE]       = 2;
    mSymbolWinMultipliers[2][SymbolType::CHICKEN]         = 2;
    mSymbolWinMultipliers[2][SymbolType::CHOCOLATE]       = 2;
    mSymbolWinMultipliers[2][SymbolType::COOKING_OIL]     = 1;
    mSymbolWinMultipliers[2][SymbolType::EGGS]            = 1;
    mSymbolWinMultipliers[2][SymbolType::FLOUR]           = 1;
    mSymbolWinMultipliers[2][SymbolType::GARLICS]         = 1;
    mSymbolWinMultipliers[2][SymbolType::LEMONS]          = 1;
    mSymbolWinMultipliers[2][SymbolType::STRAWBERRIES]    = 1;
    mSymbolWinMultipliers[2][SymbolType::SUGAR]           = 1;
    mSymbolWinMultipliers[2][SymbolType::CHOCOLATE_CAKE]  = 540;
    mSymbolWinMultipliers[2][SymbolType::STRAWBERRY_CAKE] = 450;
    mSymbolWinMultipliers[2][SymbolType::ROAST_CHICKEN]   = 450;
    mSymbolWinMultipliers[2][SymbolType::SCATTER]         = 5;

    // 4x matches
    mSymbolWinMultipliers[3][SymbolType::BUTTER]          = 2;
    mSymbolWinMultipliers[3][SymbolType::CAMP_FIRE]       = 4;
    mSymbolWinMultipliers[3][SymbolType::CHICKEN]         = 4;
    mSymbolWinMultipliers[3][SymbolType::CHOCOLATE]       = 4;
    mSymbolWinMultipliers[3][SymbolType::COOKING_OIL]     = 2;
    mSymbolWinMultipliers[3][SymbolType::EGGS]            = 2;
    mSymbolWinMultipliers[3][SymbolType::FLOUR]           = 2;
    mSymbolWinMultipliers[3][SymbolType::GARLICS]         = 2;
    mSymbolWinMultipliers[3][SymbolType::LEMONS]          = 2;
    mSymbolWinMultipliers[3][SymbolType::STRAWBERRIES]    = 2;
    mSymbolWinMultipliers[3][SymbolType::SUGAR]           = 2;
    mSymbolWinMultipliers[3][SymbolType::CHOCOLATE_CAKE]  = 1500;
    mSymbolWinMultipliers[3][SymbolType::STRAWBERRY_CAKE] = 1400;
    mSymbolWinMultipliers[3][SymbolType::ROAST_CHICKEN]   = 1400;
    mSymbolWinMultipliers[3][SymbolType::SCATTER]         = 20;
    
    // 5x matches
    mSymbolWinMultipliers[4][SymbolType::BUTTER]          = 6;
    mSymbolWinMultipliers[4][SymbolType::CAMP_FIRE]       = 12;
    mSymbolWinMultipliers[4][SymbolType::CHICKEN]         = 12;
    mSymbolWinMultipliers[4][SymbolType::CHOCOLATE]       = 12;
    mSymbolWinMultipliers[4][SymbolType::COOKING_OIL]     = 6;
    mSymbolWinMultipliers[4][SymbolType::EGGS]            = 6;
    mSymbolWinMultipliers[4][SymbolType::FLOUR]           = 6;
    mSymbolWinMultipliers[4][SymbolType::GARLICS]         = 6;
    mSymbolWinMultipliers[4][SymbolType::LEMONS]          = 6;
    mSymbolWinMultipliers[4][SymbolType::STRAWBERRIES]    = 6;
    mSymbolWinMultipliers[4][SymbolType::SUGAR]           = 6;
    mSymbolWinMultipliers[4][SymbolType::CHOCOLATE_CAKE]  = 8000;
    mSymbolWinMultipliers[4][SymbolType::STRAWBERRY_CAKE] = 7000;
    mSymbolWinMultipliers[4][SymbolType::ROAST_CHICKEN]   = 7000;
    mSymbolWinMultipliers[4][SymbolType::SCATTER]         = 50;
    mSymbolWinMultipliers[4][SymbolType::WILD]            = 100;
}

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------


