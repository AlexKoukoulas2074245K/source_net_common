///------------------------------------------------------------------------------------------------
///  Board.cpp
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 28/02/2025
///------------------------------------------------------------------------------------------------

#include "Board.h"
#include <cassert>

#if __has_include(<engine/utils/MathUtils.h>)
#include <engine/utils/MathUtils.h>
#else
#include "../util/MathUtils.h"
#endif

///------------------------------------------------------------------------------------------------

namespace slots
{

///------------------------------------------------------------------------------------------------

Board::Board()
{
    mBoardReels[0].SetReelSymbol(3, SymbolType::WILD);
    mBoardReels[1].SetReelSymbol(4, SymbolType::WILD);
    mBoardReels[2].SetReelSymbol(5, SymbolType::WILD);
    mBoardReels[3].SetReelSymbol(4, SymbolType::WILD);
    mBoardReels[4].SetReelSymbol(3, SymbolType::WILD);
}

///------------------------------------------------------------------------------------------------

void Board::PopulateBoard(const int spinId)
{
    mCurrentRandomSeed = spinId;
    math::SetControlSeed(mCurrentRandomSeed);
    
    for (int row = 0; row < REEL_LENGTH; ++row)
    {
        for (int col = 0; col < BOARD_COLS; ++col)
        {
            auto symbolType = static_cast<slots::SymbolType>(math::ControlledRandomInt() % static_cast<int>(SymbolType::COUNT));
            while (symbolType == SymbolType::CHOCOLATE_CAKE ||
                   symbolType == SymbolType::STRAWBERRY_CAKE ||
                   symbolType == SymbolType::ROAST_CHICKEN)
            {
                symbolType = static_cast<slots::SymbolType>(math::ControlledRandomInt() % static_cast<int>(SymbolType::COUNT));
            }
            
            SetBoardSymbol(row, col, symbolType);
        }
    }
    
    mCurrentRandomSeed = math::GetControlSeed();
}

///------------------------------------------------------------------------------------------------

void Board::SetBoardSymbol(const int row, const int col, const SymbolType symbol)
{
    assert(row < slots::REEL_LENGTH && col < BOARD_COLS);
    mBoardReels[col].SetReelSymbol(row, symbol);
}

///------------------------------------------------------------------------------------------------

SymbolType Board::GetBoardSymbol(const int row, const int col) const
{
    assert(row < slots::REEL_LENGTH && col < BOARD_COLS);
    return mBoardReels[col].GetReelSymbol(row);
}

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------


