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

#if __has_include(<engine/utils/Logging.h>)
#include <engine/utils/Logging.h>
#else
#include "../util/Logging.h"
#endif

///------------------------------------------------------------------------------------------------

namespace slots
{

///------------------------------------------------------------------------------------------------

static const std::unordered_map<SymbolType, std::string> DEBUG_SYMBOL_NAMES =
{
    { SymbolType::BUTTER, "Butter" },
    { SymbolType::CAMP_FIRE, "CampFire" },
    { SymbolType::CHICKEN, "Chicken" },
    { SymbolType::CHOCOLATE, "Chocolate" },
    { SymbolType::COOKING_OIL, "CookingOil" },
    { SymbolType::EGGS, "Eggs" },
    { SymbolType::FLOUR, "Flour" },
    { SymbolType::GARLICS, "Garlics" },
    { SymbolType::LEMONS, "Lemons" },
    { SymbolType::STRAWBERRIES, "Strawberries" },
    { SymbolType::SUGAR, "Sugar" },
    { SymbolType::WATER, "Water" },
    { SymbolType::CHOCOLATE_CAKE, "ChocolateCake" },
    { SymbolType::STRAWBERRY_CAKE, "StrawberryCake" },
    { SymbolType::ROAST_CHICKEN, "RoastChicken" },
    { SymbolType::WILD, "Wild" },
    { SymbolType::SCATTER, "Scatter" },
    { SymbolType::CHICKEN_SOUP, "ChickenSoup" }
};

///------------------------------------------------------------------------------------------------

const std::string& Board::GetSymbolDebugName(const SymbolType symbol)
{
    return DEBUG_SYMBOL_NAMES.at(symbol);
}

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
    mBoardReels[2].SetReelSymbol(4, SymbolType::WILD);
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
        
        if (paylineResolutionData.mCombo)
        {
            mCurrentResolutionData.mShouldTumble = true;
        }
    }
    
    // Handle scatters
    const auto& scatterCoordinates = GetSymbolCoordinatesInPlayableBoard(SymbolType::SCATTER);
    if (scatterCoordinates.size() >= 3)
    {
        PaylineResolutionData scatterPayline = {};
        scatterPayline.mScatter = true;

        for (const auto& coords: scatterCoordinates)
        {
            scatterPayline.mSymbolData.emplace_back(SymbolEntryData{ SymbolType::SCATTER, coords.second, coords.first });
        }
        
        if (scatterCoordinates.size() == 3)
        {
            scatterPayline.mWinSourceType = WinSourceType::SCATTER_3;
        }
        else if (scatterCoordinates.size() == 4)
        {
            scatterPayline.mWinSourceType = WinSourceType::SCATTER_4;
        }
        else
        {
            scatterPayline.mWinSourceType = WinSourceType::SCATTER_5;
        }
        
        scatterPayline.mWinMultiplier += SymbolDataRepository::GetInstance().GetSymbolWinMultiplier(SymbolType::SCATTER, static_cast<int>(scatterCoordinates.size()));
        mCurrentResolutionData.mTotalWinMultiplier += scatterPayline.mWinMultiplier;
        mCurrentResolutionData.mWinningPaylines.push_back(std::move(scatterPayline));
    }
    
    
    return mCurrentResolutionData;
}

///------------------------------------------------------------------------------------------------

