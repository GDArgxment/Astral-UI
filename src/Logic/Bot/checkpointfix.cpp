#include "includes.hpp"
using namespace geode::prelude;

namespace Astral::Bot::CheckpointFix {
    class $modify(CheckpointDelayHook, PlayLayer) {
        void resetLevel() {
            m_tryPlaceCheckpoint = false;
            PlayLayer::resetLevel();
        }
        
        void markCheckpoint() {
            m_tryPlaceCheckpoint = false;
            PlayLayer::markCheckpoint();
        }
        
        void update(float dt) {
            // If framestepper is active and checkpoint was requested, place it immediately
            if (Astral::Hacks::FrameStepper::isEnabled() && m_tryPlaceCheckpoint) {
                m_tryPlaceCheckpoint = false;
                this->markCheckpoint();
            }
            
            PlayLayer::update(dt);
        }
    };
    
    class $modify(CheckpointDelayUIHook, UILayer) {
        void onCheck(cocos2d::CCObject* sender) {
            UILayer::onCheck(sender);
            
            // If framestepper is active, place checkpoint immediately instead of waiting
            if (Astral::Hacks::FrameStepper::isEnabled()) {
                auto pl = PlayLayer::get();
                if (pl) {
                    pl->m_tryPlaceCheckpoint = false;
                    pl->markCheckpoint();
                }
            }
        }
    };
}