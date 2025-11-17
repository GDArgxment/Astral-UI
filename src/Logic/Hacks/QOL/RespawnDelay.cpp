#include "includes.hpp"

using namespace geode::prelude;

namespace Astral::QOL::RespawnDelay {
    static float respawnDelay = 0.5f;
    
    float getDelay() {
        return respawnDelay;
    }
    
    void setDelay(float delay) {
        respawnDelay = delay;
    }
}

class $modify(RespawnDelayPLHook, PlayLayer) {
    void destroyPlayer(PlayerObject* player, GameObject* object) {
        PlayLayer::destroyPlayer(player, object);
        
        auto delay = Astral::QOL::RespawnDelay::getDelay();
        if (auto respawnSequence = this->getActionByTag(0x10)) {
            this->stopAction(respawnSequence);
            auto newSequence = CCSequence::create(
                CCDelayTime::create(delay),
                CCCallFunc::create(this, callfunc_selector(PlayLayer::delayedResetLevel)),
                nullptr
            );
            newSequence->setTag(0x10);
            this->runAction(newSequence);
        }
    }
};