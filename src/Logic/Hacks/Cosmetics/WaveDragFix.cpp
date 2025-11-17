#include "includes.hpp"
using namespace geode::prelude;
// this code was taken from the dragfix mod. I will remvoe it later.
namespace Astral::Cosmetics::WaveDragFix {
    static bool waveDragFixEnabled = true;
    
    bool isEnabled() {
        return waveDragFixEnabled;
    }
    
    void setEnabled(bool enabled) {
        waveDragFixEnabled = enabled;
    }
}

class $modify(WTDFPlayerObject, PlayerObject) {
    struct Fields {
        bool gotHereFromUpdate = false;
        bool forceAdd = true;
        bool spiderPadTriggered = false;
        bool forceAddSpiderPad = false;
        bool forceAddSpiderRing = false;
        bool justTeleported = false;
        bool teleportedPreviouslySpiderRing = false;
        bool transitionToCollision = false;
        float portalTargetLine = 0;
        CCPoint previousPos{-12, -12};
        CCPoint currentPos{-12, -12};
        CCPoint nextPosNoCollision{-12, -12};
        GameObject* collidedSlope = nullptr;
        float deltaFactor = 0;
    };

    static void onModify(auto& self) {
        Result res = self.setHookPriorityAfter("PlayerObject::update", "syzzi.click_between_frames");
        if (!res && res != Err("Mod not found"))
            AstralLogger::log("Failed to set hook priority for WaveDragFix.");
    }

    void resetObject() {
        WTDFPlayerObject::Fields defaultFields;
        *m_fields.operator->() = defaultFields;
        m_waveTrail->reset();
        PlayerObject::resetObject();
    }

    bool preSlopeCollision(float p0, GameObject* p1) {
        bool value = PlayerObject::preSlopeCollision(p0, p1);
        if (!value) m_fields->collidedSlope = p1;
        return value;
    }

    void update(float deltaFactor) {
        m_fields->currentPos = getRealPosition();
        m_fields->gotHereFromUpdate = true;
        PlayerObject::update(deltaFactor);
        m_fields->nextPosNoCollision = getRealPosition();
        m_fields->collidedSlope = nullptr;
        m_fields->deltaFactor = deltaFactor;
    }

