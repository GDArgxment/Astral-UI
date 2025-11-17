#include "mobileUI.hpp"

#ifdef GEODE_IS_MOBILE
Ref<Astral_GUI_Mobile_UI> ins;
#define HorizontalInset 8
#define VerticalInset 9

int mobileCurrentTab = 0;
const char* mobileTabNames[] = {"Botting", "Hacks", "AutoClicker", "Render", "Settings"};
const int mobileTabCount = 5;

int mobileFrameCount = 0;

float getMobileCurrentFrame() {
    auto* playLayer = PlayLayer::get();
    if (!playLayer) return 0.0f;
    
    mobileFrameCount++;
    return (float)mobileFrameCount;
}

bool Astral_GUI_Mobile_UI::setup() {
    this->setAnchorPoint({0.5, 0});
    this->runAction(CCFadeTo::create(0.5f, 100));
    this->setKeypadEnabled(true);
    this->setTitle("Astral:", "bigFont.fnt", 0.7f, -12.5f);
    
    m_mainLayer->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Center));
    m_mainLayer->updateLayout();
    
    auto bgRight = CCScale9Sprite::create("square02_001.png");
    bgRight->setAnchorPoint(ccp(1, 0.5f));
    bgRight->setOpacity(60);
    bgRight->setContentWidth((this->m_size.width / 2) - HorizontalInset * 2);
    bgRight->setContentHeight(this->m_size.height - VerticalInset * 2.5);
    bgRight->setScale(1.0f / 2.0f);
    bgRight->setContentSize(bgRight->getContentSize() * 2);
    
    rightMenu = CCMenu::create();
    rightMenu->setLayout(AxisLayout::create()->setAxis(Axis::Column)->setGap(5));
    rightMenu->setAnchorPoint(ccp(0.5f, 0.5f));
    
    auto bgLeft = CCScale9Sprite::create("square02_001.png");
    bgLeft->setAnchorPoint(ccp(1, 0.5f));
    bgLeft->setOpacity(40);
    bgLeft->setContentWidth((this->m_size.width / 3.2) - HorizontalInset * 2);
    bgLeft->setContentHeight(this->m_size.height - VerticalInset * 2.5);
    bgLeft->setScale(1.0f / 2.0f);
    bgLeft->setContentSize(bgLeft->getContentSize() * 2);
    
    leftMenu = CCMenu::create();
    leftMenu->setLayout(AxisLayout::create()->setAxis(Axis::Column));
    leftMenu->setAnchorPoint(ccp(0.f, 0.5f));
    
    for (int i = 0; i < mobileTabCount; i++) {
        createTabBtn(mobileTabNames[i], menu_selector(Astral_GUI_Mobile_UI::onTabSelected), i);
    }
    
    updateRightMenuContent();
    
    m_mainLayer->addChildAtPosition(bgRight, Anchor::Right, ccp(-HorizontalInset, 0));
    m_mainLayer->addChildAtPosition(rightMenu, Anchor::Right, ccp(-75, 0));
    m_mainLayer->addChildAtPosition(bgLeft, Anchor::Left, ccp(HorizontalInset + 101, 0));
    m_mainLayer->addChildAtPosition(leftMenu, Anchor::Left, ccp(17.5, 0));
    
    return true;
}

void Astral_GUI_Mobile_UI::createTabBtn(const char* text, SEL_MenuHandler callback, int tabIndex) {
    auto spr = ButtonSprite::create(text, 65, 69420, .75f, true, "bigFont.fnt", "GJ_button_04.png", 30);
    
    auto btn = CCMenuItemSpriteExtra::create(spr, this, callback);
    btn->setTag(tabIndex);
    
    leftMenu->addChild(btn);
    leftMenu->updateLayout();
}

void Astral_GUI_Mobile_UI::onTabSelected(CCObject* sender) {
    auto menuItem = static_cast<CCMenuItemSpriteExtra*>(sender);
    mobileCurrentTab = menuItem->getTag();
    updateRightMenuContent();
}

void Astral_GUI_Mobile_UI::updateRightMenuContent() {
    rightMenu->removeAllChildren();
    
    switch (mobileCurrentTab) {
        case 0: createBottingContent(); break;
        case 1: createHacksContent(); break;
        case 2: createAutoClickerContent(); break;
        case 3: createRenderContent(); break;
        case 4: createSettingsContent(); break;
    }
    
    rightMenu->updateLayout();
}

