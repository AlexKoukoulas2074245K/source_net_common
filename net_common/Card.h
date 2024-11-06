///------------------------------------------------------------------------------------------------
///  Card.h
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 6/11/2024
///------------------------------------------------------------------------------------------------

#ifndef Card_h
#define Card_h

///------------------------------------------------------------------------------------------------

#if __has_include(<engine/utils/StringUtils.h>)
#include <engine/utils/StringUtils.h>
#else
#include "../util/StringUtils.h"
#endif

///------------------------------------------------------------------------------------------------

namespace poker
{

///------------------------------------------------------------------------------------------------

enum class CardRank
{
    TWO = 2,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE = 14
};

///------------------------------------------------------------------------------------------------

enum class CardSuit
{
    SPADE = 'S',
    HEART = 'H',
    DIAMOND = 'D',
    CLUB = 'C'
};

///------------------------------------------------------------------------------------------------

class Card final
{
public:
    Card(const CardRank cardRank, const CardSuit cardSuit) noexcept
    : mCardRank(cardRank)
    , mCardSuit(cardSuit)
    {}
    
    std::string ToString() const noexcept;
    
public:
    const CardRank mCardRank;
    const CardSuit mCardSuit;
};

///------------------------------------------------------------------------------------------------

inline bool operator == (const Card& lhs, const Card& rhs) noexcept
{
    return lhs.mCardRank == rhs.mCardRank && lhs.mCardSuit == rhs.mCardSuit;
}

inline bool operator < (const Card& lhs, const Card& rhs) noexcept
{
    return lhs.mCardRank < rhs.mCardRank;
}

inline bool operator > (const Card& lhs, const Card& rhs) noexcept
{
    return lhs.mCardRank > rhs.mCardRank;
}

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

#endif /* Card_h */