    void updateRotation(float p0) {
        PlayerObject::updateRotation(p0);
        
        if (!Astral::Cosmetics::WaveDragFix::isEnabled() || LevelEditorLayer::get() || !m_gameLayer || !m_fields->gotHereFromUpdate) 
            return;
        
        m_fields->gotHereFromUpdate = false;

        if (!m_isDart || m_isHidden) {
            m_fields->previousPos = m_fields->currentPos;
            m_fields->justTeleported = false;
            m_fields->teleportedPreviouslySpiderRing = false;
            m_fields->forceAddSpiderRing = false;
            m_fields->forceAddSpiderPad = false;
            m_fields->spiderPadTriggered = false;
            m_fields->forceAdd = true;
            return;
        }

        CCPoint previousPosition = m_fields->previousPos;
        CCPoint currentPosition = m_fields->currentPos;
        CCPoint nextPositionNoCollision = m_fields->nextPosNoCollision;
        CCPoint nextPosition = getRealPosition();

        if (m_wasTeleported && !m_fields->justTeleported) {
            addWaveTrailPoint(nextPosition);
            return;
        } else if (m_fields->justTeleported) {
            m_fields->justTeleported = false;
            m_fields->teleportedPreviouslySpiderRing = m_fields->forceAddSpiderRing;
            m_fields->forceAddSpiderRing = false;
            m_fields->forceAddSpiderPad = false;
            m_fields->spiderPadTriggered = false;
            m_fields->forceAdd = false;
            m_fields->portalTargetLine = m_isSideways ? previousPosition.y : previousPosition.x;
            addWaveTrailPoint(previousPosition);
            return;
        } else if (m_fields->forceAdd && !m_fields->forceAddSpiderRing && !m_fields->forceAddSpiderPad) {
            m_fields->forceAdd = false;
            addWaveTrailPoint(currentPosition);
            m_fields->previousPos = currentPosition;
            return;
        } else if (m_fields->spiderPadTriggered) {
            m_fields->spiderPadTriggered = false;
            m_fields->forceAddSpiderPad = true;
            m_fields->previousPos = currentPosition;
            return;
        } else if (m_fields->forceAddSpiderRing || m_fields->forceAddSpiderPad) {
            m_fields->forceAddSpiderRing = false;
            m_fields->forceAddSpiderPad = false;
            m_fields->forceAdd = false;
            m_fields->transitionToCollision = false;
            m_fields->teleportedPreviouslySpiderRing = false;
            CCPoint pointToAdd = m_isSideways ? CCPoint{nextPosition.x, previousPosition.y} : CCPoint{previousPosition.x, nextPosition.y};
            addWaveTrailPoint(pointToAdd);
            m_fields->previousPos = pointToAdd;
            return;
        } else if (m_fields->teleportedPreviouslySpiderRing) {
            m_fields->teleportedPreviouslySpiderRing = false;
            CCPoint pointToAdd = m_isSideways ? CCPoint{nextPosition.x, m_fields->portalTargetLine} : CCPoint{m_fields->portalTargetLine, nextPosition.y};
            addWaveTrailPoint(pointToAdd);
            m_fields->previousPos = pointToAdd;
            return;
        }

        cocos2d::CCPoint currentVector = (nextPosition - currentPosition).normalize();
        cocos2d::CCPoint previousVector = (currentPosition - previousPosition).normalize();
        float crossProductMagnitude = abs(currentVector.cross(previousVector));
        float errorMargin = 0.004 / m_fields->deltaFactor;

        if (crossProductMagnitude <= errorMargin) return;

        if (nextPosition != nextPositionNoCollision && !m_fields->transitionToCollision) {
            float objectBoundMin = std::numeric_limits<float>::lowest();
            float objectBoundMax = std::numeric_limits<float>::max();
            float hitboxSizeProbably = getObjectRect().size.width;
            GameObject *object = m_collidedObject ? m_collidedObject : m_fields->collidedSlope;
            
            if (object) {
                objectBoundMin = object->getObjectRect().getMinX();
                objectBoundMax = object->getObjectRect().getMaxX();
            }

            if (m_isSideways) {
                if (object) {
                    objectBoundMin = object->getObjectRect().getMinY();
                    objectBoundMax = object->getObjectRect().getMaxY();
                }
                std::swap(nextPositionNoCollision.x, nextPositionNoCollision.y);
                std::swap(nextPosition.x, nextPosition.y);
                std::swap(currentPosition.x, currentPosition.y);
            }
            
            float intercept = (nextPositionNoCollision.y - currentPosition.y) / (nextPositionNoCollision.x - currentPosition.x);
            float desiredValue = (nextPosition.y - currentPosition.y + intercept * currentPosition.x) / intercept;
            
            if ((desiredValue < currentPosition.x && !m_isGoingLeft) ||
                (desiredValue > currentPosition.x && m_isGoingLeft) ||
                (currentPosition.x + hitboxSizeProbably/2 < objectBoundMin && !m_isGoingLeft) ||
                (currentPosition.x - hitboxSizeProbably/2 > objectBoundMax && m_isGoingLeft)) {
                if (m_isSideways) addWaveTrailPoint({currentPosition.y, currentPosition.x});
                else addWaveTrailPoint(currentPosition);
                desiredValue = currentPosition.x;
            }
            
            CCPoint intersectionPoint{desiredValue, nextPosition.y};
            if (m_isSideways) {
                std::swap(nextPositionNoCollision.x, nextPositionNoCollision.y);
                std::swap(nextPosition.x, nextPosition.y);
                std::swap(currentPosition.x, currentPosition.y);
                std::swap(intersectionPoint.x, intersectionPoint.y);
            }
            addWaveTrailPoint(intersectionPoint);
            m_fields->previousPos = intersectionPoint;
            m_fields->transitionToCollision = true;
        } else {
            if (nextPosition == nextPositionNoCollision) m_fields->transitionToCollision = false;
            addWaveTrailPoint(currentPosition);
            m_fields->previousPos = currentPosition;
        }
    }

    inline void addWaveTrailPoint(CCPoint point) {
        unsigned objectCount = static_cast<unsigned>(m_waveTrail->m_pointArray->count());
        if (objectCount >= 3) {
            CCPoint point0 = static_cast<PointNode *>(m_waveTrail->m_pointArray->objectAtIndex(objectCount - 2))->m_point;
            CCPoint point1 = static_cast<PointNode *>(m_waveTrail->m_pointArray->objectAtIndex(objectCount - 1))->m_point;
            CCPoint point2 = point;
            CCPoint vector01 = (point1 - point0).normalize();
            CCPoint vector12 = (point2 - point1).normalize();
            float cpm = abs(vector12.cross(vector01));
            if (cpm <= 0.001) m_waveTrail->m_pointArray->removeObjectAtIndex(objectCount - 1);
        }
        m_waveTrail->addPoint(point);
    }

