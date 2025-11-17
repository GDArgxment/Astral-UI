// ============ HackData.hpp ============
#pragma once
void saveHackData();
#include <Geode/Geode.hpp>
#include <string>
#include <vector>
using namespace geode::prelude;

struct HackData {
    std::string selectedMacroName;
    std::vector<std::string> availableMacros;
    bool isRecording;
    bool waveTrailDragFix;
    bool trajectoryEnabled;
    bool noclipEnabled;
    bool noclipP1;
    bool noclipP2;
    bool speedhackEnabled;
    float speedhackMultiplier;
    int seedValue;
    bool layoutEnabled;
    float respawnDelay;
    bool seedHackEnabled;
    bool keepWaveEnabled;
    int decimalPlaces;
    bool accuratePercentage;
    bool noMirror;
    bool instantMirror;
    bool noDashFire;
    bool noShaders;
    bool noDashBoom;
    bool noOrbHitEffect;
    bool noOrbRing;
    bool noOrbEffectEnabled;
    bool ghostTrail;
    bool noRespawnFlash;
    bool noDeathEffect;
    bool noFade;
    bool noReset;
    bool showHitboxes;
    bool showHitboxTrail;
    bool showRotated;
    bool showFill;
    int trailLength;
    bool showInner;
    bool showCircle;
    float tpsValue;
    bool tpsBypassEnabled;
    bool safeMode;
    bool styleApplied;
    bool currentGuiState;
    int selectedTheme;
    int backgroundTheme;
    float hitboxThickness;
    bool noTrail;
    bool labelsEnabled;
    float labelScale;
    float labelOpacity;
    int player1Alignment;
    int player2Alignment;
    bool showLabelPosition;
    bool showLabelVelocity;
    bool showLabelStatus;
    bool showLabelAttempt;
    bool showLabelProgress;
    bool showLabelFPS;
    cocos2d::enumKeyCodes capturedCustomKey;
    
    bool debugLog;
    
};

