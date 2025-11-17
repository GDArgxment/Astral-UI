#include "includes.hpp"
using namespace geode::prelude;


namespace Astral::Cosmetics::NoRespawnFlash {
    static bool noRespawnFlashEnabled = false;
    
    bool isEnabled() {
        return noRespawnFlashEnabled;
    }
    
    void setEnabled(bool enabled) {
        noRespawnFlashEnabled = enabled;
    }
}

class $modify(NoRespawnFlashPOHook, PlayerObject) {
    void playSpawnEffect() {
        if (Astral::Cosmetics::NoRespawnFlash::isEnabled()) {
            return;
        }
        else {
            PlayerObject::playSpawnEffect();
        }
    }
};