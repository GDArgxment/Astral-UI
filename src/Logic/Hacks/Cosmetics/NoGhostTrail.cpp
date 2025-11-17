#include "includes.hpp"
using namespace geode::prelude;

class $modify(PlayerObjectNGTHook, PlayerObject) {
    struct Fields {
        GhostType m_ghostType = GhostType::Disabled;
    };

    void toggleGhostEffect(GhostType p0) {
        if (!Astral::Cosmetics::NoGhostTrail::isEnabled()) {
            m_fields->m_ghostType = p0;
        }
        PlayerObject::toggleGhostEffect(Astral::Cosmetics::NoGhostTrail::isEnabled() ? GhostType::Disabled : p0);
    }
};

namespace Astral::Cosmetics::NoGhostTrail {
    static bool ghostTrail = false;
    
    bool isEnabled() {
        return ghostTrail;
    }

    void setEnabled(bool enabled) {
        ghostTrail = enabled;
        updateGhostTrail();
    }
    
    void updateGhostTrail() {
        auto playLayer = PlayLayer::get();
        if (!playLayer) {
            return;
        }

        if (playLayer->m_player1) {
            auto p1 = static_cast<PlayerObjectNGTHook*>(playLayer->m_player1);
            if (isEnabled()) {
                p1->toggleGhostEffect(GhostType::Disabled);
            } else {
                p1->toggleGhostEffect(p1->m_fields->m_ghostType);
            }
        }

        if (playLayer->m_player2) {
            auto p2 = static_cast<PlayerObjectNGTHook*>(playLayer->m_player2);
            if (isEnabled()) {
                p2->toggleGhostEffect(GhostType::Disabled);
            } else {
                p2->toggleGhostEffect(p2->m_fields->m_ghostType);
            }
        }
    }
}