void Astral_GUI_Mobile_UI::createBottingContent() {
    auto recordSpr = ButtonSprite::create("Record", 70, true, "goldFont.fnt", "GJ_button_01.png", 25, .5f);
    auto recordBtn = CCMenuItemSpriteExtra::create(recordSpr, this, menu_selector(Astral_GUI_Mobile_UI::onRecordMacro));
    rightMenu->addChild(recordBtn);
    
    auto playSpr = ButtonSprite::create("Play", 70, true, "goldFont.fnt", "GJ_button_01.png", 25, .5f);
    auto playBtn = CCMenuItemSpriteExtra::create(playSpr, this, menu_selector(Astral_GUI_Mobile_UI::onPlayMacro));
    rightMenu->addChild(playBtn);
    
    auto tpsLabel = CCLabelBMFont::create("TPS: 240", "chatFont.fnt");
    tpsLabel->setScale(0.7f);
    rightMenu->addChild(tpsLabel);
    
    auto trajectorySpr = ButtonSprite::create("Trajectory: OFF", 100, true, "chatFont.fnt", "GJ_button_04.png", 20, .6f);
    auto trajectoryBtn = CCMenuItemSpriteExtra::create(trajectorySpr, this, menu_selector(Astral_GUI_Mobile_UI::onToggleTrajectory));
    rightMenu->addChild(trajectoryBtn);
    
    auto stepperSpr = ButtonSprite::create("Frame Step: OFF", 100, true, "chatFont.fnt", "GJ_button_04.png", 20, .6f);
    auto stepperBtn = CCMenuItemSpriteExtra::create(stepperSpr, this, menu_selector(Astral_GUI_Mobile_UI::onToggleFrameStep));
    rightMenu->addChild(stepperBtn);
    
    float currentFrame = getMobileCurrentFrame();
    auto frameLabel = CCLabelBMFont::create(CCString::createWithFormat("Frame: %.0f", currentFrame)->getCString(), "chatFont.fnt");
    frameLabel->setScale(0.6f);
    rightMenu->addChild(frameLabel);
}

void Astral_GUI_Mobile_UI::createHacksContent() {
    auto noclipSpr = ButtonSprite::create("Noclip: OFF", 100, true, "chatFont.fnt", "GJ_button_04.png", 20, .6f);
    auto noclipBtn = CCMenuItemSpriteExtra::create(noclipSpr, this, menu_selector(Astral_GUI_Mobile_UI::onToggleNoclip));
    rightMenu->addChild(noclipBtn);
    
    auto speedSpr = ButtonSprite::create("Speedhack: 1.0x", 100, true, "chatFont.fnt", "GJ_button_04.png", 20, .6f);
    auto speedBtn = CCMenuItemSpriteExtra::create(speedSpr, this, menu_selector(Astral_GUI_Mobile_UI::onToggleSpeedhack));
    rightMenu->addChild(speedBtn);
    
    auto respawnSpr = ButtonSprite::create("Respawn: 2s", 100, true, "chatFont.fnt", "GJ_button_04.png", 20, .6f);
    auto respawnBtn = CCMenuItemSpriteExtra::create(respawnSpr, this, menu_selector(Astral_GUI_Mobile_UI::onToggleRespawn));
    rightMenu->addChild(respawnBtn);
    
    auto safeSpr = ButtonSprite::create("Safe Mode: OFF", 100, true, "chatFont.fnt", "GJ_button_04.png", 20, .6f);
    auto safeBtn = CCMenuItemSpriteExtra::create(safeSpr, this, menu_selector(Astral_GUI_Mobile_UI::onToggleSafeMode));
    rightMenu->addChild(safeBtn);
    
    auto deathSpr = ButtonSprite::create("No Death FX: OFF", 100, true, "bigFont.fnt", "GJ_button_04.png", 20, .6f);
    auto deathBtn = CCMenuItemSpriteExtra::create(deathSpr, this, menu_selector(Astral_GUI_Mobile_UI::onToggleNoDeath));
    rightMenu->addChild(deathBtn);
    
    auto shaderSpr = ButtonSprite::create("No Shaders: OFF", 100, true, "bigFont.fnt", "GJ_button_04.png", 20, .6f);
    auto shaderBtn = CCMenuItemSpriteExtra::create(shaderSpr, this, menu_selector(Astral_GUI_Mobile_UI::onToggleNoShaders));
    rightMenu->addChild(shaderBtn);
}