template<>
struct matjson::Serialize<HackData> {
    static Result<HackData> fromJson(matjson::Value const& value) {
        HackData data{};
        if (value.contains("capturedCustomKey")) {
            GEODE_UNWRAP_INTO(int keyCode, value["capturedCustomKey"].asInt());
            data.capturedCustomKey = (cocos2d::enumKeyCodes)keyCode;
        }
        if (value.contains("selectedMacroName")) {
            GEODE_UNWRAP_INTO(auto macro, value["selectedMacroName"].asString());
            data.selectedMacroName = macro;
        }
        
        if (value.contains("availableMacros") && value["availableMacros"].isArray()) {
            for (auto& macro : value["availableMacros"].asArray().unwrap()) {
                if (auto m = macro.asString()) {
                    data.availableMacros.push_back(m.unwrap());
                }
            }
        }
        if (value.contains("capturedCustomKey")) {
            GEODE_UNWRAP_INTO(int keyCode, value["capturedCustomKey"].asInt());
            data.capturedCustomKey = (cocos2d::enumKeyCodes)keyCode;
        }
        GEODE_UNWRAP_INTO(auto rec, value["isRecording"].asBool());
        data.isRecording = rec;
        GEODE_UNWRAP_INTO(auto wtdf, value["waveTrailDragFix"].asBool());
        data.waveTrailDragFix = wtdf;
        GEODE_UNWRAP_INTO(auto traj, value["trajectoryEnabled"].asBool());
        data.trajectoryEnabled = traj;
        GEODE_UNWRAP_INTO(auto noclip, value["noclipEnabled"].asBool());
        data.noclipEnabled = noclip;
        GEODE_UNWRAP_INTO(auto nc_p1, value["noclipP1"].asBool());
        data.noclipP1 = nc_p1;
        GEODE_UNWRAP_INTO(auto nc_p2, value["noclipP2"].asBool());
        data.noclipP2 = nc_p2;
        GEODE_UNWRAP_INTO(auto speedhack, value["speedhackEnabled"].asBool());
        data.speedhackEnabled = speedhack;
        GEODE_UNWRAP_INTO(float speedMult, value["speedhackMultiplier"].asDouble());
        data.speedhackMultiplier = speedMult;
        GEODE_UNWRAP_INTO(int seed, value["seedValue"].asInt());
        data.seedValue = seed;
        GEODE_UNWRAP_INTO(auto layout, value["layoutEnabled"].asBool());
        data.layoutEnabled = layout;
        GEODE_UNWRAP_INTO(float respawn, value["respawnDelay"].asDouble());
        data.respawnDelay = respawn;
        GEODE_UNWRAP_INTO(auto seedh, value["seedHackEnabled"].asBool());
        data.seedHackEnabled = seedh;
        GEODE_UNWRAP_INTO(auto keepWave, value["keepWaveEnabled"].asBool());
        data.keepWaveEnabled = keepWave;
        GEODE_UNWRAP_INTO(int decimals, value["decimalPlaces"].asInt());
        data.decimalPlaces = decimals;
        GEODE_UNWRAP_INTO(auto accPct, value["accuratePercentage"].asBool());
        data.accuratePercentage = accPct;
        GEODE_UNWRAP_INTO(auto noMir, value["noMirror"].asBool());
        data.noMirror = noMir;
        GEODE_UNWRAP_INTO(auto instMir, value["instantMirror"].asBool());
        data.instantMirror = instMir;
        GEODE_UNWRAP_INTO(auto noDash, value["noDashFire"].asBool());
        data.noDashFire = noDash;
        GEODE_UNWRAP_INTO(auto noShader, value["noShaders"].asBool());
        data.noShaders = noShader;
        GEODE_UNWRAP_INTO(auto noBoom, value["noDashBoom"].asBool());
        data.noDashBoom = noBoom;
        GEODE_UNWRAP_INTO(auto noOrbHit, value["noOrbHitEffect"].asBool());
        data.noOrbHitEffect = noOrbHit;
        GEODE_UNWRAP_INTO(auto noOrbRng, value["noOrbRing"].asBool());
        data.noOrbRing = noOrbRng;
        GEODE_UNWRAP_INTO(auto noOrbEff, value["noOrbEffectEnabled"].asBool());
        data.noOrbEffectEnabled = noOrbEff;
        GEODE_UNWRAP_INTO(auto ghost, value["ghostTrail"].asBool());
        data.ghostTrail = ghost;
        GEODE_UNWRAP_INTO(auto noResp, value["noRespawnFlash"].asBool());
        data.noRespawnFlash = noResp;
        GEODE_UNWRAP_INTO(auto noDeath, value["noDeathEffect"].asBool());
        data.noDeathEffect = noDeath;
        GEODE_UNWRAP_INTO(auto noFd, value["noFade"].asBool());
        data.noFade = noFd;
        GEODE_UNWRAP_INTO(auto noRst, value["noReset"].asBool());
        data.noReset = noRst;
        GEODE_UNWRAP_INTO(auto hitbox, value["showHitboxes"].asBool());
        data.showHitboxes = hitbox;
        GEODE_UNWRAP_INTO(auto hitboxTrail, value["showHitboxTrail"].asBool());
        data.showHitboxTrail = hitboxTrail;
        GEODE_UNWRAP_INTO(auto rotated, value["showRotated"].asBool());
        data.showRotated = rotated;
        GEODE_UNWRAP_INTO(auto fill, value["showFill"].asBool());
        data.showFill = fill;
        GEODE_UNWRAP_INTO(int trail, value["trailLength"].asInt());
        data.trailLength = trail;
        GEODE_UNWRAP_INTO(auto inner, value["showInner"].asBool());
        data.showInner = inner;
        GEODE_UNWRAP_INTO(auto circle, value["showCircle"].asBool());
        data.showCircle = circle;
        
        GEODE_UNWRAP_INTO(float tps, value["tpsValue"].asDouble());
        data.tpsValue = tps;
        GEODE_UNWRAP_INTO(auto tpsBp, value["tpsBypassEnabled"].asBool());
        data.tpsBypassEnabled = tpsBp;
        GEODE_UNWRAP_INTO(auto safe, value["safeMode"].asBool());
        data.safeMode = safe;
        GEODE_UNWRAP_INTO(auto debugLog, value["debugLog"].asBool());
        data.debugLog = debugLog;
        GEODE_UNWRAP_INTO(float hitboxThick, value["hitboxThickness"].asDouble());
        data.hitboxThickness = hitboxThickness;  
        GEODE_UNWRAP_INTO(auto noTrailVal, value["noTrail"].asBool());
        data.noTrail = noTrailVal;
        GEODE_UNWRAP_INTO(auto labelsEn, value["labelsEnabled"].asBool());
        data.labelsEnabled = labelsEn;
        GEODE_UNWRAP_INTO(float labelScale, value["labelScale"].asDouble());
        data.labelScale = labelScale;
        GEODE_UNWRAP_INTO(float labelOpacity, value["labelOpacity"].asDouble());
        data.labelOpacity = labelOpacity;
        GEODE_UNWRAP_INTO(int p1Align, value["player1Alignment"].asInt());
        data.player1Alignment = p1Align;
        GEODE_UNWRAP_INTO(int p2Align, value["player2Alignment"].asInt());
        data.player2Alignment = p2Align;
        GEODE_UNWRAP_INTO(auto showPos, value["showLabelPosition"].asBool());
        data.showLabelPosition = showPos;
        GEODE_UNWRAP_INTO(auto showVel, value["showLabelVelocity"].asBool());
        data.showLabelVelocity = showVel;
        GEODE_UNWRAP_INTO(auto showStat, value["showLabelStatus"].asBool());
        data.showLabelStatus = showStat;
        GEODE_UNWRAP_INTO(auto showAtt, value["showLabelAttempt"].asBool());
        data.showLabelAttempt = showAtt;
        GEODE_UNWRAP_INTO(auto showProg, value["showLabelProgress"].asBool());
        data.showLabelProgress = showProg;
        GEODE_UNWRAP_INTO(auto showFps, value["showLabelFPS"].asBool());
        data.showLabelFPS = showFps;
        return Ok(data);
    }
    
