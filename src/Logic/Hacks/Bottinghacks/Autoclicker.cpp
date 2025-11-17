#include "includes.hpp"
// to be fixed, hold and release got swapped lmfao
namespace Astral::Hacks::AutoClicker {
    static bool autoClickerEnabled = false;
    static KeySettings keyW, keyA, keyD, keyUP, keyLEFT, keyRIGHT, keySPACE;

    bool isEnabled() {
        return autoClickerEnabled;
    }

    void setEnabled(bool enabled) {
        autoClickerEnabled = enabled;
    }

    KeySettings& getKeySettings(char key) {
    switch(key) {
        case 'W': return keyW;
        case 'A': return keyA;
        case 'D': return keyD;
        case 'U': return keyUP;
        case 'L': return keyLEFT;
        case 'R': return keyRIGHT;
        case 'S': return keySPACE;
        default: return keyW;
    }
}
}

#ifdef GEODE_IS_DESKTOP
class $modify(AutoClickerGJBGLHook, GJBaseGameLayer) {
    struct Fields {
        struct KeyState {
            int timer = 0;
            bool clicking = false;
            int framesClicked = 0;
            int orbFrame = 0;
            bool initialized = false;
        };
        KeyState keyW, keyA, keyD, keyUP, keyLEFT, keyRIGHT, keySPACE;
    };
    
    void processKey(Fields::KeyState& state,
        const Astral::Hacks::AutoClicker::KeySettings& settings,
        PlayerButton button, bool isPlayer1)
    {
        using namespace Astral::Hacks::AutoClicker;

        if (!settings.enabled) {
            if (state.clicking) {
                this->handleButton(false, static_cast<int>(button), isPlayer1);
                state.clicking = false;
            }
            state.initialized = false;
            return;
        }
        
        if (!state.initialized) {
            state.timer = 0;
            state.clicking = false;
            state.framesClicked = 0;
            state.orbFrame = 0;
            state.initialized = true;
        }
        
        if (settings.limitFrames && state.framesClicked >= settings.maxFrames && settings.maxFrames > 0) {
            if (state.clicking) {
                this->handleButton(false, static_cast<int>(button), isPlayer1);
                state.clicking = false;
            }
            return;
        }
        
        if (settings.intervalHold == 0) {
            if (!state.clicking) {
                this->handleButton(true, static_cast<int>(button), isPlayer1);
                state.clicking = true;
                if (settings.limitFrames) state.framesClicked++;
            }
            return;
        }
        
        state.timer++;
        bool shouldToggle = false;
        
        if (!state.clicking && state.timer >= settings.intervalHold) {
            shouldToggle = true;
        } else if (state.clicking && state.timer >= settings.intervalRelease) {
            shouldToggle = true;
        }
        
        if (shouldToggle) {
            state.clicking = !state.clicking;
            
            if (state.clicking) {
                for (int i = 0; i < settings.clicksPerFrame; i++) {
                    this->handleButton(true, static_cast<int>(button), isPlayer1);
                    
                    if (settings.swiftClick) {
                        this->handleButton(false, static_cast<int>(button), isPlayer1);
                    }
                }
                if (settings.limitFrames) state.framesClicked++;
            } else {
                if (!settings.swiftClick) {
                    this->handleButton(false, static_cast<int>(button), isPlayer1);
                }
            }
            
            state.timer = 0;
        }
    }
    
    void update(float dt) {
        GJBaseGameLayer::update(dt);
        
        if (!Astral::Hacks::AutoClicker::isEnabled() || !PlayLayer::get()) return;
        
        using namespace Astral::Hacks::AutoClicker;

        processKey(m_fields->keyW, keyW, PlayerButton::Jump, true);
        processKey(m_fields->keyA, keyA, PlayerButton::Left, true);
        processKey(m_fields->keyD, keyD, PlayerButton::Right, true);
        processKey(m_fields->keyUP, keyUP, PlayerButton::Jump, false);
        processKey(m_fields->keyLEFT, keyLEFT, PlayerButton::Left, false);
        processKey(m_fields->keyRIGHT, keyRIGHT, PlayerButton::Right, false);
        processKey(m_fields->keySPACE, keySPACE, PlayerButton::Jump, true);
    }
};
#endif