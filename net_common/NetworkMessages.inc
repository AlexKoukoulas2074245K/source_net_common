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
FIELD(colliderData, ObjectColliderData)
END_MESSAGE()

BEGIN_MESSAGE(ObjectDestroyedMessage)
FIELD(objectId, objectId_t)
END_MESSAGE()

BEGIN_MESSAGE(ObjectStateUpdateMessage)
FIELD(objectData, ObjectData)
END_MESSAGE()

BEGIN_MESSAGE(AttackMessage)
FIELD(attackerId, objectId_t)
FIELD(attackType, AttackType)
FIELD(projectileType, ProjectileType)
END_MESSAGE()
