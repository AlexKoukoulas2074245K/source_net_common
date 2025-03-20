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
    float mWinMultiplier = 0.0f;
};

///------------------------------------------------------------------------------------------------

struct BoardStateResolutionData
{
    std::vector<PaylineResolutionData> mWinningReels;
    bool mTumbled = false;
};

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

#endif /* BoardStateResolutionData_h */

