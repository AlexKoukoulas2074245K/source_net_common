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

static const std::unordered_map<SymbolType, std::vector<SymbolType>> RECIPES =
{
    { SymbolType::ROAST_CHICKEN, {SymbolType::CHICKEN, SymbolType::COOKING_OIL, SymbolType::GARLICS, SymbolType::CAMP_FIRE, SymbolType::LEMONS} },
    { SymbolType::CHICKEN_SOUP, {SymbolType::CHICKEN, SymbolType::LEMONS, SymbolType::GARLICS, SymbolType::CAMP_FIRE, SymbolType::WATER} },
    { SymbolType::CHOCOLATE_CAKE, {SymbolType::BUTTER, SymbolType::CHOCOLATE, SymbolType::EGGS, SymbolType::FLOUR, SymbolType::SUGAR} },
    { SymbolType::STRAWBERRY_CAKE, {SymbolType::BUTTER, SymbolType::STRAWBERRIES, SymbolType::EGGS, SymbolType::FLOUR, SymbolType::SUGAR} }
};

///------------------------------------------------------------------------------------------------

int SymbolDataRepository::GetSymbolWinMultiplier(const SymbolType symbolType, const int matchingCount) const
{
    assert(matchingCount <= mSymbolWinMultipliers.size());
    return mSymbolWinMultipliers.at(matchingCount - 1).at(symbolType);
}

///------------------------------------------------------------------------------------------------

const std::vector<SymbolType>& SymbolDataRepository::GetIngredientsForRecipeSymbol(const SymbolType symbolType) const
{
    assert(RECIPES.count(symbolType));
    return RECIPES.at(symbolType);
}

///------------------------------------------------------------------------------------------------

const std::unordered_map<SymbolType, std::vector<SymbolType>>& SymbolDataRepository::GetAllRecipesAndIngredientsMap() const
{
    return RECIPES;
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
    mSymbolWinMultipliers[0][SymbolType::ROAST_CHICKEN]   = 4;
    mSymbolWinMultipliers[0][SymbolType::CHOCOLATE_CAKE]  = 4;
    mSymbolWinMultipliers[0][SymbolType::STRAWBERRY_CAKE] = 4;
    mSymbolWinMultipliers[0][SymbolType::CHICKEN_SOUP]    = 4;
    
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
    mSymbolWinMultipliers[2][SymbolType::WATER]           = 1;
    mSymbolWinMultipliers[2][SymbolType::CHOCOLATE_CAKE]  = 20;
    mSymbolWinMultipliers[2][SymbolType::STRAWBERRY_CAKE] = 20;
    mSymbolWinMultipliers[2][SymbolType::ROAST_CHICKEN]   = 20;
    mSymbolWinMultipliers[2][SymbolType::CHICKEN_SOUP]    = 20;
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
    mSymbolWinMultipliers[3][SymbolType::WATER]           = 2;
    mSymbolWinMultipliers[3][SymbolType::CHOCOLATE_CAKE]  = 40;
    mSymbolWinMultipliers[3][SymbolType::STRAWBERRY_CAKE] = 40;
    mSymbolWinMultipliers[3][SymbolType::ROAST_CHICKEN]   = 40;
    mSymbolWinMultipliers[3][SymbolType::CHICKEN_SOUP]    = 40;
    mSymbolWinMultipliers[3][SymbolType::SCATTER]         = 40;
    
    // 5x matches
    mSymbolWinMultipliers[4][SymbolType::BUTTER]          = 6;
    mSymbolWinMultipliers[4][SymbolType::CAMP_FIRE]       = 8;
    mSymbolWinMultipliers[4][SymbolType::CHICKEN]         = 8;
    mSymbolWinMultipliers[4][SymbolType::CHOCOLATE]       = 8;
    mSymbolWinMultipliers[4][SymbolType::COOKING_OIL]     = 6;
    mSymbolWinMultipliers[4][SymbolType::EGGS]            = 6;
    mSymbolWinMultipliers[4][SymbolType::FLOUR]           = 6;
    mSymbolWinMultipliers[4][SymbolType::GARLICS]         = 6;
    mSymbolWinMultipliers[4][SymbolType::LEMONS]          = 6;
    mSymbolWinMultipliers[4][SymbolType::STRAWBERRIES]    = 6;
    mSymbolWinMultipliers[4][SymbolType::SUGAR]           = 6;
    mSymbolWinMultipliers[4][SymbolType::WATER]           = 6;
    mSymbolWinMultipliers[4][SymbolType::CHOCOLATE_CAKE]  = 100;
    mSymbolWinMultipliers[4][SymbolType::STRAWBERRY_CAKE] = 100;
    mSymbolWinMultipliers[4][SymbolType::ROAST_CHICKEN]   = 100;
    mSymbolWinMultipliers[4][SymbolType::CHICKEN_SOUP]    = 100;
    mSymbolWinMultipliers[4][SymbolType::SCATTER]         = 100;
    mSymbolWinMultipliers[4][SymbolType::WILD]            = 100;
}

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------


