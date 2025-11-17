// my favorite and the most important hack
#include "includes.hpp"
#include <Geode/binding/HardStreak.hpp>

using namespace geode::prelude;

namespace Astral::Cosmetics::KeepWave {
    static bool keepWaveEnabled = false;
    static bool noFadeOut = false;
    static bool noReset = false;
    
    bool isEnabled() {
        return keepWaveEnabled;
    }
    
    void setEnabled(bool enabled) {
        keepWaveEnabled = enabled;
    }
    
    bool noFadeOutEnabled() {
        return noFadeOut;
    }
    
    void setNoFadeOut(bool enabled) {
        noFadeOut = enabled;
    }
    
    bool noResetEnabled() {
        return noReset;
    }
    
    void setNoReset(bool enabled) {
        noReset = enabled;
    }
}

class $modify(KeepWaveFadeOutHook, PlayerObject) {
    void fadeOutStreak2(float p0) {
        if (Astral::Cosmetics::KeepWave::isEnabled() && Astral::Cosmetics::KeepWave::noFadeOutEnabled())
            return;
        PlayerObject::fadeOutStreak2(p0);
    }
};

class $modify(KeepWaveResetHook, PlayerObject) {
    void resetStreak() {
        if (Astral::Cosmetics::KeepWave::isEnabled() && Astral::Cosmetics::KeepWave::noResetEnabled() && m_isDart)
            return;
        PlayerObject::resetStreak();
    }
};
/*
class $modify(PlayerObject) {
    struct Fields {
        HardStreak* m_waveTrail;
    };
    void stopStroke();
    void resumeStroke();
};
class $modify(WaveTrailTeleportFix, GJBaseGameLayer) {
    void teleportPlayer(TeleportPortalObject* portal, PlayerObject* player) {
        if (player->m_isDart && player->m_waveTrail) {
            bool wasNoResetEnabled = Astral::Cosmetics::KeepWave::noResetEnabled();
            Astral::Cosmetics::KeepWave::setNoReset(false);
            
            GJBaseGameLayer::teleportPlayer(portal, player);
            
            Astral::Cosmetics::KeepWave::setNoReset(wasNoResetEnabled);
            
            if (player->m_waveTrail) {
                player->m_waveTrail->stopStroke();
                player->m_waveTrail->m_currentPoint = player->getPosition();
                player->m_waveTrail->resumeStroke();
            }
        } else {
            GJBaseGameLayer::teleportPlayer(portal, player);
        }
    }
};
*/