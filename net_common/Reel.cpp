///------------------------------------------------------------------------------------------------
///  Reel.cpp
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 13/03/2025
///------------------------------------------------------------------------------------------------

#include "Reel.h"
#include <cassert>

///------------------------------------------------------------------------------------------------

namespace slots
{

///------------------------------------------------------------------------------------------------

Reel::Reel()
{
    mReelSymbols.resize(REEL_LENGTH);
}

///------------------------------------------------------------------------------------------------

void Reel::AddNewReelSymbol(const SymbolType symbol)
{
    mReelSymbols.insert(mReelSymbols.begin(), symbol);
    mReelSymbols.pop_back();
    assert(mReelSymbols.size() == REEL_LENGTH);
}

///------------------------------------------------------------------------------------------------

void Reel::SetReelSymbol(const int index, const SymbolType symbol)
{
    assert(index < REEL_LENGTH);
    mReelSymbols[index] = symbol;
}

///------------------------------------------------------------------------------------------------

SymbolType Reel::GetReelSymbol(const int index) const
{
    assert(index < REEL_LENGTH);
    return mReelSymbols[index];
}

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------


