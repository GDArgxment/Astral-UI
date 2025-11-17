#include "includes.hpp"

using namespace geode::prelude;

namespace Astral::Cosmetics::NoDeathEffect {
    static bool noDeathEffect = false;
    
    bool isEnabled() {
        return noDeathEffect;
    }
    
    void setEnabled(bool enabled) {
        noDeathEffect = enabled;
    }
}
class $modify(NoDeathEffectHook, PlayerObject) {
    void playDeathEffect() {
        if (Astral::Cosmetics::NoDeathEffect::isEnabled()) {
            return;
        }
        else {
            PlayerObject::playDeathEffect();
        }
    }
};