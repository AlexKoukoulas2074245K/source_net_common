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
    static const std::string& GetSymbolDebugName(const SymbolType symbol);
    
public:
    Board();
    
    const BoardStateResolutionData& ResolveBoardState();
    PaylineResolutionData ResolvePayline(const PaylineType payline);
    TumbleResolutionData ResolveBoardTumble();

    void PopulateBoardForSpin(const int spinId);
    void SetBoardSymbol(const int row, const int col, const SymbolType symbol);
    SymbolType GetBoardSymbol(const int row, const int col) const;
    int GetSymbolCountInEntireReel(const int reelIndex, const SymbolType symbol) const;
    int GetSymbolCountInPlayableReelArea(const int reelIndex, const SymbolType symbol) const;
    int GetSymbolCountInPlayableBoard(const SymbolType symbol) const;
    std::vector<std::pair<int, int>> GetSymbolCoordinatesInPlayableBoard(const SymbolType symbol) const;
    
private:
    void RandomControlledBoardPopulation();
    SymbolType GenerateNewSymbolForCoords(const int row, const int col) const;
    bool IsValidSymbol(const int row, const int col, const SymbolType symbol) const;
    void PrintBoardDebug();

private:
    Reel mBoardReels[BOARD_COLS];
    BoardStateResolutionData mCurrentResolutionData;
    int mCurrentRandomSeed;
};

///------------------------------------------------------------------------------------------------
 
}

///------------------------------------------------------------------------------------------------

#endif /* Board_h */

