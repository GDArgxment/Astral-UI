#include "includes.hpp"
using namespace geode::prelude;

namespace Astral::Hacks::Speedhack {
    static bool speedhackEnabled = false;
    static bool speedhackAudio = false;
    static float speedhackMultiplier = 1.0f;
    
    bool isSpeedhackEnabled() {
        return speedhackEnabled;
    }
    
    void setEnabled(bool enabled) {
        speedhackEnabled = enabled;
    }
    
    bool isAudioEnabled() {
        return speedhackAudio;
    }
    
    void setAudioEnabled(bool enabled) {
        speedhackAudio = enabled;
    }
    
    float getMultiplier() {
        return speedhackMultiplier;
    }
    
    void setSpeedhackMultiplier(float multiplier) {
        speedhackMultiplier = multiplier;
    }
    
    bool isSpeedhackAudioEnabled() {
        return speedhackAudio;
    }
    
    void setSpeedhackAudioEnabled(bool enabled) {
        speedhackAudio = enabled;
    }
    void setSpeedhackEnabled(bool enabled) {
        speedhackEnabled = enabled;
    }
}
class $modify(SpeedhackSchedulerHook, cocos2d::CCScheduler) {
    void update(float dt) override {
        if (Astral::Hacks::Speedhack::isSpeedhackEnabled()) {
            CCScheduler::update(dt * Astral::Hacks::Speedhack::getMultiplier());
        } else {
            CCScheduler::update(dt);
        }
    }
};