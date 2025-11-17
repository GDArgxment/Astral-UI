#pragma once

#include <cocos2d.h>
#include <string>
#include <vector>

// ==================== UI State ====================
extern bool recording;
extern bool replaying;
extern bool rendering;
extern bool initialized;
extern bool isCapturingKeybind;
extern bool guiVisible;
extern bool styleApplied;
extern bool currentGuiState;
// ==================== UI Configuration ====================
extern float themeColor[3];
extern float frameCount;
extern float tpsValue;

extern int selectedKeybind;
extern int selectedTheme;
extern int inputMerge;
extern int currentTab;
extern int backgroundTheme;
extern const int tabCount;

// ==================== UI Strings ====================
extern char macroName[128];
extern const char* tabNames[];
extern const char* currentKeyDisplay;
extern const char* getKeyName(cocos2d::enumKeyCodes keyCode);
extern cocos2d::enumKeyCodes capturedCustomKey;

// ==================== Autoclicker - Macro Definition ====================
#define DECL_AUTOKEY(NAME) \
    extern bool autoClick_##NAME##_enabled; \
    extern int autoClick_##NAME##_intervalHold; \
    extern int autoClick_##NAME##_intervalRelease; \
    extern int autoClick_##NAME##_clicksPerFrame; \
    extern bool autoClick_##NAME##_swiftClick; \
    extern bool autoClick_##NAME##_limitFrames; \
    extern int autoClick_##NAME##_maxFrames; \
    extern bool autoClick_##NAME##_blackOrbModeEnabled; \
    extern int autoClick_##NAME##_blackOrb_clickCount; \
    extern int autoClick_##NAME##_blackOrb_holdFrames;

// ==================== Autoclicker - Declarations ====================
DECL_AUTOKEY(W)
DECL_AUTOKEY(A)
DECL_AUTOKEY(D)
DECL_AUTOKEY(UP)
DECL_AUTOKEY(LEFT)
DECL_AUTOKEY(RIGHT)
DECL_AUTOKEY(SPACE)

// ==================== Level Tracking ====================
extern float lastLevelTime;
extern float lastProgress;

// ==================== Hack Data ====================
extern std::string selectedMacroName;
extern std::vector<std::string> availableMacros;
extern bool isRecording;
extern bool isPlayingMacro;
extern bool waveTrailDragFix;
extern bool trajectoryEnabled;
extern bool noclipEnabled;
extern bool noclipP1;
extern bool noclipP2;
extern bool speedhackEnabled;
extern float speedhackMultiplier;
extern int seedValue;
extern bool layoutEnabled;
extern float respawnDelay;
extern bool seedHackEnabled;
extern bool keepWaveEnabled;
extern int decimalPlaces;
extern bool accuratePercentage;
extern bool noMirror;
extern bool instantMirror;
extern bool noDashFire;
extern bool noShaders;
extern bool noDashBoom;
extern bool noOrbHitEffect;
extern bool noOrbRing;
extern bool noOrbEffectEnabled;
extern bool ghostTrail;
extern bool noRespawnFlash;
extern bool noDeathEffect;
extern bool noFade;
extern bool noReset;
extern bool showHitboxes;
extern bool showHitboxTrail;
extern bool showRotated;
extern bool showFill;
extern int trailLength;
extern bool showInner;
extern bool showCircle;
extern bool tpsBypassEnabled;
extern bool safeMode;
extern bool debugLog;
extern float hitboxThickness;
extern bool noTrail;
extern bool labelsEnabled;
extern float labelScale;
extern float labelOpacity;
extern int player1Alignment;
extern int player2Alignment;
extern bool showLabelPosition;
extern bool showLabelVelocity;
extern bool showLabelStatus;
extern bool showLabelAttempt;
extern bool showLabelProgress;
extern bool showLabelFPS;
// ==================== Macro System ====================
extern char macroSearchBuffer[256];
extern bool needsRefresh;