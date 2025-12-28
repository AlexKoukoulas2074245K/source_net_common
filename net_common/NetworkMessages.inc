///------------------------------------------------------------------------------------------------
///  NetworkMessages.inc
///  TinyMMOCommon
///
///  Created by Alex Koukoulas on 26/12/2025
///------------------------------------------------------------------------------------------------

BEGIN_MESSAGE(AssignPlayerIdMessage)
FIELD(playerId, uint32_t)
END_MESSAGE()

BEGIN_MESSAGE(PlayerDisconnectedMessage)
FIELD(playerId, uint32_t)
END_MESSAGE()

BEGIN_MESSAGE(MoveMessage)
FIELD(playerId, uint32_t)
FIELD(position, glm::vec2)
FIELD(velocity, glm::vec2)
FIELD(animationIndex, int)
END_MESSAGE()

BEGIN_MESSAGE(SnapshotMessage)
FIELD(playerId, uint32_t)
FIELD(position, glm::vec2)
FIELD(velocity, glm::vec2)
FIELD(animationIndex, int)
END_MESSAGE()

BEGIN_MESSAGE(AttackMessage)
FIELD(attackerId, uint32_t)
END_MESSAGE()

BEGIN_MESSAGE(QuestCompleteMessage)
FIELD(playerId, uint32_t)
FIELD(questId, uint32_t)
END_MESSAGE()
