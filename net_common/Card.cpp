///------------------------------------------------------------------------------------------------
///  Card.cpp
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 6/11/2024
///------------------------------------------------------------------------------------------------

#include "Card.h"

///------------------------------------------------------------------------------------------------

namespace poker
{

///------------------------------------------------------------------------------------------------

std::string Card::ToString() const noexcept
{
    const auto& cardSuit = std::string(1, static_cast<char>(mCardSuit));
    
    switch (mCardRank)
    {
        case CardRank::JACK: return "J" + cardSuit;
        case CardRank::QUEEN: return "Q" + cardSuit;
        case CardRank::KING: return "K" + cardSuit;
        case CardRank::ACE: return "A" + cardSuit;
        default: return std::to_string(static_cast<int>(mCardRank)) + cardSuit;
    }
};

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

