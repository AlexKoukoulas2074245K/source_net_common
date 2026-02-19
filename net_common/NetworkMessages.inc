///------------------------------------------------------------------------------------------------
///  NetworkMessages.inc
///  TinyMMOCommon
///
///  Created by Alex Koukoulas on 26/12/2025
///------------------------------------------------------------------------------------------------

BEGIN_MESSAGE(PlayerConnectedMessage)
FIELD(objectId, objectId_t)
END_MESSAGE()

BEGIN_MESSAGE(PlayerDisconnectedMessage)
FIELD(objectId, objectId_t)
END_MESSAGE()

BEGIN_MESSAGE(ObjectCreatedMessage)
FIELD(objectData, ObjectData)
END_MESSAGE()

BEGIN_MESSAGE(ObjectDestroyedMessage)
FIELD(objectId, objectId_t)
END_MESSAGE()

BEGIN_MESSAGE(ObjectStateUpdateMessage)
FIELD(objectData, ObjectData)
END_MESSAGE()

BEGIN_MESSAGE(BeginAttackRequestMessage)
FIELD(attackerId, objectId_t)
FIELD(attackType, AttackType)
FIELD(projectileType, ProjectileType)
END_MESSAGE()

BEGIN_MESSAGE(CancelAttackMessage)
FIELD(attackerId, objectId_t)
END_MESSAGE()

BEGIN_MESSAGE(BeginAttackResponseMessage)
FIELD(attackerId, objectId_t)
FIELD(attackType, AttackType)
FIELD(projectileType, ProjectileType)
FIELD(allowed, bool)
FIELD(chargeDurationSecs, float)
END_MESSAGE()

BEGIN_MESSAGE(DebugGetQuadtreeRequestMessage)
END_MESSAGE()

BEGIN_MESSAGE(DebugGetQuadtreeResponseMessage)
FIELD(quadtreeData, DebugQuadtreeRequestData)
END_MESSAGE()


BEGIN_MESSAGE(DebugGetObjectPathRequestMessage)
FIELD(objectId, objectId_t)
END_MESSAGE()

BEGIN_MESSAGE(DebugGetObjectPathResponseMessage)
FIELD(objectId, objectId_t)
FIELD(pathData, DebugObjectPathRequestData)
END_MESSAGE()
