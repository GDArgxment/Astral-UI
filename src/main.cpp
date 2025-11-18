#include "includes.hpp"
// Forward declaration so other files can use it
using namespace geode::prelude;
extern HackData loadedHackData;


/*
de3am is a boykisser and a list helper
Breuhh is also a boykisser and a list helper. (de3am told me to put this)
*/

// The unholy list of vars
// bools


#ifdef GEODE_IS_MACOS
cocos2d::enumKeyCodes capturedCustomKey = cocos2d::enumKeyCodes::KEY_Tab;
#else
cocos2d::enumKeyCodes capturedCustomKey = cocos2d::enumKeyCodes::KEY_Alt;
#endif
// Ai is good for one thing, making a fucking massive list :skull:
const char* getKeyName(cocos2d::enumKeyCodes keyCode) {
    switch(keyCode) {
        #ifdef GEODE_IS_WINDOWS
        case cocos2d::enumKeyCodes::KEY_Alt: return "Alt";
        case cocos2d::enumKeyCodes::KEY_F1: return "F1";
        case cocos2d::enumKeyCodes::KEY_F2: return "F2";
        case cocos2d::enumKeyCodes::KEY_F3: return "F3";
        case cocos2d::enumKeyCodes::KEY_F4: return "F4";
        case cocos2d::enumKeyCodes::KEY_F5: return "F5";
        case cocos2d::enumKeyCodes::KEY_F6: return "F6";
        case cocos2d::enumKeyCodes::KEY_F7: return "F7";
        case cocos2d::enumKeyCodes::KEY_F8: return "F8";
        case cocos2d::enumKeyCodes::KEY_F9: return "F9";
        case cocos2d::enumKeyCodes::KEY_F10: return "F10";
        case cocos2d::enumKeyCodes::KEY_F11: return "F11";
        case cocos2d::enumKeyCodes::KEY_F12: return "F12";
        case cocos2d::enumKeyCodes::KEY_Insert: return "Insert";
        case cocos2d::enumKeyCodes::KEY_Home: return "Home";
        case cocos2d::enumKeyCodes::KEY_End: return "End";
        case cocos2d::enumKeyCodes::KEY_PageUp: return "Page Up";
        case cocos2d::enumKeyCodes::KEY_PageDown: return "Page Down";
        case cocos2d::enumKeyCodes::KEY_Delete: return "Delete";
        case cocos2d::enumKeyCodes::KEY_Space: return "Space";
        case cocos2d::enumKeyCodes::KEY_Enter: return "Enter";
        case cocos2d::enumKeyCodes::KEY_Escape: return "Escape";
        case cocos2d::enumKeyCodes::KEY_Tab: return "Tab";
        case cocos2d::enumKeyCodes::KEY_Shift: return "Shift";
        case cocos2d::enumKeyCodes::KEY_Control: return "Ctrl";
        case cocos2d::enumKeyCodes::KEY_ArrowLeft: return "Left Arrow";
        case cocos2d::enumKeyCodes::KEY_ArrowRight: return "Right Arrow";
        case cocos2d::enumKeyCodes::KEY_ArrowUp: return "Up Arrow";
        case cocos2d::enumKeyCodes::KEY_ArrowDown: return "Down Arrow";
        
        // Missing cases to add:
        case cocos2d::enumKeyCodes::KEY_None: return "None";
        case cocos2d::enumKeyCodes::KEY_Backspace: return "Backspace";
        case cocos2d::enumKeyCodes::KEY_Pause: return "Pause";
        case cocos2d::enumKeyCodes::KEY_CapsLock: return "Caps Lock";
        case cocos2d::enumKeyCodes::KEY_Select: return "Select";
        
        // Letter keys (A-Z)
        case cocos2d::enumKeyCodes::KEY_A: return "A";
        case cocos2d::enumKeyCodes::KEY_B: return "B";
        case cocos2d::enumKeyCodes::KEY_C: return "C";
        case cocos2d::enumKeyCodes::KEY_D: return "D";
        case cocos2d::enumKeyCodes::KEY_E: return "E";
        case cocos2d::enumKeyCodes::KEY_F: return "F";
        case cocos2d::enumKeyCodes::KEY_G: return "G";
        case cocos2d::enumKeyCodes::KEY_H: return "H";
        case cocos2d::enumKeyCodes::KEY_I: return "I";
        case cocos2d::enumKeyCodes::KEY_J: return "J";
        case cocos2d::enumKeyCodes::KEY_K: return "K";
        case cocos2d::enumKeyCodes::KEY_L: return "L";
        case cocos2d::enumKeyCodes::KEY_M: return "M";
        case cocos2d::enumKeyCodes::KEY_N: return "N";
        case cocos2d::enumKeyCodes::KEY_O: return "O";
        case cocos2d::enumKeyCodes::KEY_P: return "P";
        case cocos2d::enumKeyCodes::KEY_Q: return "Q";
        case cocos2d::enumKeyCodes::KEY_R: return "R";
        case cocos2d::enumKeyCodes::KEY_S: return "S";
        case cocos2d::enumKeyCodes::KEY_T: return "T";
        case cocos2d::enumKeyCodes::KEY_U: return "U";
        case cocos2d::enumKeyCodes::KEY_V: return "V";
        case cocos2d::enumKeyCodes::KEY_W: return "W";
        case cocos2d::enumKeyCodes::KEY_X: return "X";
        case cocos2d::enumKeyCodes::KEY_Y: return "Y";
        case cocos2d::enumKeyCodes::KEY_Z: return "Z";
        case cocos2d::enumKeyCodes::KEY_ScrollLock: return "Scroll Lock";
        #endif
        #ifdef GEODE_IS_MACOS
        case cocos2d::enumKeyCodes::KEY_Alt: return "Option";
        case cocos2d::enumKeyCodes::KEY_F1: return "F1";
        case cocos2d::enumKeyCodes::KEY_F2: return "F2";
        case cocos2d::enumKeyCodes::KEY_F3: return "F3";
        case cocos2d::enumKeyCodes::KEY_F4: return "F4";
        case cocos2d::enumKeyCodes::KEY_F5: return "F5";
        case cocos2d::enumKeyCodes::KEY_F6: return "F6";
        case cocos2d::enumKeyCodes::KEY_F7: return "F7";
        case cocos2d::enumKeyCodes::KEY_F8: return "F8";
        case cocos2d::enumKeyCodes::KEY_F9: return "F9";
        case cocos2d::enumKeyCodes::KEY_F10: return "F10";
        case cocos2d::enumKeyCodes::KEY_F11: return "F11";
        case cocos2d::enumKeyCodes::KEY_F12: return "F12";
        case cocos2d::enumKeyCodes::KEY_Insert: return "Insert";
        case cocos2d::enumKeyCodes::KEY_Home: return "Home";
        case cocos2d::enumKeyCodes::KEY_End: return "End";
        case cocos2d::enumKeyCodes::KEY_PageUp: return "Page Up";
        case cocos2d::enumKeyCodes::KEY_PageDown: return "Page Down";
        case cocos2d::enumKeyCodes::KEY_Delete: return "Delete";
        case cocos2d::enumKeyCodes::KEY_Space: return "Space";
        case cocos2d::enumKeyCodes::KEY_Enter: return "Return";
        case cocos2d::enumKeyCodes::KEY_Escape: return "Escape";
        case cocos2d::enumKeyCodes::KEY_Tab: return "Tab";
        case cocos2d::enumKeyCodes::KEY_Shift: return "Shift";
        case cocos2d::enumKeyCodes::KEY_Control: return "Control";
        case cocos2d::enumKeyCodes::KEY_ArrowLeft: return "←";
        case cocos2d::enumKeyCodes::KEY_ArrowRight: return "→";
        case cocos2d::enumKeyCodes::KEY_ArrowUp: return "↑";
        case cocos2d::enumKeyCodes::KEY_ArrowDown: return "↓";
        
        case cocos2d::enumKeyCodes::KEY_None: return "None";
        case cocos2d::enumKeyCodes::KEY_Backspace: return "Delete";
        case cocos2d::enumKeyCodes::KEY_Pause: return "Pause";
        case cocos2d::enumKeyCodes::KEY_CapsLock: return "Caps Lock";
        case cocos2d::enumKeyCodes::KEY_Select: return "Select";
        
        // Letter keys (A-Z)
        case cocos2d::enumKeyCodes::KEY_A: return "A";
        case cocos2d::enumKeyCodes::KEY_B: return "B";
        case cocos2d::enumKeyCodes::KEY_C: return "C";
        case cocos2d::enumKeyCodes::KEY_D: return "D";
        case cocos2d::enumKeyCodes::KEY_E: return "E";
        case cocos2d::enumKeyCodes::KEY_F: return "F";
        case cocos2d::enumKeyCodes::KEY_G: return "G";
        case cocos2d::enumKeyCodes::KEY_H: return "H";
        case cocos2d::enumKeyCodes::KEY_I: return "I";
        case cocos2d::enumKeyCodes::KEY_J: return "J";
        case cocos2d::enumKeyCodes::KEY_K: return "K";
        case cocos2d::enumKeyCodes::KEY_L: return "L";
        case cocos2d::enumKeyCodes::KEY_M: return "M";
        case cocos2d::enumKeyCodes::KEY_N: return "N";
        case cocos2d::enumKeyCodes::KEY_O: return "O";
        case cocos2d::enumKeyCodes::KEY_P: return "P";
        case cocos2d::enumKeyCodes::KEY_Q: return "Q";
        case cocos2d::enumKeyCodes::KEY_R: return "R";
        case cocos2d::enumKeyCodes::KEY_S: return "S";
        case cocos2d::enumKeyCodes::KEY_T: return "T";
        case cocos2d::enumKeyCodes::KEY_U: return "U";
        case cocos2d::enumKeyCodes::KEY_W: return "W";
        case cocos2d::enumKeyCodes::KEY_X: return "X";
        case cocos2d::enumKeyCodes::KEY_Y: return "Y";
        case cocos2d::enumKeyCodes::KEY_Z: return "Z";
        case cocos2d::enumKeyCodes::KEY_ScrollLock: return "Scroll Lock";
        #endif
        default: return "Unknown Key";
    }
}
    void SetupKeybinds();
    $on_mod(Loaded)
    {
        std::filesystem::create_directories(AstralPath());
        std::filesystem::create_directories(ffmpegDir());
        std::filesystem::create_directories(RendersDir());
        std::filesystem::create_directories(MacrosDir());
        std::filesystem::create_directories(LogsDir());
        std::filesystem::create_directories(GuisDir());
        std::filesystem::create_directories(AutoclickerDir());
        AstralLogger::init();
        Astral::Renderer::InstallFFmpeg();
        SetupKeybinds();
        auto director = cocos2d::CCDirector::sharedDirector();
        if (director) {
            float tps = Astral::Hacks::TPSBypass::getTPS();
            director->setAnimationInterval(1.0 / tps);
        }
        
        AstralLogger::log("Astral Bot loaded");
        AstralLogger::log("Made by AstralTeam and more");
        #ifndef GEODE_IS_MOBILE
        ImGuiCocos::get().setup([]
            {
                setupImGuiStyle();
            })
            .draw([]
                {
                    renderMainGui();
                });
                #endif
                
            }
            
            
            class $modify(ImGuiKeybindHook, cocos2d::CCKeyboardDispatcher){
                bool dispatchKeyboardMSG(cocos2d::enumKeyCodes key, bool isKeyDown, bool isKeyRepeat)
                {
                    if (isCapturingKeybind && isKeyDown && !isKeyRepeat) {
                        if (key == cocos2d::enumKeyCodes::KEY_Escape) {
                            isCapturingKeybind = false;
                        } else {
                            capturedCustomKey = key;
                            saveHackData();  
                            isCapturingKeybind = false;
                        }
                        return true; 
                    }
                    
                    if (capturedCustomKey != cocos2d::enumKeyCodes::KEY_None && key == capturedCustomKey && isKeyDown) {
                        ImGuiCocos::get().toggle();
                        guiVisible = ImGuiCocos::get().isVisible();
                    }
                    
                    return cocos2d::CCKeyboardDispatcher::dispatchKeyboardMSG(key, isKeyDown, isKeyRepeat);
                }
            };
            