///------------------------------------------------------------------------------------------------
///  SymbolDataRepository.h
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 28/02/2025
///------------------------------------------------------------------------------------------------

#ifndef SymbolDataRepository_h
#define SymbolDataRepository_h

///------------------------------------------------------------------------------------------------

#include "Symbols.h"
#include <vector>
#include <unordered_map>

///------------------------------------------------------------------------------------------------

namespace slots
{

///------------------------------------------------------------------------------------------------

class SymbolDataRepository final
{
public:
    static SymbolDataRepository& GetInstance();
    ~SymbolDataRepository() = default;
    
    SymbolDataRepository(const SymbolDataRepository&) = delete;
    SymbolDataRepository(SymbolDataRepository&&) = delete;
    const SymbolDataRepository& operator = (const SymbolDataRepository&) = delete;
    SymbolDataRepository& operator = (SymbolDataRepository&&) = delete;
    
    float GetSymbolWinMultiplier(const SymbolType symbolType, const int matchingCount) const;
private:
    SymbolDataRepository();
    
    void InitWithDefaultConfig();
    
private:
    std::vector<std::unordered_map<SymbolType, float>> mSymbolWinMultipliers;
};

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

#endif /* SymbolDataRepository_h */

