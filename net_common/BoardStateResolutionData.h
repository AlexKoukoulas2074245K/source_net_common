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

#include <vector>

///------------------------------------------------------------------------------------------------

namespace slots
{

///------------------------------------------------------------------------------------------------

struct SymbolEntryData
{
    SymbolType mSymbolType;
    int mCol = 0;
    int mRow = 0;
};

///------------------------------------------------------------------------------------------------

struct PaylineResolutionData
{
    PaylineType mPayline;
    std::vector<SymbolEntryData> mSymbolData;
    int mWinMultiplier = 0;
    bool mTumbled = false;
    bool mFeature = false;
};

///------------------------------------------------------------------------------------------------

struct BoardStateResolutionData
{
    std::vector<PaylineResolutionData> mWinningPaylines;
    int mTotalWinMultiplier = 0;
};

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

#endif /* BoardStateResolutionData_h */

