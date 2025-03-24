///------------------------------------------------------------------------------------------------
///  Board.h
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 27/02/2025
///------------------------------------------------------------------------------------------------

#ifndef Board_h
#define Board_h

///------------------------------------------------------------------------------------------------

#include "Reel.h"
#include "Paylines.h"
#include "BoardStateResolutionData.h"

///------------------------------------------------------------------------------------------------

namespace slots
{

///------------------------------------------------------------------------------------------------

inline constexpr int BOARD_COLS = 5;

///------------------------------------------------------------------------------------------------

class Board
{
public:
    Board();
    
    const BoardStateResolutionData& ResolveBoardState();
    PaylineResolutionData ResolvePayline(const PaylineType payline);

    void PopulateBoardForSpin(const int spinId);
    void SetBoardSymbol(const int row, const int col, const SymbolType symbol);
    SymbolType GetBoardSymbol(const int row, const int col) const;
    int GetSymbolCountInEntireReel(const int reelIndex, const SymbolType symbol) const;
    int GetSymbolCountInPlayableBoard(const SymbolType symbol) const;

private:
    void RandomControlledBoardPopulation();
    bool IsValidSymbol(const int row, const int col, const SymbolType symbol) const;

private:
    Reel mBoardReels[BOARD_COLS];
    BoardStateResolutionData mCurrentResolutionData;
    int mCurrentRandomSeed;
};

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

#endif /* Board_h */

