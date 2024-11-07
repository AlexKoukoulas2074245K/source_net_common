///------------------------------------------------------------------------------------------------
///  Hand.h
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 6/11/2024
///------------------------------------------------------------------------------------------------

#ifndef Hand_h
#define Hand_h

///------------------------------------------------------------------------------------------------

#if __has_include(<engine/utils/StringUtils.h>)
#include <engine/utils/StringUtils.h>
#else
#include "../util/StringUtils.h"
#endif
#include "Card.h"

#include <array>

///------------------------------------------------------------------------------------------------

namespace poker
{

///------------------------------------------------------------------------------------------------

inline constexpr int HAND_SIZE = 5;

///------------------------------------------------------------------------------------------------

enum class HandKind
{
    HIGH_CARD = 0,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH
};

///------------------------------------------------------------------------------------------------

class Hand final
{
public:
    Hand(const std::array<Card, HAND_SIZE>& handCards, const HandKind handKind)
        : mHandCards(handCards)
        , mHandKind(handKind)
    {
    }

    inline const std::array<Card, HAND_SIZE>& GetHandCards() const { return mHandCards; }
    inline const HandKind GetHandKind() const { return mHandKind; }
    inline int GetHandKindValue() const { return static_cast<int>(mHandKind); }
    
private:
    const std::array<Card, HAND_SIZE> mHandCards;
    const HandKind mHandKind;
};

///------------------------------------------------------------------------------------------------
// All operators here test explicitly for hand strength, not sameness of card ranks & suits
bool operator < (const Hand& lhs, const Hand& rhs);
bool operator > (const Hand& lhs, const Hand& rhs);
bool operator == (const Hand& lhs, const Hand& rhs);
bool operator != (const Hand& lhs, const Hand& rhs);

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

#endif /* Hand_h */

