// FrameStepper.cpp
#include "includes.hpp"
using namespace geode::prelude;

extern float frameCount;

namespace Astral::Hacks::FrameStepper {
    static bool framestepEnabled = false;
    static bool justStepped = false;
    static int framesToStep = 0;
    static bool keyWasPressed = false;
    static float holdTimer = 0.0f;
    static const float HOLD_DELAY = 0.3f;
    static const float HOLD_RATE = 0.05f;
    static float holdStepTimer = 0.0f;
    
    // Frame history structures
    struct FrameSnapshot {
        cocos2d::CCPoint p1Pos;
        cocos2d::CCPoint p2Pos;
        float p1Rotation;
        float p2Rotation;
        int frameNumber;
    };
    
    static std::deque<FrameSnapshot> frameHistory;
    static const size_t MAX_HISTORY = 3600; // Store up to 3600 frames (~15 seconds at 240 TPS)
    static int currentFrameIndex = 0;
    static bool isRestoring = false;
    
    bool isEnabled() {
        return framestepEnabled;
    }
    
    void setEnabled(bool enabled) {
        framestepEnabled = enabled;
    }
    
    void setTPS(float tps) {
        tpsValue = tps;
    }
    
    float getTPS() {
        return tpsValue;
    }
    
    bool getJustStepped() {
        return justStepped;
    }
    
    void captureFrameState(PlayLayer* playLayer) {
        if (!playLayer || !playLayer->m_player1) return;
        
        FrameSnapshot snapshot;
        
        // Capture Player 1
        auto p1 = playLayer->m_player1;
        snapshot.p1Pos = p1->getPosition();
        snapshot.p1Rotation = p1->getRotation();
        
        // Capture Player 2 if it exists
        if (playLayer->m_player2) {
            auto p2 = playLayer->m_player2;
            snapshot.p2Pos = p2->getPosition();
            snapshot.p2Rotation = p2->getRotation();
        }
        
        snapshot.frameNumber = currentFrameIndex++;
        
        frameHistory.push_back(snapshot);
        
        // Keep history size under limit
        if (frameHistory.size() > MAX_HISTORY) {
            frameHistory.pop_front();
        }
    }
    
    void restoreFrameState(PlayLayer* playLayer, const FrameSnapshot& snapshot) {
        if (!playLayer || !playLayer->m_player1) return;
        
        // Restore Player 1
        auto p1 = playLayer->m_player1;
        p1->setPosition(snapshot.p1Pos);
        p1->setRotation(snapshot.p1Rotation);
        
        // Restore Player 2 if it exists
        if (playLayer->m_player2) {
            auto p2 = playLayer->m_player2;
            p2->setPosition(snapshot.p2Pos);
            p2->setRotation(snapshot.p2Rotation);
        }
    }
    
    void stepFrame() {
        if (isEnabled()) {
            framesToStep = 1;
        }
    }
    
    void stepBack() {
        if (frameHistory.size() <= 1) {
            AstralLogger::log("No frames to step back to");
            return;
        }
        
        if (auto playLayer = PlayLayer::get()) {
            // Remove current frame and restore previous
            frameHistory.pop_back();
            
            if (!frameHistory.empty()) {
                restoreFrameState(playLayer, frameHistory.back());
                isRestoring = true;
                AstralLogger::log("Stepped back to frame {}", frameHistory.back().frameNumber);
            }
        }
    }
    
    void clearHistory() {
        frameHistory.clear();
        currentFrameIndex = 0;
    }
}

class $modify(FrameStepperHook, GJBaseGameLayer) {
    void update(float dt) override {
        using namespace Astral::Hacks::FrameStepper;
        justStepped = false;
        
        if (!isEnabled()) {
            return GJBaseGameLayer::update(dt);
        }

        bool canUse = false;
        if (auto playLayer = PlayLayer::get()) {
            canUse = !playLayer->m_hasCompletedLevel && 
                    playLayer->m_started && !playLayer->m_player1->m_isDead;
        }
        
        if (!canUse) {
            return GJBaseGameLayer::update(dt);
        }

        // Capture state before update
        if (auto playLayer = PlayLayer::get()) {
            captureFrameState(playLayer);
        }

        if (framesToStep > 0) {
            float tps = Astral::Hacks::TPSBypass::getTPS();
            float tickTime = 1.f / tps;
            GJBaseGameLayer::update(tickTime);
            framesToStep--;
            justStepped = true;
            isRestoring = false;
        } else if (isRestoring) {
            // Don't update when we just restored, wait for next frame step
            isRestoring = false;
        }
    }
};
/*
class $modify(FrameStepperKeyHook, cocos2d::CCKeyboardDispatcher) {
    bool dispatchKeyboardMSG(cocos2d::enumKeyCodes key, bool isKeyDown, bool isKeyRepeat) {
        using namespace Astral::Hacks::FrameStepper;
        
        if (key == cocos2d::enumKeyCodes::KEY_G && isKeyDown && !isKeyRepeat) {
            setEnabled(!isEnabled());
            framesToStep = 0;
            keyWasPressed = false;
            holdTimer = 0.0f;
            holdStepTimer = 0.0f;
            if (isEnabled()) {
                clearHistory();
            }
            AstralLogger::log("Frame stepper: {}", isEnabled() ? "enabled" : "disabled");
            return true;
        }
        
        if (key == cocos2d::enumKeyCodes::KEY_F) {
            if (isKeyDown) {
                if (!isEnabled()) {
                    setEnabled(true);
                    AstralLogger::log("Frame stepper: enabled (via F key)");
                }
                
                if (!keyWasPressed) {
                    framesToStep = 1;
                    keyWasPressed = true;
                    holdTimer = 0.0f;
                    holdStepTimer = 0.0f;
                } else {
                    holdTimer += 0.016f;
                    
                    if (holdTimer >= HOLD_DELAY) {
                        holdStepTimer += 0.016f;
                        if (holdStepTimer >= HOLD_RATE) {
                            framesToStep = 1;
                            holdStepTimer = 0.0f;
                        }
                    }
                }
            } else {
                keyWasPressed = false;
                holdTimer = 0.0f;
                holdStepTimer = 0.0f;
            }
            return true;
        }
        
        return cocos2d::CCKeyboardDispatcher::dispatchKeyboardMSG(key, isKeyDown, isKeyRepeat);
    }
};
*/