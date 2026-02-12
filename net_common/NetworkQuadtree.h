///------------------------------------------------------------------------------------------------
///  NetworkQuadtree.h
///  TinyMMOClient
///                                                                                                
///  Created by Alex Koukoulas on 07/02/2026
///------------------------------------------------------------------------------------------------

#ifndef NetworkQuadtree_h
#define NetworkQuadtree_h

///------------------------------------------------------------------------------------------------

#if __has_include(<engine/utils/MathUtils.h>)
#include <engine/utils/MathUtils.h>
#else
#include "../util/MathUtils.h"
#endif

#include <net_common/NetworkCommon.h>
#include <vector>

///-----------------------------------------------------------------------------------------------

namespace network
{

///-----------------------------------------------------------------------------------------------

class NetworkQuadtree final
{
public:
    NetworkQuadtree(const glm::vec3& origin, const glm::vec3& dimensions, const int depth = 0);
    ~NetworkQuadtree();
    
    const glm::vec3& GetOrigin() const;
    const glm::vec3& GetDimensions() const;
    std::vector<objectId_t> GetCollisionCandidates(const ObjectData& objectData) const;
    void InsertObject(const objectId_t objectId, const glm::vec3& position, const glm::vec3& dimensions);
    void PopulateSceneGraph(const std::vector<ObjectData>& netObjectData);
    void Clear();
    int GetMatchedQuadrant(const glm::vec3& objectPosition, const glm::vec3& objectDimensions) const;

    std::vector<std::pair<glm::vec3, glm::vec3>> GetDebugRenderRectangles() const;
    std::string GetFullMatchedQuadrantPositionString(const glm::vec3& objectPosition, const glm::vec3& objectDimensions) const;

private:
    struct QuadtreeEntityEntry
    {
        QuadtreeEntityEntry(const objectId_t objectId, const glm::vec3& position, const glm::vec3& dimensions)
            : mObjectId(objectId)
            , mObjectPosition(position)
            , mObjectDimensions(dimensions)
        {
        }
        
        objectId_t mObjectId;
        glm::vec3 mObjectPosition;
        glm::vec3 mObjectDimensions;
    };
    
    void InternalClear();
    void InternalGetCollisionCandidates(const ObjectData& objectData, std::vector<objectId_t>& collisionCandidates) const;
    void InternalGetDebugRenderRectangles(std::vector<std::pair<glm::vec3, glm::vec3>>& debugRectangles) const;
    void InternalGetMatchedQuadrantPositionString(const glm::vec3& objectPosition, const glm::vec3& objectDimensions, std::string& positionString) const;
    void Split();
    
private:
    const glm::vec3 mOrigin;
    const glm::vec3 mDimensions;
    const int mDepth;
    
    std::unique_ptr<NetworkQuadtree> mNodes[4];
    std::vector<QuadtreeEntityEntry> mObjectsInNode;
};

///------------------------------------------------------------------------------------------------

#include "NetworkQuadtree.inc"

};
#endif /* NetworkQuadtree_h */