void Astral_GUI_Mobile_UI::createAutoClickerContent() {
    auto enableSpr = ButtonSprite::create("AutoClicker: OFF", 120, true, "goldFont.fnt", "GJ_button_01.png", 25, .6f);
    auto enableBtn = CCMenuItemSpriteExtra::create(enableSpr, this, menu_selector(Astral_GUI_Mobile_UI::onToggleAutoClicker));
    rightMenu->addChild(enableBtn);
    
    auto p1Label = CCLabelBMFont::create("Player 1:", "goldFont.fnt");
    p1Label->setScale(0.6f);
    rightMenu->addChild(p1Label);
    
    auto wSpr = ButtonSprite::create("W Key: OFF", 100, true, "bigFont.fnt", "GJ_button_04.png", 20, .6f);
    auto wBtn = CCMenuItemSpriteExtra::create(wSpr, this, menu_selector(Astral_GUI_Mobile_UI::onToggleW));
    rightMenu->addChild(wBtn);
    
    auto moveSpr = ButtonSprite::create("A/D Keys: OFF", 100, true, "bigFont.fnt", "GJ_button_04.png", 20, .6f);
    auto moveBtn = CCMenuItemSpriteExtra::create(moveSpr, this, menu_selector(Astral_GUI_Mobile_UI::onToggleMovement));
    rightMenu->addChild(moveBtn);
    
    auto p2Label = CCLabelBMFont::create("Player 2:", "goldFont.fnt");
    p2Label->setScale(0.6f);
    rightMenu->addChild(p2Label);
    
    auto upSpr = ButtonSprite::create("UP Key: OFF", 100, true, "bigFont.fnt", "GJ_button_04.png", 20, .6f);
    auto upBtn = CCMenuItemSpriteExtra::create(upSpr, this, menu_selector(Astral_GUI_Mobile_UI::onToggleUP));
    rightMenu->addChild(upBtn);
    
    auto arrowSpr = ButtonSprite::create("Arrow Keys: OFF", 100, true, "bigFont.fnt", "GJ_button_04.png", 20, .6f);
    auto arrowBtn = CCMenuItemSpriteExtra::create(arrowSpr, this, menu_selector(Astral_GUI_Mobile_UI::onToggleArrows));
    rightMenu->addChild(arrowBtn);
}

void Astral_GUI_Mobile_UI::createRenderContent() {
    auto layoutSpr = ButtonSprite::create("Layout: OFF", 100, true, "bigFont.fnt", "GJ_button_04.png", 20, .6f);
    auto layoutBtn = CCMenuItemSpriteExtra::create(layoutSpr, this, menu_selector(Astral_GUI_Mobile_UI::onToggleLayout));
    rightMenu->addChild(layoutBtn);
    
    auto startSpr = ButtonSprite::create("Start Render", 80, true, "goldFont.fnt", "GJ_button_01.png", 25, .6f);
    auto startBtn = CCMenuItemSpriteExtra::create(startSpr, this, menu_selector(Astral_GUI_Mobile_UI::onStartRender));
    rightMenu->addChild(startBtn);
    
    auto stopSpr = ButtonSprite::create("Stop Render", 80, true, "goldFont.fnt", "GJ_button_06.png", 25, .6f);
    auto stopBtn = CCMenuItemSpriteExtra::create(stopSpr, this, menu_selector(Astral_GUI_Mobile_UI::onStopRender));
    rightMenu->addChild(stopBtn);
    
    auto hitboxSpr = ButtonSprite::create("Hitboxes: OFF", 100, true, "chatFont.fnt", "GJ_button_04.png", 20, .6f);
    auto hitboxBtn = CCMenuItemSpriteExtra::create(hitboxSpr, this, menu_selector(Astral_GUI_Mobile_UI::onToggleHitboxes));
    rightMenu->addChild(hitboxBtn);
    
    auto gridSpr = ButtonSprite::create("Grid: OFF", 100, true, "chatFont.fnt", "GJ_button_04.png", 20, .6f);
    auto gridBtn = CCMenuItemSpriteExtra::create(gridSpr, this, menu_selector(Astral_GUI_Mobile_UI::onToggleGrid));
    rightMenu->addChild(gridBtn);
}

