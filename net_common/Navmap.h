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
/// Simple utility & lookup helper class encapsulating a navmap.
/// the pixel data re non-owning here so that the client can easily provide
/// a pointer to the loaded SDL surface data, and the server can just store the copy of
/// the pixels somewhere whilst the navmap is alive.
class Navmap
{
public:
    Navmap(const unsigned char* navmapPixels, const int navmapSize)
    : mNavmapPixels(navmapPixels)
    , mNavmapSize(navmapSize)
    {}
    
    inline NavmapTileType GetNavmapTileAt(const int x, const int y)
    {
        unsigned char r = mNavmapPixels[(mNavmapSize * 4) * y + x * 4 + 0];
        unsigned char g = mNavmapPixels[(mNavmapSize * 4) * y + x * 4 + 1];
        unsigned char b = mNavmapPixels[(mNavmapSize * 4) * y + x * 4 + 2];
        unsigned char a = mNavmapPixels[(mNavmapSize * 4) * y + x * 4 + 3];
        
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
    const unsigned char* mNavmapPixels;
    const int mNavmapSize;
};

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

#endif /* Navmap_h */
