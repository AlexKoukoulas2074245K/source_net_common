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
    
    Card(const std::string& cardString);
    
    std::string ToString() const noexcept;
    
    inline CardRank GetRank() const noexcept { return mCardRank; }
    inline CardSuit GetSuit() const noexcept { return mCardSuit; }
    inline int GetRankValue() const noexcept { return static_cast<int>(mCardRank); }
    
private:
    CardRank mCardRank;
    CardSuit mCardSuit;
};

///------------------------------------------------------------------------------------------------
inline bool operator == (const Card& lhs, const Card& rhs) noexcept
{
    return lhs.GetRank() == rhs.GetRank() && lhs.GetSuit() == rhs.GetSuit();
}

inline bool operator != (const Card& lhs, const Card& rhs) noexcept
{
    return !(lhs == rhs);
}

inline bool operator < (const Card& lhs, const Card& rhs) noexcept
{
    return lhs.GetRank() < rhs.GetRank();
}

inline bool operator > (const Card& lhs, const Card& rhs) noexcept
{
    return lhs.GetRank() > rhs.GetRank();
}

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

#endif /* Card_h */

