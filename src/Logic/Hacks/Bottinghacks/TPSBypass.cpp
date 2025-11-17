#include "includes.hpp"
#include <Geode/modify/CCDirector.hpp>
using namespace geode::prelude;

namespace Astral::Hacks::TPSBypass {

static float g_currentTPS = 240.f;
static bool g_tpsEnabled = true;
static float g_savedTPS = 240.f;

void saveTPS() {
    g_savedTPS = g_currentTPS;
}

void restoreTPS() {
    setTPS(g_savedTPS);
}

void setTPS(float tpsValue) {
    g_currentTPS = tpsValue;
    g_tpsEnabled = true;
    
    auto director = cocos2d::CCDirector::sharedDirector();
    if (director) {
        director->setAnimationInterval(1.0 / tpsValue);
    }
}

float getTPS() {
    return g_currentTPS;
}

bool isTpsEnabled() {
    return true;
}
static int g_updatesThisFrame = 0;

int getUpdatesThisFrame() {
    return g_updatesThisFrame;
}
class $modify(GJBaseGameLayer) {
    
    void update(float dt) {
        auto* playLayer = PlayLayer::get();
        if (!playLayer) return GJBaseGameLayer::update(dt);
        
        float tps = getTPS();
        if (tps == 240.f) return GJBaseGameLayer::update(dt);
        
        float newDt = 1.f / tps;
        int mult = static_cast<int>(dt / newDt);
        
        g_updatesThisFrame = 0;  // Reset before the loop!
        auto startTime = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < mult; ++i) {
            GJBaseGameLayer::update(newDt);
            g_updatesThisFrame++;
            // Check time every iteration but only cast if needed
            if ((i & 15) == 0) {  // Check every 16 iterations using bitwise AND
                auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
                if (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() > 16.667f) {
                    break;
                }
            }
        }
    }
    
    float getModifiedDelta(float dt) {
        auto* playLayer = PlayLayer::get();
        if (!playLayer) return GJBaseGameLayer::getModifiedDelta(dt);
        
        float tps = getTPS();
        if (tps == 240.f) return GJBaseGameLayer::getModifiedDelta(dt);
        
        float newDt = 1.f / tps;
        
        if (m_resumeTimer > 0) {
            m_resumeTimer--;
            dt = 0.0;
        }
        
        float timeWarp = 1.0;
        if (m_gameState.m_timeWarp <= 1.0) {
            timeWarp = m_gameState.m_timeWarp;
        }
        
        float dVar1 = dt + m_extraDelta;
        float fVar3 = std::round(dVar1 / (timeWarp * newDt));
        float dVar4 = fVar3 * timeWarp * newDt;
        m_extraDelta = dVar1 - dVar4;
        
        return dVar4;
    }
};

class $modify(CCDirector) {
    void setAnimationInterval(double interval) {
        float tps = getTPS();
        double newInterval = 1.0 / tps;
        return CCDirector::setAnimationInterval(newInterval);
    }
    
    void startAnimation() {
        float tps = getTPS();
        this->setAnimationInterval(1.0 / tps);
        return CCDirector::startAnimation();
    }
    
    float getDeltaTime() {
        return 1.f / getTPS();
    }
};

class $modify(AstralFrameCounterPL, PlayLayer) {
    void onQuit() {
        Astral::Hacks::TPSBypass::saveTPS();
        resetFrameCounter();
        PlayLayer::onQuit();
    }
};

class $modify(PlayLayerEnter, PlayLayer) {
    void onEnter() {
        PlayLayer::onEnter();
        Astral::Hacks::TPSBypass::restoreTPS();
    }
};

} // namespace Astral::Hacks::TPSBypass