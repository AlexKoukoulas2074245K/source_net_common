///------------------------------------------------------------------------------------------------
///  BestHandFinder.cpp
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 7/11/2024
///------------------------------------------------------------------------------------------------

#include "BestHandFinder.h"

#include <algorithm>
#include <vector>
///------------------------------------------------------------------------------------------------

namespace poker
{

///------------------------------------------------------------------------------------------------

static bool DoesPoolContainExactCard(const std::array<Card, CARD_POOL_SIZE>& sortedCardPool, const Card& card)
{
    for (int i = 0; i < CARD_POOL_SIZE; ++i)
    {
        if (sortedCardPool[i] == card)
        {
            return true;
        }
    }
    
    return false;
}

static std::optional<Card> FindCardWithRank(const std::array<Card, CARD_POOL_SIZE>& sortedCardPool, const CardRank rank)
{
    for (int i = 0; i < CARD_POOL_SIZE; ++i)
    {
        if (sortedCardPool[i].GetRank() == rank)
        {
            return std::optional<Card>(sortedCardPool[i]);
        }
    }
    
    return std::optional<Card>();
}

static bool DoesPoolContainAllExactCards(const std::array<Card, CARD_POOL_SIZE>& sortedCardPool, const std::array<Card, HAND_SIZE>& cards)
{
    for (const auto& card: cards)
    {
        if (!DoesPoolContainExactCard(sortedCardPool, card))
        {
            return false;
        }
    }
    
    return true;
}

static std::vector<Card> FindAllCardsWithRank(const std::array<Card, CARD_POOL_SIZE>& sortedCardPool, const CardRank cardRank)
{
    std::vector<Card> result;
    for (const auto& card: sortedCardPool)
    {
        if (card.GetRank() == cardRank)
        {
            result.push_back(card);
        }
    }

    return result;
}

static std::vector<Card> FindAllCardsWithSuit(const std::array<Card, CARD_POOL_SIZE>& sortedCardPool, const CardSuit cardSuit)
{
    std::vector<Card> result;
    for (const auto& card: sortedCardPool)
    {
        if (card.GetSuit() == cardSuit)
        {
            result.push_back(card);
        }
    }

    return result;
}

///------------------------------------------------------------------------------------------------

Hand BestHandFinder::FindBestHand(const std::array<Card, CARD_POOL_SIZE>& cardPool)
{
    auto sortedCardPool = cardPool;
    std::sort(sortedCardPool.begin(), sortedCardPool.end());
    
    // Test for Royal Flush
    for (const auto& suit: { CardSuit::SPADE, CardSuit::HEART, CardSuit::DIAMOND, CardSuit::CLUB})
    {
        std::array<Card, HAND_SIZE> cards =
        {
            Card(CardRank::TEN, suit),
            Card(CardRank::JACK, suit),
            Card(CardRank::QUEEN, suit),
            Card(CardRank::KING, suit),
            Card(CardRank::ACE, suit)
        };
        
        if (DoesPoolContainAllExactCards(sortedCardPool, cards))
        {
            return Hand(cards, HandKind::ROYAL_FLUSH);
        }
    }
    
    // Test for A->5 Straight Flush
    std::vector<Card> aceToFiveStraightFlush;
    for (const auto& suit: { CardSuit::SPADE, CardSuit::HEART, CardSuit::DIAMOND, CardSuit::CLUB})
    {
        std::array<Card, HAND_SIZE> cards =
        {
            Card(CardRank::ACE, suit),
            Card(CardRank::TWO, suit),
            Card(CardRank::THREE, suit),
            Card(CardRank::FOUR, suit),
            Card(CardRank::FIVE, suit)
        };
        
        if (DoesPoolContainAllExactCards(sortedCardPool, cards))
        {
            aceToFiveStraightFlush.insert(aceToFiveStraightFlush.begin(), cards.cbegin(), cards.cend());
            break;
        }
    }
    
    // Test for other Straight Flush variants
    std::vector<Card> speculativeStraightFlushCards;
    for (int i = CARD_POOL_SIZE - 1; i >= 0; --i)
    {
        // No possible straight <= 5 at this point (A->5 handled above)
        if (sortedCardPool[i].GetRankValue() <= 5)
        {
            break;
        }
        
        std::array<Card, HAND_SIZE> cards =
        {
            Card(static_cast<CardRank>(sortedCardPool[i].GetRankValue() - 4), sortedCardPool[i].GetSuit()),
            Card(static_cast<CardRank>(sortedCardPool[i].GetRankValue() - 3), sortedCardPool[i].GetSuit()),
            Card(static_cast<CardRank>(sortedCardPool[i].GetRankValue() - 2), sortedCardPool[i].GetSuit()),
            Card(static_cast<CardRank>(sortedCardPool[i].GetRankValue() - 1), sortedCardPool[i].GetSuit()),
            Card(static_cast<CardRank>(sortedCardPool[i].GetRankValue() - 0), sortedCardPool[i].GetSuit())
        };
        
        if (DoesPoolContainAllExactCards(sortedCardPool, cards))
        {
            speculativeStraightFlushCards.insert(speculativeStraightFlushCards.begin(), cards.cbegin(), cards.cend());
            break;
        }
    }
    
    // Achieved straight flush higher than A->5
    if (speculativeStraightFlushCards.size() == HAND_SIZE)
    {
        return Hand({speculativeStraightFlushCards[0], speculativeStraightFlushCards[1], speculativeStraightFlushCards[2], speculativeStraightFlushCards[3], speculativeStraightFlushCards[4]}, HandKind::STRAIGHT_FLUSH);
    }
    
    // Achieved A->5 straight flush
    if (aceToFiveStraightFlush.size() == HAND_SIZE)
    {
        return Hand({aceToFiveStraightFlush[0], aceToFiveStraightFlush[1], aceToFiveStraightFlush[2], aceToFiveStraightFlush[3], aceToFiveStraightFlush[4]}, HandKind::STRAIGHT_FLUSH);
    }
    
    // Test for Quads
    for (int i = CARD_POOL_SIZE - 1; i >= 0; --i)
    {
        auto currentCardRank = sortedCardPool[i].GetRank();
        
        if (DoesPoolContainExactCard(sortedCardPool, Card(currentCardRank, CardSuit::SPADE)) &&
            DoesPoolContainExactCard(sortedCardPool, Card(currentCardRank, CardSuit::CLUB)) &&
            DoesPoolContainExactCard(sortedCardPool, Card(currentCardRank, CardSuit::DIAMOND)) &&
            DoesPoolContainExactCard(sortedCardPool, Card(currentCardRank, CardSuit::HEART)))
        {
            for (int j = CARD_POOL_SIZE - 1; j >= 0; --j)
            {
                if (sortedCardPool[j].GetRank() != currentCardRank)
                {
                    return Hand({Card(currentCardRank, CardSuit::SPADE), Card(currentCardRank, CardSuit::CLUB), Card(currentCardRank, CardSuit::DIAMOND), Card(currentCardRank, CardSuit::HEART), sortedCardPool[j]}, HandKind::FOUR_OF_A_KIND);
                }
            }
        }
    }
    
    // Test for Full House
    std::vector<Card> highestTrippletsFound;
    std::vector<Card> highestPairFound;
    
    for (int i = CARD_POOL_SIZE - 1; i >= 0; --i)
    {
        auto currentCardRank = sortedCardPool[i].GetRank();
        const auto& allCardsWithRank = FindAllCardsWithRank(sortedCardPool, currentCardRank);
        
        if (highestTrippletsFound.empty() && allCardsWithRank.size() == 3)
        {
            highestTrippletsFound = allCardsWithRank;
            continue;
        }
        
        if (highestPairFound.empty() && allCardsWithRank.size() == 2)
        {
            highestPairFound = allCardsWithRank;
            continue;
        }
    }
    
    if (!highestTrippletsFound.empty() && !highestPairFound.empty())
    {
        return Hand({highestTrippletsFound[0], highestTrippletsFound[1], highestTrippletsFound[2], highestPairFound[0], highestPairFound[1]}, HandKind::FULL_HOUSE);
    }
    
    
    // Test for Flush
    for (int i = CARD_POOL_SIZE - 1; i >= 0; --i)
    {
        auto currentCardSuit = sortedCardPool[i].GetSuit();
        const auto& allCardsWithSuit = FindAllCardsWithSuit(sortedCardPool, currentCardSuit);
        
        if (allCardsWithSuit.size() >= 5)
        {
            return Hand({
                allCardsWithSuit[allCardsWithSuit.size() - 5],
                allCardsWithSuit[allCardsWithSuit.size() - 4],
                allCardsWithSuit[allCardsWithSuit.size() - 3],
                allCardsWithSuit[allCardsWithSuit.size() - 2],
                allCardsWithSuit[allCardsWithSuit.size() - 1]}, HandKind::FLUSH);
        }
    }
    
    // Test for straight
    std::vector<Card> aceToFiveStraight;
    for (const auto& rank: { CardRank::ACE, CardRank::TWO, CardRank::THREE, CardRank::FOUR, CardRank::FIVE})
    {
        auto foundCard = FindCardWithRank(sortedCardPool, rank);
        if (foundCard)
        {
            aceToFiveStraight.push_back(*foundCard);
        }
    }
    
    // Test for other Straight Flush variants
    std::vector<Card> speculativeStraightCards;
    for (int i = CARD_POOL_SIZE - 1; i >= 0; --i)
    {
        // No possible straight <= 5 at this point (A->5 handled above)
        if (sortedCardPool[i].GetRankValue() <= 5)
        {
            break;
        }
        
        for (int j = 0; j < 5; ++j)
        {
            auto foundCard = FindCardWithRank(sortedCardPool, static_cast<CardRank>(sortedCardPool[i].GetRankValue() - j));
            if (foundCard)
            {
                speculativeStraightCards.push_back(*foundCard);
            }
            else
            {
                speculativeStraightCards.clear();
                break;
            }
        }
        
        if (speculativeStraightCards.size() == HAND_SIZE)
        {
            break;
        }
    }
    
    // Achieved straight higher than A->5
    if (speculativeStraightCards.size() == HAND_SIZE)
    {
        return Hand({speculativeStraightCards[4], speculativeStraightCards[3], speculativeStraightCards[2], speculativeStraightCards[1], speculativeStraightCards[0]}, HandKind::STRAIGHT);
    }
    
    // Achieved A->5 straight
    if (aceToFiveStraight.size() == HAND_SIZE)
    {
        return Hand({aceToFiveStraight[0], aceToFiveStraight[1], aceToFiveStraight[2], aceToFiveStraight[3], aceToFiveStraight[4]}, HandKind::STRAIGHT);
    }
    
    // Test Three of A Kind
    if (!highestTrippletsFound.empty())
    {
        std::vector<Card> constructedHand(highestTrippletsFound.begin(), highestTrippletsFound.end());
        std::vector<Card> kickerCards;
        
        auto trippletRank = highestTrippletsFound.front().GetRank();
        
        for (int i = CARD_POOL_SIZE - 1; i >= 0; --i)
        {
            if (sortedCardPool[i].GetRank() != trippletRank && kickerCards.size() < 2)
            {
                kickerCards.insert(kickerCards.begin(), sortedCardPool[i]);
            }
        }
        return Hand({highestTrippletsFound[0], highestTrippletsFound[1], highestTrippletsFound[2], kickerCards[0], kickerCards[1]}, HandKind::THREE_OF_A_KIND);
    }
    
    // Test two pair
    std::vector<Card> topPair;
    std::vector<Card> bottomPair;
    
    for (int i = CARD_POOL_SIZE - 1; i >= 0; --i)
    {
        auto currentCardRank = sortedCardPool[i].GetRank();
        const auto& allCardsWithRank = FindAllCardsWithRank(sortedCardPool, currentCardRank);
        
        if (allCardsWithRank.size() == 2)
        {
            if (topPair.empty())
            {
                topPair = allCardsWithRank;
            }
            else if (bottomPair.empty() && topPair.front().GetRank() != currentCardRank)
            {
                bottomPair = allCardsWithRank;
            }
        }
    }
    
    // Test 1/2 Pair
    if (!topPair.empty())
    {
        if (!bottomPair.empty())
        {
            for (int i = CARD_POOL_SIZE - 1; i >= 0; --i)
            {
                const auto& currentCard = sortedCardPool[i];
                if (topPair.front().GetRank() != currentCard.GetRank() && bottomPair.front().GetRank() != currentCard.GetRank())
                {
                    return Hand({topPair[0], topPair[1], bottomPair[0], bottomPair[1], currentCard}, HandKind::TWO_PAIR);
                }
            }
        }
        else
        {
            std::vector<Card> constructedHand(topPair.begin(), topPair.end());
            std::vector<Card> kickerCards;
            
            auto topPairRank = topPair.front().GetRank();
            
            for (int i = CARD_POOL_SIZE - 1; i >= 0; --i)
            {
                if (sortedCardPool[i].GetRank() != topPairRank && kickerCards.size() < 3)
                {
                    kickerCards.insert(kickerCards.begin(), sortedCardPool[i]);
                }
            }
            return Hand({topPair[0], topPair[1], kickerCards[0], kickerCards[1], kickerCards[2]}, HandKind::ONE_PAIR);
        }
    }
    
    // All other options are exhausted return just the top 5 cards of the pool
    return Hand({sortedCardPool[2], sortedCardPool[3], sortedCardPool[4], sortedCardPool[5], sortedCardPool[6]}, HandKind::HIGH_CARD);
}

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

