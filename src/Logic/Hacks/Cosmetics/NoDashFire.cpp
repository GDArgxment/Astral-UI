#include "includes.hpp"

using namespace geode::prelude;

namespace Astral::Cosmetics::NoDashFire {
    static bool noDashFire = false;
    
    bool isEnabled() {
        return noDashFire;
    }
    
    void setEnabled(bool enabled) {
        noDashFire = enabled;
    }
}

class $modify(NoDashFireHook, PlayerObject) {
    void update(float dt) {
        PlayerObject::update(dt);
        if (!m_gameLayer) return;
        if (this != m_gameLayer->m_player1 && this != m_gameLayer->m_player2) return;
        if (CCSprite* dashFire = this->m_dashFireSprite; dashFire && Astral::Cosmetics::NoDashFire::isEnabled() && this->m_isDashing) {
            dashFire->setScale(0);
            if (CCSprite* child = dashFire->getChildByType<CCSprite>(0)) {
                child->setScale(0);
            }
        }
    }
};