void Astral_GUI_Mobile_UI::createSettingsContent() {
    auto infoLabel = CCLabelBMFont::create("Mobile Settings:", "goldFont.fnt");
    infoLabel->setScale(0.7f);
    rightMenu->addChild(infoLabel);
    
    auto keyLabel = CCLabelBMFont::create("GUI Key: Menu Button", "chatFont.fnt");
    keyLabel->setScale(0.6f);
    rightMenu->addChild(keyLabel);
    
    auto resetSpr = ButtonSprite::create("Reset Frame Count", 120, true, "chatFont.fnt", "GJ_button_04.png", 20, .6f);
    auto resetBtn = CCMenuItemSpriteExtra::create(resetSpr, this, menu_selector(Astral_GUI_Mobile_UI::onResetFrameCount));
    rightMenu->addChild(resetBtn);
    
    auto aboutSpr = ButtonSprite::create("About Astral", 100, true, "goldFont.fnt", "GJ_button_01.png", 25, .6f);
    auto aboutBtn = CCMenuItemSpriteExtra::create(aboutSpr, this, menu_selector(Astral_GUI_Mobile_UI::onAbout));
    rightMenu->addChild(aboutBtn);
}

void Astral_GUI_Mobile_UI::onRecordMacro(CCObject*) {
}

void Astral_GUI_Mobile_UI::onPlayMacro(CCObject*) {
}

void Astral_GUI_Mobile_UI::onToggleTrajectory(CCObject*) {
    updateRightMenuContent();
}

void Astral_GUI_Mobile_UI::onToggleFrameStep(CCObject*) {
    updateRightMenuContent();
}

void Astral_GUI_Mobile_UI::onToggleNoclip(CCObject*) {
    updateRightMenuContent();
}

void Astral_GUI_Mobile_UI::onToggleSpeedhack(CCObject*) {
    updateRightMenuContent();
}

void Astral_GUI_Mobile_UI::onToggleRespawn(CCObject*) {
    updateRightMenuContent();
}

void Astral_GUI_Mobile_UI::onToggleSafeMode(CCObject*) {
    updateRightMenuContent();
}

void Astral_GUI_Mobile_UI::onToggleNoDeath(CCObject*) {
    updateRightMenuContent();
}

void Astral_GUI_Mobile_UI::onToggleNoShaders(CCObject*) {
    updateRightMenuContent();
}

void Astral_GUI_Mobile_UI::onToggleAutoClicker(CCObject*) {
    updateRightMenuContent();
}

void Astral_GUI_Mobile_UI::onToggleW(CCObject*) {
    updateRightMenuContent();
}

void Astral_GUI_Mobile_UI::onToggleMovement(CCObject*) {
    updateRightMenuContent();
}

void Astral_GUI_Mobile_UI::onToggleUP(CCObject*) {
    updateRightMenuContent();
}

void Astral_GUI_Mobile_UI::onToggleArrows(CCObject*) {
    updateRightMenuContent();
}

void Astral_GUI_Mobile_UI::onToggleLayout(CCObject*) {
    updateRightMenuContent();
}

void Astral_GUI_Mobile_UI::onStartRender(CCObject*) {
}

void Astral_GUI_Mobile_UI::onStopRender(CCObject*) {
}

void Astral_GUI_Mobile_UI::onToggleHitboxes(CCObject*) {
    updateRightMenuContent();
}

void Astral_GUI_Mobile_UI::onToggleGrid(CCObject*) {
    updateRightMenuContent();
}

void Astral_GUI_Mobile_UI::onResetFrameCount(CCObject*) {
    mobileFrameCount = 0;
    updateRightMenuContent();
}

void Astral_GUI_Mobile_UI::onAbout(CCObject*) {
    FLAlertLayer::create("About", "Astral Mobile v1.0\nGeometry Dash Mobile Mod", "OK")->show();
}

void Astral_GUI_Mobile_UI::onClose(CCObject* sender) {
    this->removeFromParent();
}

void Astral_GUI_Mobile_UI::keyBackClicked() {
    onClose(nullptr);
}

void Astral_GUI_Mobile_UI::onSettings(CCObject*) {
    geode::openSettingsPopup(Mod::get());
}

Astral_GUI_Mobile_UI* Astral_GUI_Mobile_UI::create() {
    auto pRet = new Astral_GUI_Mobile_UI();
    
    if (pRet && pRet->initAnchored(380, 240, "GJ_square05.png"))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
#endif