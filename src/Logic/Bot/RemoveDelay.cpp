#include "includes.hpp"

using namespace geode::prelude;

namespace Astral::Hacks::PauseBuffering {
    class $modify(PauseBufferingUILHook, UILayer) {
        void onPause(cocos2d::CCObject* sender) {
            auto* playLayer = PlayLayer::get();
            if (!playLayer) return UILayer::onPause(sender);

            uint32_t original = playLayer->m_gameState.m_pauseCounter;
            playLayer->m_gameState.m_pauseCounter = 0;
            UILayer::onPause(sender);
            playLayer->m_gameState.m_pauseCounter = original;
        }
    };
    
    class $modify(PauseBufferingPLHook, PlayLayer) {
        void resume() {
            PlayLayer::resume();
            auto* playLayer = PlayLayer::get();
            uint32_t original = playLayer->m_gameState.m_pauseCounter;
            PlayLayer::resume();
            playLayer->m_gameState.m_pauseCounter = 0;
        }
    };
}