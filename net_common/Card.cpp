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

Card::Card(const std::string& cardString)
{
    mCardSuit = static_cast<CardSuit>(cardString.back());
    switch (cardString[0])
    {
        case 'J': mCardRank = poker::CardRank::JACK; break;
        case 'Q': mCardRank = poker::CardRank::QUEEN; break;
        case 'K': mCardRank = poker::CardRank::KING; break;
        case 'A': mCardRank = poker::CardRank::ACE; break;
        case '1': mCardRank = poker::CardRank::TEN; break;
        default: mCardRank = static_cast<CardRank>(std::stoi(std::string(1, cardString[0]))); break;
    }
}

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
        default: return std::to_string(GetRankValue()) + cardSuit;
    }
};

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

