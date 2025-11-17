#pragma once
// #ifdef GEODE_IS_MOBILE
#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "../gui.hpp"

using namespace geode::prelude;

class Astral_GUI_Mobile_UI: public geode::Popup<> {
    public:
    virtual bool setup();
    CCMenu* leftMenu;
    CCMenu* rightMenu;
    
    void onClose(CCObject* sender);
    void keyBackClicked();
    
    void createTabBtn(const char* text, SEL_MenuHandler callback, int tabIndex);
    void updateRightMenuContent();
    
    void createBottingContent();
    void createHacksContent();
    void createAutoClickerContent();
    void createRenderContent();
    void createSettingsContent();
    
    void onTabSelected(CCObject* sender);
    
    void onRecordMacro(CCObject*);
    void onPlayMacro(CCObject*);
    void onToggleTrajectory(CCObject*);
    void onToggleFrameStep(CCObject*);
    
    void onToggleNoclip(CCObject*);
    void onToggleSpeedhack(CCObject*);
    void onToggleRespawn(CCObject*);
    void onToggleSafeMode(CCObject*);
    void onToggleNoDeath(CCObject*);
    void onToggleNoShaders(CCObject*);
    
    void onToggleAutoClicker(CCObject*);
    void onToggleW(CCObject*);
    void onToggleMovement(CCObject*);
    void onToggleUP(CCObject*);
    void onToggleArrows(CCObject*);
    
    void onToggleLayout(CCObject*);
    void onStartRender(CCObject*);
    void onStopRender(CCObject*);
    void onToggleHitboxes(CCObject*);
    void onToggleGrid(CCObject*);
    
    void onResetFrameCount(CCObject*);
    void onAbout(CCObject*);
    
    void onSettings(CCObject*);
    
    static Astral_GUI_Mobile_UI* create();
    static Astral_GUI_Mobile_UI* get();
};
// #endif