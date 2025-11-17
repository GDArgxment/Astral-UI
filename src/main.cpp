#include "includes.hpp"
// Forward declaration so other files can use it
void saveHackData();
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


void loadAllSavedData() {
    try {
        HackData data = Mod::get()->getSavedValue<HackData>("hack-data");
        
        // Botting & Macros
        selectedMacroName = data.selectedMacroName;
        availableMacros = data.availableMacros;
        isRecording = data.isRecording;
        
        // Movement Hacks
        waveTrailDragFix = data.waveTrailDragFix;
        trajectoryEnabled = data.trajectoryEnabled;
        noclipEnabled = data.noclipEnabled;
        noclipP1 = data.noclipP1;
        noclipP2 = data.noclipP2;
        speedhackEnabled = data.speedhackEnabled;
        speedhackMultiplier = data.speedhackMultiplier;
        
        // Level Hacks
        seedValue = data.seedValue;
        layoutEnabled = data.layoutEnabled;
        respawnDelay = data.respawnDelay;
        seedHackEnabled = data.seedHackEnabled;
        keepWaveEnabled = data.keepWaveEnabled;
        
        // Visual Hacks
        decimalPlaces = data.decimalPlaces;
        accuratePercentage = data.accuratePercentage;
        noMirror = data.noMirror;
        instantMirror = data.instantMirror;
        noDashFire = data.noDashFire;
        noShaders = data.noShaders;
        noDashBoom = data.noDashBoom;
        noOrbHitEffect = data.noOrbHitEffect;
        noOrbRing = data.noOrbRing;
        noOrbEffectEnabled = data.noOrbEffectEnabled;
        ghostTrail = data.ghostTrail;
        noRespawnFlash = data.noRespawnFlash;
        noDeathEffect = data.noDeathEffect;
        noFade = data.noFade;
        noReset = data.noReset;
        
        // Render & Debug
        showHitboxes = data.showHitboxes;
        showHitboxTrail = data.showHitboxTrail;
        showRotated = data.showRotated;
        showFill = data.showFill;
        trailLength = data.trailLength;
        showInner = data.showInner;
        showCircle = data.showCircle;
        
        tpsValue = data.tpsValue;
        tpsBypassEnabled = data.tpsBypassEnabled;
        safeMode = data.safeMode;
        debugLog = data.debugLog;
        hitboxThickness = data.hitboxThickness;
        noTrail = data.noTrail;
        labelsEnabled = data.labelsEnabled;
        labelScale = data.labelScale;
        labelOpacity = data.labelOpacity;
        player1Alignment = data.player1Alignment;
        player2Alignment = data.player2Alignment;
        showLabelPosition = data.showLabelPosition;
        showLabelVelocity = data.showLabelVelocity;
        showLabelStatus = data.showLabelStatus;
        showLabelAttempt = data.showLabelAttempt;
        showLabelProgress = data.showLabelProgress;
        showLabelFPS = data.showLabelFPS;
        #ifdef GEODE_IS_MACOS
        capturedCustomKey = cocos2d::enumKeyCodes::KEY_Tab;
        #else
        capturedCustomKey = cocos2d::enumKeyCodes::KEY_Alt;
        #endif
        capturedCustomKey = data.capturedCustomKey;
        // Only override if loaded value is valid
        if (capturedCustomKey != 0) {
            // Keep the loaded value
        }
        
        AstralLogger::log("Loaded hack data successfully");
    } catch (const std::exception& e) {
        AstralLogger::log("No saved hack data found, using defaults");
    }
    
    initialized = true;
    geode::queueInMainThread([]() {
        if (layoutEnabled) Astral::Hacks::LayoutMode::setEnabled(layoutEnabled);
        // if (trajectoryEnabled) Astral::Hacks::Trajectory::setEnabled(trajectoryEnabled);
        if (noclipEnabled) Astral::Hacks::Noclip::setNoclipEnabled(noclipEnabled);
        if (noclipP1) Astral::Hacks::Noclip::setNoclipP1(noclipP1);
        if (noclipP2) Astral::Hacks::Noclip::setNoclipP2(noclipP2);
        if (speedhackEnabled) Astral::Hacks::Speedhack::setSpeedhackEnabled(speedhackEnabled);
        if (speedhackMultiplier != 1.0f) Astral::Hacks::Speedhack::setSpeedhackMultiplier(speedhackMultiplier);
        if (seedHackEnabled) Astral::Hacks::Seedhack::setEnabled(seedHackEnabled);
        if (seedValue != 0) Astral::Hacks::Seedhack::setSeedValue(seedValue);
        if (keepWaveEnabled) Astral::Cosmetics::KeepWave::setEnabled(keepWaveEnabled);
        if (noFade) Astral::Cosmetics::KeepWave::setNoFadeOut(noFade);
        if (noReset) Astral::Cosmetics::KeepWave::setNoReset(noReset);
        if (noMirror) Astral::Cosmetics::NoMirror::setEnabled(noMirror);
        if (instantMirror) Astral::Cosmetics::InstantMirror::setEnabled(instantMirror);
        if (noDashFire) Astral::Cosmetics::NoDashFire::setEnabled(noDashFire);
        if (noShaders) Astral::Cosmetics::NoShaders::setEnabled(noShaders);
        if (noDashBoom) Astral::Cosmetics::NoOrbEffects::setNoDashBoom(noDashBoom);
        if (noOrbHitEffect) Astral::Cosmetics::NoOrbEffects::setNoOrbHitEffect(noOrbHitEffect);
        if (noOrbRing) Astral::Cosmetics::NoOrbEffects::setNoOrbRing(noOrbRing);
        if (noOrbEffectEnabled) Astral::Cosmetics::NoOrbEffects::setEnabled(noOrbEffectEnabled);
        if (ghostTrail) Astral::Cosmetics::NoGhostTrail::setEnabled(ghostTrail);
        if (noRespawnFlash) Astral::Cosmetics::NoRespawnFlash::setEnabled(noRespawnFlash);
        if (noDeathEffect) Astral::Cosmetics::NoDeathEffect::setEnabled(noDeathEffect);
        if (showHitboxes) Astral::Hacks::Hitboxes::setEnabled(showHitboxes);
        if (showHitboxTrail) Astral::Hacks::Hitboxes::setShowTrail(showHitboxTrail);
        if (showRotated) Astral::Hacks::Hitboxes::setShowRotated(showRotated);
        if (showFill) Astral::Hacks::Hitboxes::setShowFill(showFill);
        if (trailLength != 240) Astral::Hacks::Hitboxes::setTrailLength(trailLength);
        if (showInner) Astral::Hacks::Hitboxes::setShowInner(showInner);
        if (showCircle) Astral::Hacks::Hitboxes::setShowCircle(showCircle);
        if (accuratePercentage) Astral::QOL::AccuratePercentage::setEnabled(accuratePercentage);
        if (decimalPlaces != 1) Astral::QOL::AccuratePercentage::setDecimalPlaces(decimalPlaces);
        if (respawnDelay != 1.0f) Astral::QOL::RespawnDelay::setDelay(respawnDelay);
        if (safeMode) Astral::QOL::SafeMode::setEnabled(safeMode);
        if (tpsValue > 0.0f && tpsValue != 60.0f) Astral::Hacks::TPSBypass::setTPS(tpsValue);  // <-- Added check
        if (waveTrailDragFix) Astral::Cosmetics::WaveDragFix::setEnabled(waveTrailDragFix);
        if (hitboxThickness != 0.65f) Astral::Hacks::Hitboxes::setHitboxThickness(hitboxThickness);
        if (noTrail) Astral::Cosmetics::NoTrail::setEnabled(noTrail);
        if (labelsEnabled) {
            auto* labelSystem = Astral::Label::PlayerInfoLabels::getInstance();
            labelSystem->init();
            labelSystem->setVisible(labelsEnabled);
            labelSystem->setPlayer1Alignment(static_cast<Astral::Label::LabelContainer::Alignment>(player1Alignment));
            labelSystem->setPlayer2Alignment(static_cast<Astral::Label::LabelContainer::Alignment>(player2Alignment));
            labelSystem->setLabelScale(labelScale);
            labelSystem->setLabelOpacity(labelOpacity);
            labelSystem->setLabelVisibility(showLabelPosition, showLabelVelocity, showLabelStatus, 
                showLabelAttempt, showLabelProgress, showLabelFPS);
            }
        });
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
        loadAllSavedData();
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
            