TumbleResolutionData Board::ResolveBoardTumble()
{
    TumbleResolutionData tumbleResolutionData = {};
    
    for (const auto& paylineData: mCurrentResolutionData.mWinningPaylines)
    {
        if (paylineData.mCombo)
        {
            // Destroy combo payline
            for (const auto& symbolEntryData: paylineData.mSymbolData)
            {
                tumbleResolutionData.mDestroyedCoords.insert(symbolEntryData);
            }
        }
    }
    
    for (const auto& paylineData: mCurrentResolutionData.mWinningPaylines)
    {
        if (paylineData.mCombo)
        {
            // Place combo symbols
            for (const auto& symbolEntryData: paylineData.mSymbolData)
            {
                if (!tumbleResolutionData.mPlacedCombosCoords.contains(symbolEntryData))
                {
                    // Replace symbol with combo noe
                    SymbolEntryData newEntryData = symbolEntryData;
                    newEntryData.mSymbolType = paylineData.mComboSymbol;
                    
                    // Insert to finalised placement coords and remove from destroyed ones
                    tumbleResolutionData.mPlacedCombosCoords.insert(newEntryData);
                    tumbleResolutionData.mDestroyedCoords.erase(newEntryData);
                    
                    break;
                }
            }
        }
    }

    // Sanity check that we are not planning to destroy a place combo symbol entry
    for (const auto& entry: tumbleResolutionData.mDestroyedCoords)
    {
        assert(!tumbleResolutionData.mPlacedCombosCoords.contains(entry));
    }
    
    // Perform board additions
    for (const auto& comboSymbolEntryDataToPlace: tumbleResolutionData.mPlacedCombosCoords)
    {
        SetBoardSymbol(comboSymbolEntryDataToPlace.mRow, comboSymbolEntryDataToPlace.mCol, comboSymbolEntryDataToPlace.mSymbolType);
    }
    
    // Perform Tumble
    auto destroyedCoordsCopy = tumbleResolutionData.mDestroyedCoords;
    auto iter = destroyedCoordsCopy.begin();
    while (iter != destroyedCoordsCopy.end())
    {
        const auto& symbolEntryDataToDestroy = *iter;
        for (int row = symbolEntryDataToDestroy.mRow; row > 0; --row)
        {
            SetBoardSymbol(row, symbolEntryDataToDestroy.mCol, GetBoardSymbol(row - 1, symbolEntryDataToDestroy.mCol));
        }
        
        auto newSymbol = GenerateNewSymbolForCoords(0, symbolEntryDataToDestroy.mCol);
        SetBoardSymbol(0, symbolEntryDataToDestroy.mCol, newSymbol);
        
        tumbleResolutionData.mNewlyCreatedSymbolData.emplace_back(SymbolEntryData{newSymbol, 0, symbolEntryDataToDestroy.mCol});
        iter = destroyedCoordsCopy.erase(iter);
    }
    
    return tumbleResolutionData;
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
            // Scatters are handled specially
            result.mScatter = true;
            return result;
        }
        else if (wildCount == 5)
        {
            result.mWinMultiplier = symbolDataRepo.GetSymbolWinMultiplier(SymbolType::WILD, wildCount);
            result.mSymbolData = initialSymbolData;
            result.mWinSourceType = WinSourceType::WILD_5;
            return result;
        }
    }
    
    // 3/4 Scatters
    if (scatterCount >= 3)
    {
        // Scatters are handled specially
        result.mScatter = true;
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
    
    // Check for recipes
    for (const auto& recipeEntry: symbolDataRepo.GetAllRecipesAndIngredientsMap())
    {
        const auto& recipeSymbol = recipeEntry.first;
        const auto& ingredients = recipeEntry.second;
        auto testPaylineSymbols = initialSymbolData;
        
        for (const auto& ingredientSymbol: ingredients)
        {
            assert(!testPaylineSymbols.empty());
            auto foundIter = std::find_if(testPaylineSymbols.begin(), testPaylineSymbols.end(), [ingredientSymbol](const SymbolEntryData& symbolEntryData){ return symbolEntryData.mSymbolType == ingredientSymbol; });
            
            // First check if we have the actual ingredient symbol
            if (foundIter != testPaylineSymbols.end())
            {
                testPaylineSymbols.erase(foundIter);
            }
            else
            {
                // If not, check for a wild
                foundIter = std::find_if(testPaylineSymbols.begin(), testPaylineSymbols.end(), [](const SymbolEntryData& symbolEntryData){ return symbolEntryData.mSymbolType == SymbolType::WILD; });
                if (foundIter != testPaylineSymbols.end())
                {
                    testPaylineSymbols.erase(foundIter);
                }
                else
                {
                    break;
                }
            }
        }
        
        if (testPaylineSymbols.empty())
        {
            result.mWinMultiplier = symbolDataRepo.GetSymbolWinMultiplier(recipeSymbol, 1);
            result.mSymbolData = initialSymbolData;
            result.mCombo = true;
            result.mComboSymbol = recipeSymbol;
            result.mWinSourceType = WinSourceType::COMBO;
            return result;
        }
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
    
    // Make sure that the configuration is not WILDS + Scatter
    if (firstNonSpecialSymbolIndex == -1)
    {
        if ((symbolOccurrenceCount(SymbolType::SCATTER) == 1 && symbolOccurrenceCount(SymbolType::WILD) == 4) ||
            (symbolOccurrenceCount(SymbolType::SCATTER) == 2 && symbolOccurrenceCount(SymbolType::WILD) == 3))
        result.mWinMultiplier = 0;
        return result;
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
        result.mWinSourceType = WinSourceType::NORMAL_SYMBOL_3;
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
        result.mWinSourceType = WinSourceType::NORMAL_SYMBOL_4;
        return result;
    }
    
    result.mWinMultiplier = symbolDataRepo.GetSymbolWinMultiplier(initialSymbolData[firstNonSpecialSymbolIndex].mSymbolType, static_cast<int>(result.mSymbolData.size()));
    result.mWinSourceType = WinSourceType::NORMAL_SYMBOL_5;

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

int Board::GetSymbolCountInEntireReel(const int reelIndex, const SymbolType symbol) const
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

int Board::GetSymbolCountInPlayableReelArea(const int reelIndex, const SymbolType symbol) const
{
    int count = 0;
    for (int row = 3; row < 6; ++row)
    {
        if (mBoardReels[reelIndex].GetReelSymbol(row) == symbol)
        {
            count++;
        }
    }
    
    return count;
}

///------------------------------------------------------------------------------------------------

int Board::GetSymbolCountInPlayableBoard(const SymbolType symbol) const
{
    int count = 0;
    for (int reelIndex = 0; reelIndex < BOARD_COLS; ++reelIndex)
    {
        for (int row = 3; row < 6; ++row)
        {
            if (mBoardReels[reelIndex].GetReelSymbol(row) == symbol)
            {
                count++;
            }
        }
    }
    return count;
}

///------------------------------------------------------------------------------------------------

std::vector<std::pair<int, int>> Board::GetSymbolCoordinatesInPlayableBoard(const SymbolType symbol) const
{
    std::vector<std::pair<int, int>> result;
    for (int reelIndex = 0; reelIndex < BOARD_COLS; ++reelIndex)
    {
        for (int row = 3; row < 6; ++row)
        {
            if (mBoardReels[reelIndex].GetReelSymbol(row) == symbol)
            {
                result.emplace_back(std::make_pair(row, reelIndex));
            }
        }
    }
    return result;
}

///------------------------------------------------------------------------------------------------

void Board::RandomControlledBoardPopulation()
{
    // Reset board (to avoid side effects of board population from previous
    // board configuration)
    for (int row = 0; row < REEL_LENGTH; ++row)
    {
        for (int col = 0; col < BOARD_COLS; ++col)
        {
            SetBoardSymbol(row, col, SymbolType::BUTTER);
        }
    }

    for (int row = 0; row < REEL_LENGTH; ++row)
    {
        for (int col = 0; col < BOARD_COLS; ++col)
        {
            auto newSymbol = GenerateNewSymbolForCoords(row, col);
            SetBoardSymbol(row, col, newSymbol);
        }
    }

//    SetBoardSymbol(0, 0, SymbolType::BUTTER); SetBoardSymbol(0, 1, SymbolType::BUTTER); SetBoardSymbol(0, 2, SymbolType::BUTTER); SetBoardSymbol(0, 3, SymbolType::BUTTER); SetBoardSymbol(0, 4, SymbolType::BUTTER);
//    SetBoardSymbol(1, 0, SymbolType::BUTTER); SetBoardSymbol(1, 1, SymbolType::BUTTER); SetBoardSymbol(1, 2, SymbolType::BUTTER); SetBoardSymbol(1, 3, SymbolType::BUTTER); SetBoardSymbol(1, 4, SymbolType::BUTTER);
//    SetBoardSymbol(2, 0, SymbolType::BUTTER); SetBoardSymbol(2, 1, SymbolType::BUTTER); SetBoardSymbol(2, 2, SymbolType::BUTTER); SetBoardSymbol(2, 3, SymbolType::BUTTER); SetBoardSymbol(2, 4, SymbolType::BUTTER);
//    SetBoardSymbol(3, 0, SymbolType::SUGAR);  SetBoardSymbol(3, 1, SymbolType::SUGAR);  SetBoardSymbol(3, 2, SymbolType::BUTTER); SetBoardSymbol(3, 3, SymbolType::SUGAR); SetBoardSymbol(3, 4, SymbolType::SUGAR);
//    SetBoardSymbol(4, 0, SymbolType::SUGAR);  SetBoardSymbol(4, 1, SymbolType::SUGAR);  SetBoardSymbol(4, 2, SymbolType::SUGAR);  SetBoardSymbol(4, 3, SymbolType::BUTTER); SetBoardSymbol(4, 4, SymbolType::BUTTER);
//    SetBoardSymbol(5, 0, SymbolType::SUGAR);  SetBoardSymbol(5, 1, SymbolType::SUGAR);  SetBoardSymbol(5, 2, SymbolType::BUTTER); SetBoardSymbol(5, 3, SymbolType::SUGAR); SetBoardSymbol(5, 4, SymbolType::SUGAR);
//    SetBoardSymbol(6, 0, SymbolType::BUTTER); SetBoardSymbol(6, 1, SymbolType::BUTTER); SetBoardSymbol(6, 2, SymbolType::BUTTER); SetBoardSymbol(6, 3, SymbolType::BUTTER); SetBoardSymbol(6, 4, SymbolType::BUTTER);
//    SetBoardSymbol(7, 0, SymbolType::BUTTER); SetBoardSymbol(7, 1, SymbolType::BUTTER); SetBoardSymbol(7, 2, SymbolType::BUTTER); SetBoardSymbol(7, 3, SymbolType::BUTTER); SetBoardSymbol(7, 4, SymbolType::BUTTER);
//    SetBoardSymbol(8, 0, SymbolType::BUTTER); SetBoardSymbol(8, 1, SymbolType::BUTTER); SetBoardSymbol(8, 2, SymbolType::BUTTER); SetBoardSymbol(8, 3, SymbolType::BUTTER); SetBoardSymbol(8, 4, SymbolType::BUTTER);
}

///------------------------------------------------------------------------------------------------

SymbolType Board::GenerateNewSymbolForCoords(const int row, const int col) const
{
    auto symbolType = static_cast<slots::SymbolType>(math::ControlledRandomInt() % static_cast<int>(SymbolType::COUNT));
    if (symbolType == SymbolType::SCATTER && GetSymbolCountInPlayableBoard(SymbolType::SCATTER) == 2)
    {
        symbolType = static_cast<slots::SymbolType>(math::ControlledRandomInt() % static_cast<int>(SymbolType::COUNT));
    }
    else if (symbolType == SymbolType::WILD)
    {
        symbolType = static_cast<slots::SymbolType>(math::ControlledRandomInt() % static_cast<int>(SymbolType::COUNT));
    }

    while (!IsValidSymbol(row, col, symbolType))
    {
        symbolType = static_cast<slots::SymbolType>(math::ControlledRandomInt() % static_cast<int>(SymbolType::COUNT));
    }
    return symbolType;
}

///------------------------------------------------------------------------------------------------

bool Board::IsValidSymbol(const int row, const int col, const SymbolType symbol) const
{
    // No complex symbols
    if (SymbolDataRepository::GetInstance().GetAllRecipesAndIngredientsMap().count(symbol))
    {
        return false;
    }
    
    // No duplicate wild symbols per reel
    if (symbol == SymbolType::WILD && GetSymbolCountInEntireReel(col, SymbolType::WILD) > 0)
    {
        return false;
    }
    
    // No duplicate scatter symbols per reel
    if (symbol == SymbolType::SCATTER && GetSymbolCountInEntireReel(col, SymbolType::SCATTER) > 0)
    {
        return false;
    }

    return true;
}

///------------------------------------------------------------------------------------------------

void Board::PrintBoardDebug()
{
    logging::Log(logging::LogType::INFO, "\n");
    for (auto row = 0; row < REEL_LENGTH; ++row)
    {
        if (row == 3)
        {
            logging::Log(logging::LogType::INFO, "======================================== Begin Playable Area ===========================================");
        }
        else if (row == 6)
        {
            logging::Log(logging::LogType::INFO, "========================================================================================================");
        }

        logging::Log(logging::LogType::INFO, "%-20s %-20s %-20s %-20s %-20s",
            GetSymbolDebugName(GetBoardSymbol(row, 0)).c_str(),
            GetSymbolDebugName(GetBoardSymbol(row, 1)).c_str(),
            GetSymbolDebugName(GetBoardSymbol(row, 2)).c_str(),
            GetSymbolDebugName(GetBoardSymbol(row, 3)).c_str(),
            GetSymbolDebugName(GetBoardSymbol(row, 4)).c_str());
    }
    logging::Log(logging::LogType::INFO, "\n");
}

///------------------------------------------------------------------------------------------------

}

