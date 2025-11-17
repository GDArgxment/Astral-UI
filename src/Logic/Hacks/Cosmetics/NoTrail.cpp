#include "includes.hpp"
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCMotionStreak.hpp>

using namespace geode::prelude;

namespace Astral::Cosmetics::NoTrail {
    static bool noTrailEnabled = false;
    static bool alwaysTrailEnabled = false;
    
    bool isEnabled() {
        return noTrailEnabled;
    }
    
    void setEnabled(bool enabled) {
        noTrailEnabled = enabled;
    }
    
    bool isAlwaysEnabled() {
        return alwaysTrailEnabled;
    }
    
    void setAlwaysEnabled(bool enabled) {
        alwaysTrailEnabled = enabled;
    }
}

#if defined(GEODE_IS_ANDROID) || defined(GEODE_IS_ARM_MAC) || defined(GEODE_IS_IOS)
#define GEODE_IS_ARM 1
#endif

#ifdef GEODE_IS_ARM
class $modify(NoTrailPOHook, PlayerObject) {
    void deactivateStreak(bool p0) {
        if (!this->m_fadeOutStreak) return;
        this->m_fadeOutStreak = false;
        this->fadeOutStreak2(this->m_playEffects ? 0.2f : 0.6f);
    }

    void activateStreak() {
        if (Astral::Cosmetics::NoTrail::isEnabled()) return;
        if (this->levelFlipping()) return;
        if (GameManager::sharedState()->m_editorEnabled) return;
        if (this->m_isHidden) return;

        this->m_fadeOutStreak = true;
        if (this->m_isDart) {
            auto pos = this->getPosition();
            this->m_waveTrail->m_currentPoint = pos;
            this->m_waveTrail->stopAllActions();
            this->m_waveTrail->setOpacity(255);
            this->m_waveTrail->resumeStroke();
        }
    }
};
#else
class $modify(NoTrailCCMSHook, cocos2d::CCMotionStreak) {
    void resumeStroke() {
        if (!Astral::Cosmetics::NoTrail::isEnabled()) CCMotionStreak::resumeStroke();
    }

    void stopStroke() {
        if (!Astral::Cosmetics::NoTrail::isAlwaysEnabled()) CCMotionStreak::stopStroke();
    }
};
#endif

class $modify(NoWaveTrailBehindPOHook, PlayerObject) {
    void toggleDartMode(bool p0, bool p1) {
        PlayerObject::toggleDartMode(p0, p1);
        if (this->m_isDart && this->m_regularTrail) {
            this->m_regularTrail->stopStroke();
        }
    }

    void activateStreak() {
        if (!this->m_isDart) return PlayerObject::activateStreak();
        if (this->levelFlipping()) return;
        if (GameManager::sharedState()->m_editorEnabled) return;
        if (this->m_isHidden) return;

        this->m_fadeOutStreak = true;
        if (this->m_isDart) {
            auto pos = this->getPosition();
            this->m_waveTrail->m_currentPoint = pos;
            this->m_waveTrail->stopAllActions();
            this->m_waveTrail->setOpacity(255);
            this->m_waveTrail->resumeStroke();
        }
    }
};