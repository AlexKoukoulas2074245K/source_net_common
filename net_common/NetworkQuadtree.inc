
inline constexpr int MAX_OBJECTS_PER_NODE = 2;
inline constexpr int MAX_DEPTH = 5;

///-----------------------------------------------------------------------------------------------

NetworkQuadtree::NetworkQuadtree(const glm::vec3& position, const glm::vec3& dimensions, const int depth /* 0 */)
    : mOrigin(position)
    , mDimensions(dimensions)
    , mDepth(depth)
{
    for (int i = 0; i < 4; ++i)
    {
        mNodes[i] = nullptr;
    }
    
    InternalClear();
}

///-----------------------------------------------------------------------------------------------

NetworkQuadtree::~NetworkQuadtree()
{
    InternalClear();
}

///-----------------------------------------------------------------------------------------------

const glm::vec3& NetworkQuadtree::GetOrigin() const
{
    return mOrigin;
}

///-----------------------------------------------------------------------------------------------

const glm::vec3& NetworkQuadtree::GetDimensions() const
{
    return mDimensions;
}

///-----------------------------------------------------------------------------------------------

std::vector<objectId_t> NetworkQuadtree::GetCollisionCandidates(const ObjectData& objectData) const
{
    std::vector<objectId_t> collisionCandidates;
    InternalGetCollisionCandidates(objectData, collisionCandidates);
    return collisionCandidates;
}

///-----------------------------------------------------------------------------------------------

void NetworkQuadtree::PopulateSceneGraph(const std::vector<ObjectData>& netObjectData)
{
    for (const auto& objectData: netObjectData)
    {
        glm::vec3 colliderDimensions(objectData.colliderData.colliderRelativeDimentions.x * objectData.objectScale, objectData.colliderData.colliderRelativeDimentions.y * objectData.objectScale, 1.0f);
        InsertObject(objectData.objectId, objectData.position, colliderDimensions);
    }
}

///-----------------------------------------------------------------------------------------------

void NetworkQuadtree::Clear()
{
    InternalClear();
}

///-----------------------------------------------------------------------------------------------

std::vector<std::pair<glm::vec3, glm::vec3>> NetworkQuadtree::GetDebugRenderRectangles() const
{
    std::vector<std::pair<glm::vec3, glm::vec3>> debugRectangles;
    InternalGetDebugRenderRectangles(debugRectangles);
    return debugRectangles;
}

///-----------------------------------------------------------------------------------------------

std::string NetworkQuadtree::GetFullMatchedQuadrantPositionString(const glm::vec3& objectPosition, const glm::vec3& objectDimensions) const
{
    std::string result;
    InternalGetMatchedQuadrantPositionString(objectPosition, objectDimensions, result);
    return result;
}

///-----------------------------------------------------------------------------------------------

void NetworkQuadtree::InternalClear()
{
    mObjectsInNode.clear();
    for (int i = 0; i < 4; ++i)
    {
        if (mNodes[i] != nullptr)
        {
            mNodes[i]->InternalClear();
        }
        mNodes[i] = nullptr;
    }
}

///-----------------------------------------------------------------------------------------------

void NetworkQuadtree::InternalGetCollisionCandidates(const ObjectData& objectData, std::vector<objectId_t>& collisionCandidates) const
{
    if (mNodes[0] != nullptr)
    {
        glm::vec3 colliderDimensions(objectData.colliderData.colliderRelativeDimentions.x * objectData.objectScale, objectData.colliderData.colliderRelativeDimentions.y * objectData.objectScale, 1.0f);
        
        const auto quadrantIndex = GetMatchedQuadrant(objectData.position,  colliderDimensions);
        if (quadrantIndex != -1)
        {
            mNodes[quadrantIndex]->InternalGetCollisionCandidates(objectData, collisionCandidates);
        }
        else
        {
            if (mNodes[0] != nullptr)
            {
                for (int i = 0; i < 4; ++i)
                {
                    mNodes[i]->InternalGetCollisionCandidates(objectData, collisionCandidates);
                }
            }
        }
    }
    
    auto objectsIter = mObjectsInNode.begin();
    while (objectsIter != mObjectsInNode.end())
    {
        if (objectsIter->mObjectId != objectData.objectId)
        {
            collisionCandidates.push_back(objectsIter->mObjectId);
        }
        objectsIter++;
    }
}

///-----------------------------------------------------------------------------------------------

void NetworkQuadtree::InternalGetDebugRenderRectangles(std::vector<std::pair<glm::vec3, glm::vec3>>& debugRectangles) const
{
    const auto debugRectOrigin = glm::vec3(mOrigin.x, mOrigin.y, mOrigin.z);
    debugRectangles.push_back(std::make_pair(debugRectOrigin, mDimensions));
    if (mNodes[0] != nullptr)
    {
        for (int i = 0; i < 4; ++i)
        {
            mNodes[i]->InternalGetDebugRenderRectangles(debugRectangles);
        }
    }
}

///-----------------------------------------------------------------------------------------------

