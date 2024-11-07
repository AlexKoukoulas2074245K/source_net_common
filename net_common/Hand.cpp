///------------------------------------------------------------------------------------------------
///  Hand.cpp
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 6/11/2024
///------------------------------------------------------------------------------------------------

#include "Hand.h"

#include <algorithm>

///------------------------------------------------------------------------------------------------

namespace poker
{

///------------------------------------------------------------------------------------------------

template<class Comparator>
static bool DescendingHandComparison(const std::array<Card, HAND_SIZE>& lhsSortedHand, const std::array<Card, HAND_SIZE>& rhsSortedHand, Comparator comp)
{
    for (int i = HAND_SIZE - 1; i >= 0; i--)
    {
        if (lhsSortedHand[i].GetRank() != rhsSortedHand[i].GetRank())
        {
            return comp(lhsSortedHand[i].GetRank(), rhsSortedHand[i].GetRank());
        }
    }
    return false;
}

template<class Comparator>
static bool HandComparison(const Hand& lhs, const Hand& rhs, Comparator comp)
{
    if (lhs.GetHandKind() != rhs.GetHandKind())
    {
        return comp(lhs.GetHandKindValue(), rhs.GetHandKindValue());
    }
    
    auto lhsSortedHand = lhs.GetHandCards();
    auto rhsSortedHand = rhs.GetHandCards();
    
    std::sort(lhsSortedHand.begin(), lhsSortedHand.end());
    std::sort(rhsSortedHand.begin(), rhsSortedHand.end());
    
    switch (lhs.GetHandKind())
    {
        case HandKind::HIGH_CARD: break;
        case HandKind::FLUSH: break;
        case HandKind::ROYAL_FLUSH: break;
        
        case HandKind::ONE_PAIR:
        {
            auto lhsPairRank = CardRank::TWO;
            auto rhsPairRank = CardRank::TWO;
            
            for (int i = 0; i < HAND_SIZE - 1; i++)
            {
                if (lhsSortedHand[i].GetRank() == lhsSortedHand[i + 1].GetRank())
                {
                    lhsPairRank = lhsSortedHand[i].GetRank();
                }
                
                if (rhsSortedHand[i].GetRank() == rhsSortedHand[i + 1].GetRank())
                {
                    rhsPairRank = rhsSortedHand[i].GetRank();
                }
            }
            
            if (lhsPairRank != rhsPairRank)
            {
                return comp(lhsPairRank, rhsPairRank);
            }
        } break;

        case HandKind::TWO_PAIR:
        {
            // The fourth card will always be part of the highest pair
            // in a sorted hand
            if (lhsSortedHand[3].GetRank() != rhsSortedHand[3].GetRank())
            {
                return comp(lhsSortedHand[3].GetRank(), rhsSortedHand[3].GetRank());
            }
            
            // Otherwise the second card will always be part of the lowest pair
            // in a sorted hand
            if (lhsSortedHand[1].GetRank() != rhsSortedHand[1].GetRank())
            {
                return comp(lhsSortedHand[1].GetRank(), rhsSortedHand[1].GetRank());
            }
        } break;
            
        case HandKind::THREE_OF_A_KIND:
        {
            // The third card will always be part of the tripplet
            // in a sorted hand
            if (lhsSortedHand[2].GetRank() != rhsSortedHand[2].GetRank())
            {
                return comp(lhsSortedHand[2].GetRank(), rhsSortedHand[2].GetRank());
            }
        } break;
            
        case HandKind::STRAIGHT:
        case HandKind::STRAIGHT_FLUSH:
        {
            // If either hand has an ace, we skip its check and move on to
            // the next highest card. This is due to the A->5 special case
            auto skipNextCard = lhsSortedHand.back().GetRank() == CardRank::ACE || rhsSortedHand.back().GetRank() == CardRank::ACE;
            
            for (int i = HAND_SIZE - 1; i >= 0; i--)
            {
                if (skipNextCard)
                {
                    skipNextCard = false;
                    continue;
                }
                
                if (lhsSortedHand[i].GetRank() != rhsSortedHand[i].GetRank())
                {
                    return comp(lhsSortedHand[i].GetRank(), rhsSortedHand[i].GetRank());
                }
            }
            return false;
        } break;

        case HandKind::FULL_HOUSE:
        {
            // The third card will always be part of the tripplet in a
            // sorted hand
            if (lhsSortedHand[2].GetRank() != rhsSortedHand[2].GetRank())
            {
                return comp(lhsSortedHand[2].GetRank(), rhsSortedHand[2].GetRank());
            }
            
            // otherwise the pair will either be on the second or fourth place
            // for both hands. If the second and third card are the same it
            // means the pair is at the end, whilst if the third and fourth card
            // are the same it means the pair is at the start
            auto lhsPairRank = lhsSortedHand[1].GetRank() == lhsSortedHand[2].GetRank() ? lhsSortedHand.back().GetRank() : lhsSortedHand.front().GetRank();
            auto rhsPairRank = rhsSortedHand[1].GetRank() == rhsSortedHand[2].GetRank() ? rhsSortedHand.back().GetRank() : rhsSortedHand.front().GetRank();
            
            if (lhsPairRank != rhsPairRank)
            {
                return comp(lhsPairRank, rhsPairRank);
            }
        } break;

        case HandKind::FOUR_OF_A_KIND:
        {
            auto lhsFourOfAKindRank = lhsSortedHand[0].GetRank() == lhsSortedHand[1].GetRank() ? lhsSortedHand[0].GetRank() : lhsSortedHand.back().GetRank();
            auto rhsFourOfAKindRank = rhsSortedHand[0].GetRank() == rhsSortedHand[1].GetRank() ? rhsSortedHand[0].GetRank() : rhsSortedHand.back().GetRank();
            
            if (lhsFourOfAKindRank != rhsFourOfAKindRank)
            {
                return comp(lhsFourOfAKindRank, rhsFourOfAKindRank);
            }
        } break;
    }
    
    return DescendingHandComparison(lhsSortedHand, rhsSortedHand, comp);
}

///------------------------------------------------------------------------------------------------

bool operator < (const Hand& lhs, const Hand& rhs)
{
    return HandComparison(lhs, rhs, std::less<>());
}

bool operator > (const Hand& lhs, const Hand& rhs)
{
    return HandComparison(lhs, rhs, std::greater<>());
}

bool operator == (const Hand& lhs, const Hand& rhs)
{
    if (lhs.GetHandKind() != rhs.GetHandKind())
    {
        return false;
    }
    
    return (!(lhs < rhs) && !(lhs > rhs) && !(rhs < lhs) && !(rhs > lhs));
}

bool operator != (const Hand& lhs, const Hand& rhs)
{
    return !(lhs == rhs);
}

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

