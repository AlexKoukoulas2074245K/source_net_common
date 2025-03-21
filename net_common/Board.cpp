///------------------------------------------------------------------------------------------------
///  Board.cpp
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 28/02/2025
///------------------------------------------------------------------------------------------------

#include "Board.h"
#include "SymbolDataRepository.h"

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
    math::SetControlSeed(1); // Just a nice initial board state
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
        if (paylineResolutionData.mWinMultiplier > 0)
        {
            mCurrentResolutionData.mTotalWinMultiplier += paylineResolutionData.mWinMultiplier;
            mCurrentResolutionData.mWinningPaylines.push_back(std::move(paylineResolutionData));
        }
    }
    
    return mCurrentResolutionData;
}

///------------------------------------------------------------------------------------------------

PaylineResolutionData Board::ResolvePayline(const PaylineType payline)
{
    PaylineResolutionData result = {};
    result.mPayline = payline;
    result.mWinMultiplier = 0.0f;
    
    std::vector<SymbolEntryData> initialSymbolData;
    auto populateSymbolData = [&](const int row, const int col)
    {
        SymbolEntryData symbolEntryData;
        symbolEntryData.mCol = col;
        symbolEntryData.mRow = row;
        symbolEntryData.mSymbolType = mBoardReels[col].GetReelSymbol(row);
        initialSymbolData.emplace_back(std::move(symbolEntryData));
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
    
    auto& symbolDataRepo = SymbolDataRepository::GetInstance();

    auto symbolOccurrenceCount = [&](const SymbolType symbol)
    {
        int count = 0;
        for (int i = 0; i < BOARD_COLS; ++i)
        {
            if (initialSymbolData[i].mSymbolType == symbol)
            {
                count++;
            }
        }
        return count;
    };

    int scatterCount = symbolOccurrenceCount(SymbolType::SCATTER);
    int wildCount = symbolOccurrenceCount(SymbolType::WILD);

    int firstNonSpecialSymbolIndex = -1;
    for (int i = 0; i < BOARD_COLS; ++i)
    {
        if (initialSymbolData[i].mSymbolType != SymbolType::WILD &&
            initialSymbolData[i].mSymbolType != SymbolType::SCATTER)
        {
            firstNonSpecialSymbolIndex = i;
            break;
        }
    }
    
    // 5 Wilds/5 Scatters
    if (firstNonSpecialSymbolIndex == -1)
    {
        if (scatterCount == 5)
        {
            result.mWinMultiplier = symbolDataRepo.GetSymbolWinMultiplier(SymbolType::SCATTER, scatterCount);
            result.mSymbolData = initialSymbolData;
            result.mFeature = true;
            return result;
        }
        else if (wildCount == 5)
        {
            result.mWinMultiplier = symbolDataRepo.GetSymbolWinMultiplier(SymbolType::WILD, scatterCount);
            result.mSymbolData = initialSymbolData;
            return result;
        }
    }
    
    // 3+ Scatters
    if (scatterCount >= 3)
    {
        result.mWinMultiplier = symbolDataRepo.GetSymbolWinMultiplier(SymbolType::SCATTER, scatterCount);
        result.mSymbolData = initialSymbolData;
        result.mFeature = true;
        return result;
    }
    
    // Less than 3 scatters with scatter on 1/2/3 reel is always a non-win
    if (initialSymbolData[0].mSymbolType == SymbolType::SCATTER ||
        initialSymbolData[1].mSymbolType == SymbolType::SCATTER ||
        initialSymbolData[2].mSymbolType == SymbolType::SCATTER)
    {
        result.mWinMultiplier = 0;
        return result;
    }
    
    // Check for tumble/combo
    if (symbolOccurrenceCount(SymbolType::CHICKEN) == 1 &&
        symbolOccurrenceCount(SymbolType::COOKING_OIL) == 1 &&
        symbolOccurrenceCount(SymbolType::GARLICS) == 1 &&
        symbolOccurrenceCount(SymbolType::CAMP_FIRE) == 1 &&
        symbolOccurrenceCount(SymbolType::LEMONS) == 1)
    {
        // Roast chicken tumble
        result.mWinMultiplier = symbolDataRepo.GetSymbolWinMultiplier(SymbolType::ROAST_CHICKEN, 1);
        result.mSymbolData = initialSymbolData;
        result.mTumbled = true;
        return result;
    }
    
    if (symbolOccurrenceCount(SymbolType::BUTTER) == 1 &&
        symbolOccurrenceCount(SymbolType::CHOCOLATE) == 1 &&
        symbolOccurrenceCount(SymbolType::EGGS) == 1 &&
        symbolOccurrenceCount(SymbolType::FLOUR) == 1 &&
        symbolOccurrenceCount(SymbolType::SUGAR) == 1)
    {
        // Chocolate Cake tumble
        result.mWinMultiplier = symbolDataRepo.GetSymbolWinMultiplier(SymbolType::CHOCOLATE_CAKE, 1);
        result.mSymbolData = initialSymbolData;
        result.mTumbled = true;
        return result;
    }
    
    if (symbolOccurrenceCount(SymbolType::BUTTER) == 1 &&
        symbolOccurrenceCount(SymbolType::STRAWBERRIES) == 1 &&
        symbolOccurrenceCount(SymbolType::EGGS) == 1 &&
        symbolOccurrenceCount(SymbolType::FLOUR) == 1 &&
        symbolOccurrenceCount(SymbolType::SUGAR) == 1)
    {
        // Strawberry Cake tumble
        result.mWinMultiplier = symbolDataRepo.GetSymbolWinMultiplier(SymbolType::STRAWBERRY_CAKE, 1);
        result.mSymbolData = initialSymbolData;
        result.mTumbled = true;
        return result;
    }
    
    
    // At this point we need to scan up to firstNonSpecialSymbolIndex
    // and make sure that if it's not == 0, then it needs to be
    // preceeded only by Wilds to be a win
    for (int i = 0; i < firstNonSpecialSymbolIndex; ++i)
    {
        if (initialSymbolData[i].mSymbolType != SymbolType::WILD)
        {
            result.mWinMultiplier = 0;
            return result;
        }
    }
    
    // Make sure there is at least a 3+ win now
    for (int i = firstNonSpecialSymbolIndex + 1; i < 3; ++i)
    {
        if (initialSymbolData[i].mSymbolType != SymbolType::WILD && initialSymbolData[i].mSymbolType != initialSymbolData[firstNonSpecialSymbolIndex].mSymbolType)
        {
            result.mWinMultiplier = 0;
            return result;
        }
    }
    result.mSymbolData.push_back(initialSymbolData[0]);
    result.mSymbolData.push_back(initialSymbolData[1]);
    result.mSymbolData.push_back(initialSymbolData[2]);
    
    // 4+ symbol win
    if (initialSymbolData[3].mSymbolType == initialSymbolData[firstNonSpecialSymbolIndex].mSymbolType || initialSymbolData[3].mSymbolType == SymbolType::WILD)
    {
        result.mSymbolData.push_back(initialSymbolData[3]);
    }
    else
    {
        result.mWinMultiplier = symbolDataRepo.GetSymbolWinMultiplier(initialSymbolData[firstNonSpecialSymbolIndex].mSymbolType, static_cast<int>(result.mSymbolData.size()));
        return result;
    }
    
    // 5 symbol win
    if (initialSymbolData[4].mSymbolType == initialSymbolData[firstNonSpecialSymbolIndex].mSymbolType || initialSymbolData[4].mSymbolType == SymbolType::WILD)
    {
        result.mSymbolData.push_back(initialSymbolData[4]);
    }
    else
    {
        result.mWinMultiplier = symbolDataRepo.GetSymbolWinMultiplier(initialSymbolData[firstNonSpecialSymbolIndex].mSymbolType, static_cast<int>(result.mSymbolData.size()));
        return result;
    }
    
    result.mWinMultiplier = symbolDataRepo.GetSymbolWinMultiplier(initialSymbolData[firstNonSpecialSymbolIndex].mSymbolType, static_cast<int>(result.mSymbolData.size()));

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

int Board::GetSymbolCountInReel(const int reelIndex, const SymbolType symbol) const
{
    int count = 0;
    for (int row = 0; row < REEL_LENGTH; ++row)
    {
        if (mBoardReels[reelIndex].GetReelSymbol(row) == symbol)
        {
            count++;
        }
    }
    
    return count;
}

///------------------------------------------------------------------------------------------------

void Board::RandomControlledBoardPopulation()
{
    for (int row = 0; row < REEL_LENGTH; ++row)
    {
        for (int col = 0; col < BOARD_COLS; ++col)
        {
            auto symbolType = static_cast<slots::SymbolType>(math::ControlledRandomInt() % static_cast<int>(SymbolType::COUNT));
            while (!IsValidSymbol(row, col, symbolType))
            {
                symbolType = static_cast<slots::SymbolType>(math::ControlledRandomInt() % static_cast<int>(SymbolType::COUNT));
            }
            
            SetBoardSymbol(row, col, symbolType);
        }
    }
}

///------------------------------------------------------------------------------------------------

bool Board::IsValidSymbol(const int row, const int col, const SymbolType symbol) const
{
    // No complex symbols
    if (symbol == SymbolType::CHOCOLATE_CAKE ||
        symbol == SymbolType::STRAWBERRY_CAKE ||
        symbol == SymbolType::ROAST_CHICKEN)
    {
        return false;
    }
    
    // No duplicate wild symbols per reel
    if (symbol == SymbolType::WILD && GetSymbolCountInReel(col, SymbolType::WILD) > 0)
    {
        return false;
    }
    
    // No duplicate scatter symbols per reel
    if (symbol == SymbolType::SCATTER && GetSymbolCountInReel(col, SymbolType::SCATTER) > 0)
    {
        return false;
    }

    return true;
}

///------------------------------------------------------------------------------------------------

}

