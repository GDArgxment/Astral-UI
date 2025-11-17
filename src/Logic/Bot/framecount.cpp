#include "includes.hpp"
using namespace geode::prelude;
float frameCount = 0.0f;
bool initialized = false;
float lastProgress = 0.0f;
extern float frameCount;

float getCurrentFrame() {
    return frameCount;
}

void resetFrameCounter() {
    frameCount = 0.0f;
}

void setFrameCount(float value) {
    frameCount = value;
}

int getSkipCounter() {
    return 0;  // No skip needed anymore
}

class $modify(AstralFrameCounterGJBGL, GJBaseGameLayer) {
    void update(float dt) {
        auto* playLayer = PlayLayer::get();
        bool isPlaying = playLayer && 
            !playLayer->m_hasCompletedLevel &&
            !playLayer->m_isPaused &&
            playLayer->m_gameState.m_currentProgress > 0.0f;
        
        if (isPlaying) {
            if (Astral::Hacks::FrameStepper::isEnabled()) {
                // When framestepping, only count if a step just happened
                if (Astral::Hacks::FrameStepper::getJustStepped()) {
                    frameCount += 1.0f;
                }
            } else {
                // Count every update call (1 per TPS frame)
                frameCount += 1.0f;
            }
        }
        
        GJBaseGameLayer::update(dt);
    }
};

class $modify(AstralFrameCounterPL, PlayLayer) {
    void resetLevel() {
        resetFrameCounter();
        PlayLayer::resetLevel();
    }

    void onQuit() {
        resetFrameCounter();
        PlayLayer::onQuit();
    }
};