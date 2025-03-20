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
    RandomControlledBoardPopulation();
    mBoardReels[0].SetReelSymbol(3, SymbolType::SCATTER);
    mBoardReels[1].SetReelSymbol(4, SymbolType::SCATTER);
    mBoardReels[2].SetReelSymbol(5, SymbolType::SCATTER);
    mBoardReels[3].SetReelSymbol(4, SymbolType::SCATTER);
    mBoardReels[4].SetReelSymbol(3, SymbolType::SCATTER);
}

///------------------------------------------------------------------------------------------------

const BoardStateResolutionData& Board::ResolveBoardState()
{
    mCurrentResolutionData = {};
    
    for (int i = 0; i < static_cast<int>(PaylineType::PAYLINE_COUNT); ++i)
    {
        const auto& paylineResolutionData = ResolvePayline(static_cast<PaylineType>(i));
        (void)paylineResolutionData;
    }

    return mCurrentResolutionData;
}

///------------------------------------------------------------------------------------------------

PaylineResolutionData Board::ResolvePayline(const PaylineType payline)
{
    PaylineResolutionData result = {};
    result.mPayline = payline;
    result.mWinMultiplier = 0.0f;

    auto populateSymbolData = [&](const int row, const int col)
    {
        SymbolEntryData symbolEntryData;
        symbolEntryData.mCol = col;
        symbolEntryData.mRow = row;
        symbolEntryData.mSymbolType = mBoardReels[col].GetReelSymbol(row);
        result.mSymbolData.emplace_back(std::move(symbolEntryData));
    };

    switch (payline)
    {
        case PaylineType::PAYLINE_1:  populateSymbolData(3, 0); populateSymbolData(3, 1); populateSymbolData(3, 2); populateSymbolData(3, 3); populateSymbolData(3, 4);
            break;
        case PaylineType::PAYLINE_2:  populateSymbolData(4, 0); populateSymbolData(4, 1); populateSymbolData(4, 2); populateSymbolData(4, 3); populateSymbolData(4, 4);
            break;
        case PaylineType::PAYLINE_3:  populateSymbolData(5, 0); populateSymbolData(5, 1); populateSymbolData(5, 2); populateSymbolData(5, 3); populateSymbolData(5, 4);
            break;
        case PaylineType::PAYLINE_4:  populateSymbolData(3, 0); populateSymbolData(4, 1); populateSymbolData(5, 2); populateSymbolData(4, 3); populateSymbolData(3, 4);
            break;
        case PaylineType::PAYLINE_5:  populateSymbolData(5, 0); populateSymbolData(4, 1); populateSymbolData(3, 2); populateSymbolData(4, 3); populateSymbolData(5, 4);
            break;
        case PaylineType::PAYLINE_6:  populateSymbolData(3, 0); populateSymbolData(3, 1); populateSymbolData(4, 2); populateSymbolData(5, 3); populateSymbolData(5, 4);
            break;
        case PaylineType::PAYLINE_7:  populateSymbolData(5, 0); populateSymbolData(5, 1); populateSymbolData(4, 2); populateSymbolData(3, 3); populateSymbolData(3, 4);
            break;
        case PaylineType::PAYLINE_8:  populateSymbolData(3, 0); populateSymbolData(3, 1); populateSymbolData(4, 2); populateSymbolData(3, 3); populateSymbolData(3, 4);
            break;
        case PaylineType::PAYLINE_9:  populateSymbolData(5, 0); populateSymbolData(5, 1); populateSymbolData(4, 2); populateSymbolData(5, 3); populateSymbolData(5, 4);
            break;
        case PaylineType::PAYLINE_10: populateSymbolData(4, 0); populateSymbolData(3, 1); populateSymbolData(4, 2); populateSymbolData(3, 3); populateSymbolData(4, 4);
            break;
        case PaylineType::PAYLINE_11: populateSymbolData(4, 0); populateSymbolData(5, 1); populateSymbolData(4, 2); populateSymbolData(5, 3); populateSymbolData(4, 4);
            break;
        case PaylineType::PAYLINE_12: populateSymbolData(5, 0); populateSymbolData(4, 1); populateSymbolData(5, 2); populateSymbolData(4, 3); populateSymbolData(5, 4);
            break;
        case PaylineType::PAYLINE_13: populateSymbolData(3, 0); populateSymbolData(4, 1); populateSymbolData(3, 2); populateSymbolData(4, 3); populateSymbolData(3, 4);
            break;
        case PaylineType::PAYLINE_14: populateSymbolData(5, 0); populateSymbolData(3, 1); populateSymbolData(5, 2); populateSymbolData(3, 3); populateSymbolData(5, 4);
            break;
        case PaylineType::PAYLINE_15: populateSymbolData(3, 0); populateSymbolData(5, 1); populateSymbolData(3, 2); populateSymbolData(5, 3); populateSymbolData(3, 4);
            break;
        default: break;
    }
    
    
    return result;
}

///------------------------------------------------------------------------------------------------

void Board::PopulateBoardForSpin(const int spinId)
{
    mCurrentRandomSeed = spinId;
    math::SetControlSeed(mCurrentRandomSeed);
    RandomControlledBoardPopulation();
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

void Board::RandomControlledBoardPopulation()
{
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
}

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------


