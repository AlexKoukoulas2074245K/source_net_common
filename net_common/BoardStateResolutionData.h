///------------------------------------------------------------------------------------------------
///  BoardStateResolutionData.h
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 20/03/2025
///------------------------------------------------------------------------------------------------

#ifndef BoardStateResolutionData_h
#define BoardStateResolutionData_h

///------------------------------------------------------------------------------------------------

#include "Paylines.h"

#include <set>
#include <vector>
#include <unordered_map>

///------------------------------------------------------------------------------------------------

namespace slots
{

///------------------------------------------------------------------------------------------------

enum class WinSourceType
{
    NONE,
    SCATTER_5,
    SCATTER_4,
    SCATTER_3,
    WILD_5,
    COMBO,
    NORMAL_SYMBOL_5,
    NORMAL_SYMBOL_4,
    NORMAL_SYMBOL_3
};

///------------------------------------------------------------------------------------------------

inline const std::unordered_map<WinSourceType, std::string> WIN_SOURCE_TYPE_NAMES =
{
    { WinSourceType::NONE,            "NONE" },
    { WinSourceType::SCATTER_5,       "SCATTER_5" },
    { WinSourceType::SCATTER_4,       "SCATTER_4" },
    { WinSourceType::SCATTER_3,       "SCATTER_3" },
    { WinSourceType::WILD_5,          "WILD_5" },
    { WinSourceType::COMBO,           "COMBO" },
    { WinSourceType::NORMAL_SYMBOL_5, "NORMAL_SYMBOL_5" },
    { WinSourceType::NORMAL_SYMBOL_4, "NORMAL_SYMBOL_4" },
    { WinSourceType::NORMAL_SYMBOL_3, "NORMAL_SYMBOL_3" }
};

///------------------------------------------------------------------------------------------------

struct SymbolEntryData
{
    SymbolType mSymbolType;
    int mCol = 0;
    int mRow = 0;
};

///------------------------------------------------------------------------------------------------

struct SymbolEntryDataPlacementComparator
{
    bool operator()(const SymbolEntryData& lhs, const SymbolEntryData& rhs) const
    {
        return (lhs.mRow * 5 + lhs.mCol) <  (rhs.mRow * 5 + rhs.mCol);
    }
};

struct SymbolEntryDataDestructionComparator
{
    bool operator()(const SymbolEntryData& lhs, const SymbolEntryData& rhs) const
    {
        if (lhs.mCol != rhs.mCol)
        {
            return lhs.mCol < rhs.mCol;
        }
        else
        {
            return lhs.mRow < rhs.mRow;
        }
    }
};

///------------------------------------------------------------------------------------------------

struct TumbleResolutionData
{
    std::set<SymbolEntryData, SymbolEntryDataPlacementComparator> mPlacedCombosCoords;
    std::set<SymbolEntryData, SymbolEntryDataDestructionComparator> mDestroyedCoordsTopToBotom;
    std::vector<SymbolEntryData> mComboIngredientsSymbolData;
    std::vector<SymbolEntryData> mNewlyCreatedSymbolData;
};

///------------------------------------------------------------------------------------------------

struct PaylineResolutionData
{
    PaylineType mPayline;
    WinSourceType mWinSourceType;
    SymbolType mComboSymbol;
    std::vector<SymbolEntryData> mSymbolData;
    int mWinMultiplier = 0;
    bool mCombo = false;
    bool mScatter = false;
};

///------------------------------------------------------------------------------------------------

struct BoardStateResolutionData
{
    std::vector<PaylineResolutionData> mWinningPaylines;
    int mTotalWinMultiplier = 0;
    bool mShouldTumble = false;
};

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

#endif /* BoardStateResolutionData_h */