    bool pushButton(PlayerButton button) {
        if (!Astral::Cosmetics::WaveDragFix::isEnabled() || !m_isDart || !m_gameLayer || LevelEditorLayer::get()) 
            return PlayerObject::pushButton(button);
        
        const int TOGGLE_RING = 1594;
        const int TELEPORT_RING = 3027;
        const int SPIDER_RING = 3004;
        
        bool willTriggerSpiderRing = false;
        for (unsigned i = 0; i < m_touchingRings->count(); i++) {
            RingObject *ring = static_cast<RingObject *>(m_touchingRings->objectAtIndex(i));
            switch (ring->m_objectID) {
                case TOGGLE_RING:
                    if (ring->m_claimTouch) return PlayerObject::pushButton(button);
                    [[fallthrough]];
                case TELEPORT_RING:
                    continue;
                case SPIDER_RING:
                    willTriggerSpiderRing = true;
                    [[fallthrough]];
                default:
                    if (!willTriggerSpiderRing) return PlayerObject::pushButton(button);
            }
        }
        
        if (willTriggerSpiderRing) {
            m_waveTrail->addPoint(m_fields->currentPos);
            m_fields->previousPos = m_fields->currentPos;
            m_fields->forceAddSpiderRing = true;
        }
        return PlayerObject::pushButton(button);
    }

    void spiderTestJump(bool p0) {
        if (!Astral::Cosmetics::WaveDragFix::isEnabled() || !m_isDart || !m_gameLayer || LevelEditorLayer::get() || m_fields->justTeleported) 
            return PlayerObject::spiderTestJump(p0);
        
        m_waveTrail->addPoint(m_fields->currentPos);
        m_fields->previousPos = m_fields->currentPos;
        if (!m_fields->forceAddSpiderRing) m_fields->spiderPadTriggered = true;
        PlayerObject::spiderTestJump(p0);
    }

    void doReversePlayer(bool state) {
        m_fields->forceAdd = true;
        PlayerObject::doReversePlayer(state);
    }

    void placeStreakPoint() { 
        if (!m_isDart || !m_gameLayer) PlayerObject::placeStreakPoint(); 
    }

    void toggleVisibility(bool state) {
        bool needsPoint = m_isHidden;
        PlayerObject::toggleVisibility(state);
        if (state && m_isDart && needsPoint) m_fields->forceAdd = true;
    }
};

class $modify(PlayLayer) {
    /*void playEndAnimationToPos(CCPoint pos) {
        if (!Astral::Cosmetics::WaveDragFix::isEnabled()) {
            return PlayLayer::playEndAnimationToPos(pos);
        }
        
        if (m_player1 && m_player1->m_isDart)
            m_player1->m_waveTrail->addPoint(m_player1->getRealPosition());
        if (m_player2 && m_player2->m_isDart)
            m_player2->m_waveTrail->addPoint(m_player2->getRealPosition());
        PlayLayer::playEndAnimationToPos(pos);
    }*/
   void playEndAnimationToPos(CCPoint pos) {
    if (Astral::Cosmetics::WaveDragFix::isEnabled()) {
        if (m_player1 && m_player1->m_isDart)
            m_player1->m_waveTrail->addPoint(m_player1->getRealPosition());
        if (m_player2 && m_player2->m_isDart)
            m_player2->m_waveTrail->addPoint(m_player2->getRealPosition());
    }
    PlayLayer::playEndAnimationToPos(pos);
}
};

class $modify(GJBaseGameLayer) {
    void teleportPlayer(TeleportPortalObject *portal, PlayerObject *player) {
        GJBaseGameLayer::teleportPlayer(portal, player);
        if (!Astral::Cosmetics::WaveDragFix::isEnabled() || LevelEditorLayer::get()) return;
        
        if (!player) player = m_player1;
        if (!player->m_isDart) return;
        
        static_cast<WTDFPlayerObject *>(player)->m_fields->previousPos = player->getRealPosition();
        static_cast<WTDFPlayerObject *>(player)->m_fields->justTeleported = true;
    }

    void toggleDualMode(GameObject* portal, bool state, PlayerObject* playerTouchingPortal, bool p4) {
        if (!Astral::Cosmetics::WaveDragFix::isEnabled()) {
            return GJBaseGameLayer::toggleDualMode(portal, state, playerTouchingPortal, p4);
        }
        
        if (!state && playerTouchingPortal == m_player2) {
            WTDFPlayerObject::Fields fieldsBackup = *static_cast<WTDFPlayerObject *>(m_player2)->m_fields.operator->();
            GJBaseGameLayer::toggleDualMode(portal, state, playerTouchingPortal, p4);
            *static_cast<WTDFPlayerObject *>(m_player1)->m_fields.operator->() = fieldsBackup;
        } else {
            GJBaseGameLayer::toggleDualMode(portal, state, playerTouchingPortal, p4);
        }
    }
};