void NetworkQuadtree::InternalGetMatchedQuadrantPositionString(const glm::vec3& objectPosition, const glm::vec3& objectDimensions, std::string& positionString) const
{
    if (mNodes[0] == nullptr)
    {
        return;
    }
    
    const auto quadrantIndex = GetMatchedQuadrant(objectPosition,  objectDimensions);
    if (quadrantIndex != -1)
    {
        if (!positionString.empty()) positionString += "->";
        positionString += std::to_string(quadrantIndex);
        mNodes[quadrantIndex]->InternalGetMatchedQuadrantPositionString(objectPosition, objectDimensions, positionString);
    }
}

///-----------------------------------------------------------------------------------------------
void NetworkQuadtree::Split()
{
    const auto halfWidth  = mDimensions.x * 0.5f;
    const auto halfHeight = mDimensions.y * 0.5f;
    const auto quadWidth  = halfWidth * 0.5f;
    const auto quadHeight = halfHeight * 0.5f;
    
    // Push inner quads a bit towards viewer to avoid z-fighting in debugging
    const auto zFrontPush = (mDepth + 1) * 0.0001f;
    
    mNodes[0] = std::make_unique<NetworkQuadtree>(glm::vec3(mOrigin.x - quadWidth, mOrigin.y + quadHeight, mOrigin.z - zFrontPush), glm::vec3(halfWidth, halfHeight, 0.0f), mDepth + 1);
    mNodes[1] = std::make_unique<NetworkQuadtree>(glm::vec3(mOrigin.x + quadWidth, mOrigin.y + quadHeight, mOrigin.z - zFrontPush), glm::vec3(halfWidth, halfHeight, 0.0f), mDepth + 1);
    mNodes[2] = std::make_unique<NetworkQuadtree>(glm::vec3(mOrigin.x - quadWidth, mOrigin.y - quadHeight, mOrigin.z - zFrontPush), glm::vec3(halfWidth, halfHeight, 0.0f), mDepth + 1);
    mNodes[3] = std::make_unique<NetworkQuadtree>(glm::vec3(mOrigin.x + quadWidth, mOrigin.y - quadHeight, mOrigin.z - zFrontPush), glm::vec3(halfWidth, halfHeight, 0.0f), mDepth + 1);
}

///-----------------------------------------------------------------------------------------------

int NetworkQuadtree::GetMatchedQuadrant(const glm::vec3& objectPosition, const glm::vec3& objectDimensions) const
{
    int quadrant = -1;
    
    const auto objectHalfWidth = objectDimensions.x * 0.5f;
    const auto objectHalfHeight = objectDimensions.y * 0.5f;
    const auto halfSpaceVerticalLinePoint = mOrigin.x;
    const auto halfSpaceHorizontalLinePoint = mOrigin.y;
    
    // completely fits in the left halfspace region of the current node
    if (objectPosition.x + objectHalfWidth < halfSpaceVerticalLinePoint)
    {
        // completely fits in the bottom left quadrant
        if (objectPosition.y + objectHalfHeight < halfSpaceHorizontalLinePoint)
        {
            quadrant = 2;
        }
        // completly fits in the top left quadrant
        else if (objectPosition.y - objectHalfHeight > halfSpaceHorizontalLinePoint)
        {
            quadrant = 0;
        }
    }
    // completely fits in the right halfspace region of the current node
    else if (objectPosition.x - objectHalfWidth > halfSpaceVerticalLinePoint)
    {
        // completely fits in the bottom right quadrant
        if (objectPosition.y + objectHalfHeight < halfSpaceHorizontalLinePoint)
        {
            quadrant = 3;
        }
        // completly fits in the top right quadrant
        else if (objectPosition.y - objectHalfHeight > halfSpaceHorizontalLinePoint)
        {
            quadrant = 1;
        }
    }
    
    return quadrant;
}

///-----------------------------------------------------------------------------------------------

void NetworkQuadtree::InsertObject(const objectId_t objectId, const glm::vec3& position, const glm::vec3& dimensions)
{
    const auto isQuadtreeSplit = mNodes[0] != nullptr;
    
    if (isQuadtreeSplit)
    {
        const auto quadrantIndex = GetMatchedQuadrant(position, dimensions);
        
        if (quadrantIndex != -1)
        {
            mNodes[quadrantIndex]->InsertObject(objectId, position, dimensions);
            return;
        }
    }
    
    mObjectsInNode.push_back(QuadtreeEntityEntry(objectId, position, dimensions));
    
    if (mObjectsInNode.size() > MAX_OBJECTS_PER_NODE && mDepth < MAX_DEPTH)
    {
        if (!isQuadtreeSplit)
        {
            Split();
        }
        
        auto objectsIter = mObjectsInNode.begin();
        while (objectsIter != mObjectsInNode.end())
        {
            const auto objectQuadrantIndex = GetMatchedQuadrant(objectsIter->mObjectPosition, objectsIter->mObjectDimensions);
            if (objectQuadrantIndex != -1)
            {
                mNodes[objectQuadrantIndex]->InsertObject(objectsIter->mObjectId, objectsIter->mObjectPosition, objectsIter->mObjectDimensions);
                objectsIter = mObjectsInNode.erase(objectsIter);
            }
            else
            {
                objectsIter++;
            }
        }
    }
}

///-----------------------------------------------------------------------------------------------

