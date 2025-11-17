#include "includes.hpp"
using namespace geode::prelude;
namespace Astral::Cosmetics::NoOrbEffects {
    static bool noOrbEffectEnabled = false;
    static bool noOrbRing = false;
    static bool noOrbHitEffect = false;
    static bool noDashBoom = false;

    bool isEnabled() {
        return noOrbEffectEnabled;
    }

    void setEnabled(bool enabled) {
        noOrbEffectEnabled = enabled;
    }

    void setNoOrbRing(bool enabled) {
        noOrbRing = enabled;
    }

    void setNoOrbHitEffect(bool enabled) {
        noOrbHitEffect = enabled;
    }

    void setNoDashBoom(bool enabled) {
        noDashBoom = enabled;
    }
}

/*
class $modify(NoOrbRingHook, RingObject) {
    void spawnCircle() {
        if (Astral::Cosmetics::NoOrbEffects::isEnabled()) {
            return;
        }
        RingObject::spawnCircle();
    }
};

class $modify(NoOrbHitEffectHook, PlayerObject) {
    void ringJump(RingObject* p0, bool p1) {
        if (Astral::Cosmetics::NoOrbEffects::isEnabled()) {
            bool orig = m_playEffects;
            m_playEffects = false;
            PlayerObject::ringJump(p0, p1);
            m_playEffects = orig;
        }
        else {
            PlayerObject::ringJump(p0, p1);
        }
    }
};

class $modify(NoDashBoomHook, PlayerObject) {
    void startDashing(DashRingObject* ring) {
        PlayerObject::startDashing(ring);
        auto pl = PlayLayer::get();
        if (pl) {
            auto dashBoomSprite = getChildBySpriteFrameName(m_gameLayer->m_objectLayer, "playerDash2_boom2_001.png");
            if (dashBoomSprite) {
                auto oldVisible = dashBoomSprite->isVisible();

                if (Astral::Cosmetics::NoOrbEffects::isEnabled() && Astral::Cosmetics::NoOrbEffects::noDashBoom) {
                    dashBoomSprite->setVisible(false);
                }
                else {
                    dashBoomSprite->setVisible(oldVisible);
                }
            }
        }
    }
};
*/