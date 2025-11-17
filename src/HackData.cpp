#include "includes.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

void saveHackData() {
    HackData data;
    data.capturedCustomKey = capturedCustomKey;
    // Botting & Macros
    data.selectedMacroName = selectedMacroName;
    data.availableMacros = availableMacros;
    data.isRecording = isRecording;
    
    // Movement Hacks
    data.waveTrailDragFix = waveTrailDragFix;
    data.trajectoryEnabled = trajectoryEnabled;
    data.noclipEnabled = noclipEnabled;
    data.noclipP1 = noclipP1;
    data.noclipP2 = noclipP2;
    data.speedhackEnabled = speedhackEnabled;
    data.speedhackMultiplier = speedhackMultiplier;
    
    // Level Hacks
    data.seedValue = seedValue;
    data.layoutEnabled = layoutEnabled;
    data.respawnDelay = respawnDelay;
    data.seedHackEnabled = seedHackEnabled;
    data.keepWaveEnabled = keepWaveEnabled;
    
    // Visual Hacks
    data.decimalPlaces = decimalPlaces;
    data.accuratePercentage = accuratePercentage;
    data.noMirror = noMirror;
    data.instantMirror = instantMirror;
    data.noDashFire = noDashFire;
    data.noShaders = noShaders;
    data.noDashBoom = noDashBoom;
    data.noOrbHitEffect = noOrbHitEffect;
    data.noOrbRing = noOrbRing;
    data.noOrbEffectEnabled = noOrbEffectEnabled;
    data.ghostTrail = ghostTrail;
    data.noRespawnFlash = noRespawnFlash;
    data.noDeathEffect = noDeathEffect;
    data.noFade = noFade;
    data.noReset = noReset;
    
    // Render & Debug
    data.showHitboxes = showHitboxes;
    data.showHitboxTrail = showHitboxTrail;
    data.showRotated = showRotated;
    data.showFill = showFill;
    data.trailLength = trailLength;
    data.showInner = showInner;
    data.showCircle = showCircle;
    data.hitboxThickness = hitboxThickness;
    
    // General
    
    data.tpsValue = tpsValue;
    data.tpsBypassEnabled = tpsBypassEnabled;
    data.safeMode = safeMode;
    data.styleApplied = styleApplied;
    data.currentGuiState = currentGuiState;
    data.selectedTheme = selectedTheme;
    data.backgroundTheme = backgroundTheme;
    data.capturedCustomKey = capturedCustomKey;
    data.debugLog = debugLog;
    data.noTrail = noTrail;
    data.labelsEnabled = labelsEnabled;
    data.labelScale = labelScale;
    data.labelOpacity = labelOpacity;
    data.player1Alignment = player1Alignment;
    data.player2Alignment = player2Alignment;
    data.showLabelPosition = showLabelPosition;
    data.showLabelVelocity = showLabelVelocity;
    data.showLabelStatus = showLabelStatus;
    data.showLabelAttempt = showLabelAttempt;
    data.showLabelProgress = showLabelProgress;
    data.showLabelFPS = showLabelFPS;
    // Save to mod storage
    Mod::get()->setSavedValue("hack-data", data);\
}

// ============ Quick Save Wrapper ============
// Use this after changing any setting
#define SAVE_HACK_DATA() saveHackData()