///------------------------------------------------------------------------------------------------
///  Navmap.h
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 19/05/2024
///------------------------------------------------------------------------------------------------

#ifndef Navmap_h
#define Navmap_h

///------------------------------------------------------------------------------------------------

#if __has_include(<engine/utils/MathUtils.h>)
#include <engine/utils/MathUtils.h>
#else
#include "../util/MathUtils.h"
#endif
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
    
    inline glm::vec3 GetMapPositionFromNavmapCoord(const glm::ivec2& navmapCoord, const glm::vec2& mapPosition, const float mapScale, const float positionZ) const
    {
        return glm::vec3(mapScale * ((static_cast<float>(navmapCoord.x))/mNavmapSize - 0.5f) + (mapPosition.x * mapScale),
                         mapScale * (0.5f - (static_cast<float>(navmapCoord.y))/mNavmapSize) + (mapPosition.y * mapScale),
                         positionZ);
    }
    
    inline glm::ivec2 GetNavmapCoord(const glm::vec3& objectPosition, const glm::vec2& mapPosition, const float mapScale) const
    {
        return glm::ivec2(static_cast<int>(((objectPosition.x - (mapPosition.x * mapScale))/mapScale + 0.5f) * mNavmapSize),
                          static_cast<int>((1.0f - ((objectPosition.y - (mapPosition.y * mapScale))/mapScale + 0.5f)) * mNavmapSize));
    }
    
    inline NavmapTileType GetNavmapTileAt(const glm::ivec2& navmapCoord) const
    {
        unsigned char r = mNavmapPixels[(mNavmapSize * 4) * navmapCoord.y + navmapCoord.x * 4 + 0];
        unsigned char g = mNavmapPixels[(mNavmapSize * 4) * navmapCoord.y + navmapCoord.x * 4 + 1];
        unsigned char b = mNavmapPixels[(mNavmapSize * 4) * navmapCoord.y + navmapCoord.x * 4 + 2];
        unsigned char a = mNavmapPixels[(mNavmapSize * 4) * navmapCoord.y + navmapCoord.x * 4 + 3];
        
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
    
    inline int GetSize() const { return mNavmapSize; }
    
private:
    const unsigned char* mNavmapPixels;
    const int mNavmapSize;
};

///------------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

#endif /* Navmap_h */