    static matjson::Value toJson(HackData const& value) {
        auto obj = matjson::Value::object();
        obj["capturedCustomKey"] = (int)value.capturedCustomKey;
        obj["selectedMacroName"] = value.selectedMacroName;
        auto macros = matjson::Value::array();
        for (auto& m : value.availableMacros) {
            macros.push(m);
        }
        obj["availableMacros"] = macros;
        obj["isRecording"] = value.isRecording;
        obj["waveTrailDragFix"] = value.waveTrailDragFix;
        obj["trajectoryEnabled"] = value.trajectoryEnabled;
        obj["noclipEnabled"] = value.noclipEnabled;
        obj["noclipP1"] = value.noclipP1;
        obj["noclipP2"] = value.noclipP2;
        obj["speedhackEnabled"] = value.speedhackEnabled;
        obj["speedhackMultiplier"] = value.speedhackMultiplier;
        obj["seedValue"] = value.seedValue;
        obj["layoutEnabled"] = value.layoutEnabled;
        obj["respawnDelay"] = value.respawnDelay;
        obj["seedHackEnabled"] = value.seedHackEnabled;
        obj["keepWaveEnabled"] = value.keepWaveEnabled;
        obj["decimalPlaces"] = value.decimalPlaces;
        obj["accuratePercentage"] = value.accuratePercentage;
        obj["noMirror"] = value.noMirror;
        obj["instantMirror"] = value.instantMirror;
        obj["noDashFire"] = value.noDashFire;
        obj["noShaders"] = value.noShaders;
        obj["noDashBoom"] = value.noDashBoom;
        obj["noOrbHitEffect"] = value.noOrbHitEffect;
        obj["noOrbRing"] = value.noOrbRing;
        obj["noOrbEffectEnabled"] = value.noOrbEffectEnabled;
        obj["ghostTrail"] = value.ghostTrail;
        obj["noRespawnFlash"] = value.noRespawnFlash;
        obj["noDeathEffect"] = value.noDeathEffect;
        obj["noFade"] = value.noFade;
        obj["noReset"] = value.noReset;
        obj["showHitboxes"] = value.showHitboxes;
        obj["showHitboxTrail"] = value.showHitboxTrail;
        obj["showRotated"] = value.showRotated;
        obj["showFill"] = value.showFill;
        obj["trailLength"] = value.trailLength;
        obj["showInner"] = value.showInner;
        obj["showCircle"] = value.showCircle;
        
        auto colors = matjson::Value::array();
        obj["tpsValue"] = value.tpsValue;
        obj["tpsBypassEnabled"] = value.tpsBypassEnabled;
        obj["safeMode"] = value.safeMode;
        obj["capturedCustomKey"] = (int)value.capturedCustomKey;
        obj["debugLog"] = value.debugLog;
        obj["hitboxThickness"] = value.hitboxThickness;
        obj["noTrail"] = value.noTrail;
        obj["labelsEnabled"] = value.labelsEnabled;
        obj["labelScale"] = value.labelScale;
        obj["labelOpacity"] = value.labelOpacity;
        obj["player1Alignment"] = value.player1Alignment;
        obj["player2Alignment"] = value.player2Alignment;
        obj["showLabelPosition"] = value.showLabelPosition;
        obj["showLabelVelocity"] = value.showLabelVelocity;
        obj["showLabelStatus"] = value.showLabelStatus;
        obj["showLabelAttempt"] = value.showLabelAttempt;
        obj["showLabelProgress"] = value.showLabelProgress;
        obj["showLabelFPS"] = value.showLabelFPS;
        return matjson::Value(obj);
    }
};