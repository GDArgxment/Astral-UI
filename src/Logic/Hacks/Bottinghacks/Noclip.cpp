#include "includes.hpp"
using namespace geode::prelude;

namespace Astral::Hacks::Noclip {
    static bool noclipEnabled = false;
    static bool noclipP1 = false;
    static bool noclipP2 = false;
    bool isNoclipEnabled() {
        return noclipEnabled;
    }
    void setNoclipEnabled(bool enabled) {
        noclipEnabled = enabled;
    }
    bool isNoclipP1() {
        return noclipP1;
    }
    void setNoclipP1(bool enabled) {
        noclipP1 = enabled;
    }
    bool isNoclipP2() {
        return noclipP2;
    }
    void setNoclipP2(bool enabled) {
        noclipP2 = enabled;
    }
}
class $modify(NoclipHook, PlayLayer) {
    void destroyPlayer(PlayerObject* player, GameObject* object) {
        if (Astral::Hacks::Noclip::isNoclipEnabled()) {
            bool shouldNoclip = false;
            if (player == m_player1) {
                shouldNoclip = Astral::Hacks::Noclip::isNoclipP1();
            }
            else if (player == m_player2) {
                shouldNoclip = Astral::Hacks::Noclip::isNoclipP2();
            }
            if (shouldNoclip && object != m_anticheatSpike) {
              return;
            }
        }
        PlayLayer::destroyPlayer(player, object);
    }
};
