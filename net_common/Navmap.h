///------------------------------------------------------------------------------------------------
///  Navmap.h
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 19/05/2024
///------------------------------------------------------------------------------------------------

#ifndef Navmap_h
#define Navmap_h

///------------------------------------------------------------------------------------------------

#include <vector>

///------------------------------------------------------------------------------------------------

namespace networking
{

///------------------------------------------------------------------------------------------------

enum class NavmapTileType
{
    VOID,
    EMPTY,
    SOLID,
    WATER
};

///------------------------------------------------------------------------------------------------

class Navmap
{
public:
    Navmap(const std::vector<unsigned char>& navmapPixels, const int navmapSize)
    : mNavmapPixels(navmapPixels)
    , mNavmapSize(navmapSize)
    {}
    
    inline NavmapTileType GetNavmapTileAt(const int x, const int y)
    {
        int r = mNavmapPixels[(mNavmapSize * 4) * y + x * 4 + 0];
        int g = mNavmapPixels[(mNavmapSize * 4) * y + x * 4 + 1];
        int b = mNavmapPixels[(mNavmapSize * 4) * y + x * 4 + 2];
        int a = mNavmapPixels[(mNavmapSize * 4) * y + x * 4 + 3];
        
        if (a == 0)
        {
            return NavmapTileType::VOID;
        }
        
        if (r == 0 && g == 0 && b > 0)
        {
            return NavmapTileType::WATER;
        }
        else if (r > 0 && g > 0 && b > 0)
        {
            return NavmapTileType::EMPTY;
        }
        
        return NavmapTileType::SOLID;
    }
    
private:
    const std::vector<unsigned char> mNavmapPixels;
    const int mNavmapSize;
};

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

#endif /* Navmap_h */
