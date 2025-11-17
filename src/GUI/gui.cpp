#include "includes.hpp"
#include "Logic/Keybinds/keybinds.hpp"
#include <Geode/utils/file.hpp> // geode::Result<>watchFile(std::filesystem::pathconst&file)
#include <cocos2d.h>

#ifdef GEODE_IS_MACOS
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <CoreGraphics/CoreGraphics.h>
#endif


using namespace geode::prelude;
//using namespace Astral::Hacks;
//using namespace Astral::QOL;
//using namespace Astral::Cosmetics;
using namespace Astral::Hacks::AutoClicker;

bool recording = false;
bool replaying = false;
bool rendering = false;
bool guiVisible = false;
bool isCapturingKeybind = false;
bool styleApplied = true;
bool currentGuiState = true;
bool debugLog = true;

// UI Configuration
float tpsValue = 60.0f;

int selectedKeybind = 0;
int inputMerge = 0;
int fontList = 0;
int fontType = 0;
int currentTab = 0;
int selectedTheme = 0;
int backgroundTheme = 0;
const int tabCount = 6;
const int backgroundThemeNamesCount = 0;

char macroName[128] = "";
const char* backgroundThemeNames[] = {nullptr};
const char* tabNames[] = {"Botting", "Hacks", "Autoclicker","Labels", "Render", "Settings"};
const char* currentKeyDisplay = nullptr;

// Hack Data
std::string selectedMacroName = "";
std::vector<std::string> availableMacros;
bool isRecording = false;
bool isPlayingMacro = false;
bool waveTrailDragFix = false;
bool trajectoryEnabled = false;
bool noclipEnabled = false;
bool noclipP1 = false;
bool noclipP2 = false;
bool speedhackEnabled = true;
float speedhackMultiplier = 1.0f;
int seedValue = 0;
bool layoutEnabled = false;
float respawnDelay = 1.0f;
bool seedHackEnabled = false;
bool keepWaveEnabled = false;
int decimalPlaces = 1;
bool accuratePercentage = false;
bool noMirror = false;
bool instantMirror = false;
bool noDashFire = false;
bool noShaders = false;
bool noDashBoom = false;
bool noOrbHitEffect = false;
bool noOrbRing = false;
bool noOrbEffectEnabled = false;
bool ghostTrail = false;
bool noRespawnFlash = false;
bool noDeathEffect = false;
bool noFade = false;
bool noReset = false;
bool showHitboxes = false;
bool showHitboxTrail = false;
bool showRotated = true;
bool showFill = false;
int trailLength = 240;
bool showInner = true;
bool showCircle = true;
bool tpsBypassEnabled = true;
bool safeMode = false;
float lastLevelTime = 0.0f;
bool noTrail = false;
// Macro System
char macroSearchBuffer[256] = "";
bool needsRefresh = true;
float hitboxThickness = 0.65f;
static std::string rightClickedButtonId = "";

bool labelsEnabled = true;
float labelScale = 1.0f;
float labelOpacity = 1.0f;
int player1Alignment = 0;  // TopLeft
int player2Alignment = 2;  // TopRight
bool showLabelPosition = true;
bool showLabelVelocity = true;
bool showLabelStatus = true;
bool showLabelAttempt = true;
bool showLabelProgress = true;
bool showLabelFPS = true;

namespace InputStacking {
    struct Config {
        bool enabled = false;
        int stackKey = 0;  // Which key to stack clicks on
        int multiplier = 2; // How many times to repeat the action
    };
    
    static Config config;
    
    // Map key index to player button
    static int keyToButton(int keyIndex) {
        switch(keyIndex) {
            case 0: return static_cast<int>(PlayerButton::Jump);   // W
            case 1: return static_cast<int>(PlayerButton::Left);   // A
            case 2: return static_cast<int>(PlayerButton::Jump);   // S
            case 3: return static_cast<int>(PlayerButton::Right);  // D
            case 4: return static_cast<int>(PlayerButton::Jump);   // SPACE
            case 5: return static_cast<int>(PlayerButton::Jump);   // UP
            case 6: return static_cast<int>(PlayerButton::Jump);   // DOWN
            case 7: return static_cast<int>(PlayerButton::Left);   // LEFT
            case 8: return static_cast<int>(PlayerButton::Right);  // RIGHT
            default: return static_cast<int>(PlayerButton::Jump);
        }
    }
    
    static bool keyIsPlayer1(int keyIndex) {
        return keyIndex < 5;  // W, A, S, D, SPACE = Player 1
    }
    
    // Call this from GJBaseGameLayer::handleButton to intercept and multiply key presses
    void onHandleButton(GJBaseGameLayer* gameLayer, bool down, int button, bool isPlayer1) {
        if (!config.enabled || !gameLayer) return;
        
        int stackButton = keyToButton(config.stackKey);
        bool stackIsPlayer1 = keyIsPlayer1(config.stackKey);
        
        // Check if this is the stack key
        if (button != stackButton || isPlayer1 != stackIsPlayer1) return;
        
        // Send the extra clicks/holds in the same frame
        for (int i = 1; i < config.multiplier; i++) {
            gameLayer->handleButton(down, button, isPlayer1);
        }
    }
}

// Call this to refresh the macro list from Astral/Macros folder
void refreshMacroList() {
    availableMacros.clear();
    
    std::filesystem::path macrosPath = MacrosDir();
    
    try {
        if (std::filesystem::exists(macrosPath)) {
            for (const auto& entry : std::filesystem::directory_iterator(macrosPath)) {
                // Check if it's a .ast file
                if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".ast") {
                    // Add the filename without extension
                    availableMacros.push_back(entry.path().stem().string());
                }
            }
        } else {
            AstralLogger::log("Macros directory not found at: {}", macrosPath.string());
        }
    } catch (const std::exception& e) {
        AstralLogger::log("Error loading macros: {}", e.what());
    }
    
    // Sort alphabetically
    std::sort(availableMacros.begin(), availableMacros.end());
    needsRefresh = false;
}
bool KeybindButton(const std::string& id, const std::string& label, ImVec2 size = ImVec2(0, 0)) {
    auto manager = KeybindManager::get();
    auto bind = manager->getKeybind(id);
    
    // Show button with current keybind in brackets
    std::string displayLabel = label;
    if (bind && bind->keyCode != cocos2d::enumKeyCodes::KEY_None) {
        displayLabel += " [" + manager->getKeyName(bind->keyCode) + "]";
    }
    
    bool pressed = ImGui::Button(displayLabel.c_str(), size);
    
    // Right-click to open keybind config
    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup(id.c_str());
    }
    
    // Keybind config popup
    if (ImGui::BeginPopupModal(id.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Configure keybind for: %s", label.c_str());
        ImGui::Separator();
        
        if (manager->isCapturingInput() && manager->getCapturingBindId() == id) {
            ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "Press any key now...");
        } else {
            std::string currentKey = (bind && bind->keyCode != cocos2d::enumKeyCodes::KEY_None) 
            ? manager->getKeyName(bind->keyCode) 
            : "Not set";
            ImGui::Text("Current: %s", currentKey.c_str());
            ImGui::Spacing();
            
            if (ImGui::Button("Click here, then press a key", ImVec2(300, 30))) {
                manager->startCapturing(id);
            }
            ImGui::SameLine();
            if (ImGui::Button("Clear", ImVec2(80, 30))) {
                manager->setKeybindKey(id, cocos2d::enumKeyCodes::KEY_None);
            }
        }
        
        ImGui::Spacing();
        if (ImGui::Button("Done", ImVec2(100, 0))) {
            manager->stopCapturing();
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::EndPopup();
    }
    
    return pressed;
}
void SetupKeybinds() {
    auto manager = KeybindManager::get();
    
    // Register noclip keybind
    manager->registerKeybind(
        "noclip_btn",
        "Toggle Noclip",
        []() { 
            noclipEnabled = !noclipEnabled;
            Astral::Hacks::Noclip::setNoclipEnabled(noclipEnabled);
            saveHackData();
            AstralLogger::log("Noclip toggled: {}", noclipEnabled);
        },
        cocos2d::enumKeyCodes::KEY_N
    );
    
    // Register record keybind
    manager->registerKeybind(
        "record_btn",
        "Record Macro",
        []() { 
            AstralLogger::log("Record macro keybind triggered!");
        },
        cocos2d::enumKeyCodes::KEY_R
    );
    
    // Register play keybind
    manager->registerKeybind(
        "play_btn",
        "Play Macro",
        []() { 
            AstralLogger::log("Play macro keybind triggered!");
        },
        cocos2d::enumKeyCodes::KEY_P
    );
    
    // Load saved keybinds
    manager->loadKeybinds();
}


#ifdef GEODE_IS_DESKTOP
void applyBackgroundTheme()
{
    auto& style = ImGui::GetStyle();
}

void setupImGuiStyle()
{
    auto& style = ImGui::GetStyle();
    auto& io = ImGui::GetIO();
    auto* font = io.Fonts->AddFontFromFileTTF(
        (Mod::get()->getResourcesDir() / ("font" + std::to_string(0) + ".ttf")).string().c_str(), 16.0f
    );
    
    auto& colors = style.Colors;
    
    style.Alpha = 1.0f;
    style.WindowRounding = 8.0f;
    style.ChildRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.PopupRounding = 6.0f;
    style.ScrollbarRounding = 8.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 4.0f;
    
    style.WindowPadding = ImVec2(10, 10);
    style.FramePadding = ImVec2(8, 4);
    style.ItemSpacing = ImVec2(8, 6);
    style.ItemInnerSpacing = ImVec2(6, 4);
    
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;
    style.PopupBorderSize = 0.0f;
    
    style.AntiAliasedLines = true;
    style.AntiAliasedFill = true;
    
    const ImVec4 BG_DARK = ImVec4(0.08f, 0.08f, 0.10f, 1.0f);
    const ImVec4 BG_PANEL = ImVec4(0.12f, 0.12f, 0.16f, 1.0f);
    const ImVec4 ACCENT_BLUE = ImVec4(0.13f, 0.25f, 0.5f, 1.0f);
    const ImVec4 ACCENT_BLUE_HOVER = ImVec4(0.18f, 0.35f, 0.6f, 1.0f);
    const ImVec4 ACCENT_BLUE_ACTIVE = ImVec4(0.1f, 0.2f, 0.45f, 1.0f);
    const ImVec4 TEXT_LIGHT = ImVec4(0.9f, 0.9f, 0.92f, 1.0f);
    
    colors[ImGuiCol_WindowBg] = BG_DARK;
    colors[ImGuiCol_ChildBg] = BG_PANEL;
    colors[ImGuiCol_PopupBg] = BG_DARK;
    colors[ImGuiCol_Text] = TEXT_LIGHT;
    colors[ImGuiCol_TextDisabled] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
    colors[ImGuiCol_Border] = ImVec4(0.2f, 0.2f, 0.25f, 0.5f);
    colors[ImGuiCol_Separator] = ImVec4(0.2f, 0.2f, 0.25f, 1.0f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    
    colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.20f, 0.9f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.28f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.18f, 0.25f, 1.0f);
    
    colors[ImGuiCol_Button] = ImVec4(0.15f, 0.15f, 0.20f, 0.9f);
    colors[ImGuiCol_ButtonHovered] = ACCENT_BLUE_HOVER;
    colors[ImGuiCol_ButtonActive] = ACCENT_BLUE_ACTIVE;
    
    colors[ImGuiCol_CheckMark] = ACCENT_BLUE;
    
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.12f, 0.5f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.25f, 0.25f, 0.30f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.35f, 0.35f, 0.40f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.45f, 0.45f, 0.50f, 1.0f);
    
    colors[ImGuiCol_SliderGrab] = ImVec4(0.30f, 0.30f, 0.35f, 1.0f);
    colors[ImGuiCol_SliderGrabActive] = ACCENT_BLUE;
    
    colors[ImGuiCol_Header] = ImVec4(0.15f, 0.15f, 0.20f, 0.8f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.35f, 0.95f);
    colors[ImGuiCol_HeaderActive] = ACCENT_BLUE;
    
    colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.20f, 0.9f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.25f, 0.25f, 0.35f, 1.0f);
    colors[ImGuiCol_TabActive] = ACCENT_BLUE;
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.12f, 0.12f, 0.16f, 0.8f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.28f, 1.0f);
    styleApplied = true;
    
}
void renderBottingTab()
{
    
    // ===== MACRO SELECTION & CONTROL SECTION =====
    ImGui::SetNextItemWidth(430);  // Make macro box wider
    
    static bool showMacroDropdown = false;
    
    if (ImGui::InputTextWithHint("##MacroSearch", "Search or type name...", macroSearchBuffer, sizeof(macroSearchBuffer))) {
        selectedMacroName = macroSearchBuffer;
        showMacroDropdown = true;
    }
    
    if (ImGui::IsItemClicked()) {
        showMacroDropdown = !showMacroDropdown;
        if (showMacroDropdown) {
            refreshMacroList();
        }
    }
    
    // Show compact dropdown
    if (showMacroDropdown) {
        std::string searchStr = macroSearchBuffer;
        std::transform(searchStr.begin(), searchStr.end(), searchStr.begin(), ::tolower);
        
        int matchCount = 0;
        for (size_t i = 0; i < availableMacros.size(); i++) {
            std::string macroLower = availableMacros[i];
            std::transform(macroLower.begin(), macroLower.end(), macroLower.begin(), ::tolower);
            if (searchStr.empty() || macroLower.find(searchStr) != std::string::npos) {
                matchCount++;
            }
        }
        
        float dropdownHeight = matchCount <= 5 ? 20.0f * matchCount : 100.0f;
        
        if (ImGui::BeginChild("##MacroDropdown", ImVec2(300, dropdownHeight), true, ImGuiWindowFlags_NoTitleBar)) {
            for (size_t i = 0; i < availableMacros.size(); i++) {
                std::string macroLower = availableMacros[i];
                std::transform(macroLower.begin(), macroLower.end(), macroLower.begin(), ::tolower);
                
                if (searchStr.empty() || macroLower.find(searchStr) != std::string::npos) {
                    bool isSelected = (selectedMacroName == availableMacros[i]);
                    if (ImGui::Selectable(availableMacros[i].c_str(), isSelected)) {
                        selectedMacroName = availableMacros[i];
                        memset(macroSearchBuffer, 0, sizeof(macroSearchBuffer));
                        #ifdef GEODE_IS_WINDOWS
                        strcpy_s(macroSearchBuffer, sizeof(macroSearchBuffer), availableMacros[i].c_str());
                        #else
                        strncpy(macroSearchBuffer, availableMacros[i].c_str(), sizeof(macroSearchBuffer) - 1);
                        macroSearchBuffer[sizeof(macroSearchBuffer) - 1] = '\0';
                        #endif
                        showMacroDropdown = false;
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
            }
            ImGui::EndChild();
        }
    }
    
    ImGui::Spacing();
    
    ImVec4 recordBg = isRecording ? ImVec4(0.8f, 0.25f, 0.25f, 1.0f) : ImVec4(0.15f, 0.3f, 0.55f, 1.0f);
    ImVec4 recordBgHovered = isRecording ? ImVec4(0.95f, 0.4f, 0.4f, 1.0f) : ImVec4(0.2f, 0.4f, 0.65f, 1.0f);
    ImVec4 recordBgActive = isRecording ? ImVec4(0.65f, 0.15f, 0.15f, 1.0f) : ImVec4(0.1f, 0.25f, 0.5f, 1.0f);
    
    ImGui::PushStyleColor(ImGuiCol_Button, recordBg);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, recordBgHovered);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, recordBgActive);
    
    if (ImGui::Button("Record##btn", ImVec2(225, 0))) {
        if (!selectedMacroName.empty()) {
            isRecording = !isRecording;
            if (isRecording) {
                isPlayingMacro = false;
                Astral::Bot::Recording::StartRecording(selectedMacroName);
                AstralLogger::log("Recording started: {}", selectedMacroName);
            } else {
                Astral::Bot::Recording::StopRecording();
                Astral::Bot::Recording::SaveMacro();
                needsRefresh = true;
                AstralLogger::log("Macro saved: {}", selectedMacroName);
            }
        } else {
            AstralLogger::log("Please enter a macro name");
        }
    }
    
    ImGui::PopStyleColor(3);
    ImGui::SameLine();
    ImVec4 playBg = isPlayingMacro ? ImVec4(0.25f, 0.7f, 0.3f, 1.0f) : ImVec4(0.15f, 0.3f, 0.55f, 1.0f);
    ImVec4 playBgHovered = isPlayingMacro ? ImVec4(0.4f, 0.85f, 0.45f, 1.0f) : ImVec4(0.2f, 0.4f, 0.65f, 1.0f);
    ImVec4 playBgActive = isPlayingMacro ? ImVec4(0.15f, 0.6f, 0.2f, 1.0f) : ImVec4(0.1f, 0.25f, 0.5f, 1.0f);
    
    ImGui::PushStyleColor(ImGuiCol_Button, playBg);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, playBgHovered);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, playBgActive);
    
    if (ImGui::Button("Play##btn", ImVec2(225, 0))) {
        if (!selectedMacroName.empty()) {
            isPlayingMacro = !isPlayingMacro;
            if (isPlayingMacro) {
                isRecording = false;
                Astral::Bot::Recording::LoadMacro(selectedMacroName);
                Astral::Bot::Recording::Playback();
                AstralLogger::log("Playing macro: {}", selectedMacroName);
            } else {
                Astral::Bot::Recording::StopPlayback();
                AstralLogger::log("Stopped playback");
            }
        } else {
            AstralLogger::log("Please select a macro");
        }
    }
    
    ImGui::PopStyleColor(3);
    
    ImGui::Spacing();
    
    // Status indicator
    if (isRecording || isPlayingMacro) {
        ImGui::PushStyleColor(ImGuiCol_Text, isRecording ? ImVec4(1.0f, 0.3f, 0.3f, 1.0f) : ImVec4(0.3f, 1.0f, 0.3f, 1.0f));
        ImGui::TextWrapped("%s", isRecording ? "Recording in progress..." : "Playback in progress...");
        ImGui::PopStyleColor();
    }
    
    // Remove extra spacing, just separator
    ImGui::Separator();
    
    ImGui::AlignTextToFramePadding();
    ImGui::Text("TPS Value:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    if (ImGui::InputFloat("##TPS Value:", &tpsValue)) {
        AstralLogger::log("TPS Value set to: {}", tpsValue);
        if (tpsValue < 0.0f) {
            tpsValue = 240.f;
        }
        Astral::Hacks::TPSBypass::setTPS(tpsValue);
        saveHackData();
    }
    ImGui::SameLine();
    ImGui::Text("Speedhack:");
    ImGui::SameLine();
    ImGui::AlignTextToFramePadding();
    if (ImGui::Checkbox("##Speedhack:", &speedhackEnabled)) {
        Astral::Hacks::Speedhack::setSpeedhackEnabled(speedhackEnabled);
        saveHackData();
    }
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);  // Small width for multiplier
    if (ImGui::DragFloat("##Multiplier:", &speedhackMultiplier, 0.01f, 0.1f, 0.0f, "%.1f")) {
        AstralLogger::log("Speedhack Multiplier set to: {}", speedhackMultiplier);
        if (speedhackMultiplier <= 0.f) {
            speedhackMultiplier = 1.0f;
        }
        saveHackData();
        Astral::Hacks::Speedhack::setSpeedhackMultiplier(speedhackMultiplier);
    }
    ImGui::SameLine();
    ImGui::Text("Seed Hack:");
    ImGui::SameLine();
    if (ImGui::Checkbox("##SeedHack", &seedHackEnabled)) {
        Astral::Hacks::Seedhack::setEnabled(seedHackEnabled);
        saveHackData();
    }
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    if (ImGui::DragInt("##Seed", &seedValue, 1.0f)) {
        AstralLogger::log("Seed Value set to: {}", seedValue);
        Astral::Hacks::Seedhack::setSeedValue(seedValue);
        saveHackData();
    }
    ImGui::Separator();
    ImGui::BeginDisabled();
    if (ImGui::Checkbox("Show Trajectory", &trajectoryEnabled)) {
        // Astral::Hacks::Trajectory::setEnabled(trajectoryEnabled);
    }
    ImGui::EndDisabled();
    ImGui::SameLine();
    if (ImGui::Checkbox("Noclip", &noclipEnabled)) {
        Astral::Hacks::Noclip::setNoclipEnabled(noclipEnabled);
        saveHackData();
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Players")) {
        ImGui::OpenPopup("NoclipPlayersPopup");
    }
    if (ImGui::BeginPopup("NoclipPlayersPopup")) {
        if (ImGui::Checkbox("Player 1", &noclipP1)) {
            Astral::Hacks::Noclip::setNoclipP1(noclipP1);
            saveHackData();
        }
        if (ImGui::Checkbox("Player 2", &noclipP2)) {
            Astral::Hacks::Noclip::setNoclipP2(noclipP2);
            saveHackData();
        }
        ImGui::EndPopup();
    }
    
    ImGui::Separator();
    static bool framestepEnabled = false;
    framestepEnabled = Astral::Hacks::FrameStepper::isEnabled();
    if (ImGui::Checkbox("Frame Stepper", &framestepEnabled)) {
        Astral::Hacks::FrameStepper::setEnabled(framestepEnabled);
    }
    ImGui::SameLine();
    if (ImGui::Button("Step")) {
        Astral::Hacks::FrameStepper::stepFrame();
    }
    ImGui::SameLine();
    if (ImGui::Button("Back Step")) {
        Astral::Hacks::FrameStepper::stepBack();
    }
    ImGui::Separator();
    if (ImGui::Checkbox("Show Layout", &layoutEnabled)) {
        Astral::Hacks::LayoutMode::setEnabled(layoutEnabled);
        saveHackData();
    }
    ImGui::SameLine();
    // Main hitbox toggle
    if (ImGui::Checkbox("Show Hitboxes", &showHitboxes)) {
        Astral::Hacks::Hitboxes::setEnabled(showHitboxes);
        saveHackData();
    }
    ImGui::SameLine();
    if (ImGui::Button("⚙")) { // ⚙ for gear icon
        ImGui::OpenPopup("HitboxSettingsPopup");
    }
    
    if (ImGui::BeginPopup("HitboxSettingsPopup")) {
        ImGui::Text("Hitbox Display Options");
        ImGui::Separator();
        
        if (ImGui::Checkbox("Show Trail##hitbox", &showHitboxTrail)) {
            Astral::Hacks::Hitboxes::setShowTrail(showHitboxTrail);
            saveHackData();
        }
        
        ImGui::SetNextItemWidth(150);
        if (ImGui::SliderInt("Trail Length", &trailLength, 1, 2000)) {
            Astral::Hacks::Hitboxes::setTrailLength(trailLength);
            saveHackData();
        }
        
        ImGui::Separator();
        
        if (ImGui::Checkbox("Rotated Hitbox", &showRotated)) {
            Astral::Hacks::Hitboxes::setShowRotated(showRotated);
            saveHackData();
        }
        
        if (ImGui::Checkbox("Inner Hitbox", &showInner)) {
            Astral::Hacks::Hitboxes::setShowInner(showInner);
            saveHackData();
        }
        
        if (ImGui::Checkbox("Circle Hitbox", &showCircle)) {
            Astral::Hacks::Hitboxes::setShowCircle(showCircle);
            saveHackData();
        }
        
        if (ImGui::Checkbox("Fill", &showFill)) {
            Astral::Hacks::Hitboxes::setShowFill(showFill);
            saveHackData();
        }
        if (ImGui::SliderFloat("Hitbox Thickness", &hitboxThickness, 0.1f, 2.0f, "%.2f")) {
            Astral::Hacks::Hitboxes::setHitboxThickness(hitboxThickness);
        }
        ImGui::EndPopup();
    }
}

void renderHacksTab()
{
    // Respawn & Safe Mode
    ImGui::Text("Respawn Time");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(80.0f);
    if (ImGui::InputFloat("##RespawnDelay", &respawnDelay, 0.0f, 0.0f, "%.1f")) {
        if (respawnDelay < 0.f) {
            respawnDelay = 1.f;
        } else if (respawnDelay > 2.f){
            respawnDelay = 1.f;
        }
        saveHackData();
        Astral::QOL::RespawnDelay::setDelay(respawnDelay);
    }
    ImGui::SameLine();
    if (ImGui::Checkbox("Safe Mode", &safeMode)) {
        Astral::QOL::SafeMode::setEnabled(safeMode);
        saveHackData();
    }
    
    ImGui::Separator();
    
    // Death & Respawn Effects
    if (ImGui::Checkbox("No Death Effect", &noDeathEffect)) {
        Astral::Cosmetics::NoDeathEffect::setEnabled(noDeathEffect);
        saveHackData();
    }
    ImGui::SameLine();
    if (ImGui::Checkbox("No Respawn Flash", &noRespawnFlash)) {
        Astral::Cosmetics::NoRespawnFlash::setEnabled(noRespawnFlash);
        saveHackData();
    }
    ImGui::SameLine();
    if (ImGui::Checkbox("No Ghost Trail", &ghostTrail)) {
        Astral::Cosmetics::NoGhostTrail::setEnabled(ghostTrail);
        saveHackData();
    }
    
    ImGui::Separator();
    
    // Orb Effects
    if (ImGui::Checkbox("No Orb Effects", &noOrbEffectEnabled)) {
        Astral::Cosmetics::NoOrbEffects::setEnabled(noOrbEffectEnabled);
        saveHackData();
    }
    ImGui::SameLine();
    if (ImGui::Button("⚙")) {
        ImGui::OpenPopup("OrbEffectsPopup");
    }
    
    if (ImGui::BeginPopup("OrbEffectsPopup")) {
        if (ImGui::Checkbox("No Orb Ring", &noOrbRing)) {
            Astral::Cosmetics::NoOrbEffects::setNoOrbRing(noOrbRing);
            saveHackData();
        }
        if (ImGui::Checkbox("No Orb Hit Effect", &noOrbHitEffect)) {
            Astral::Cosmetics::NoOrbEffects::setNoOrbHitEffect(noOrbHitEffect);
            saveHackData();
        }
        if (ImGui::Checkbox("No Dash Boom", &noDashBoom)) {
            Astral::Cosmetics::NoOrbEffects::setNoDashBoom(noDashBoom);
            saveHackData();
        }
        ImGui::EndPopup();
    }
    
    ImGui::Separator();
    
    // Dash & Visual Effects
    if (ImGui::Checkbox("No Dash Fire", &noDashFire)) {
        Astral::Cosmetics::NoDashFire::setEnabled(noDashFire);
        saveHackData();
    }
    ImGui::SameLine();
    if (ImGui::Checkbox("No Shaders", &noShaders)) {
        Astral::Cosmetics::NoShaders::setEnabled(noShaders);
        saveHackData();
    }
    
    ImGui::Separator();
    
    // Mirror Options
    if (ImGui::Checkbox("No Mirror", &noMirror)) {
        Astral::Cosmetics::NoMirror::setEnabled(noMirror);
        saveHackData();
    }
    ImGui::SameLine();
    if (ImGui::Checkbox("Instant Mirror", &instantMirror)) {
        Astral::Cosmetics::InstantMirror::setEnabled(instantMirror);
        saveHackData();
    }
    
    ImGui::Separator();
    
    // Wave Options
    if (ImGui::Checkbox("Keep Wave On", &keepWaveEnabled)) {
        Astral::Cosmetics::KeepWave::setEnabled(keepWaveEnabled);
        saveHackData();
    }
    ImGui::SameLine();
    if (ImGui::Button("⚙##wave")) {
        ImGui::OpenPopup("KeepWaveModesPopup");
    }
    ImGui::SameLine();
    if (ImGui::Checkbox("Wave Trail Drag Fix", &waveTrailDragFix)) {
        Astral::Cosmetics::WaveDragFix::setEnabled(waveTrailDragFix);
        saveHackData();
    }
    
    if (ImGui::BeginPopup("KeepWaveModesPopup")) {
        if (ImGui::Checkbox("NoFade", &noFade)) {
            Astral::Cosmetics::KeepWave::setNoFadeOut(noFade);
            saveHackData();
        }
        if (ImGui::Checkbox("NoReset", &noReset)) {
            Astral::Cosmetics::KeepWave::setNoReset(noReset);
            saveHackData();
        }
        ImGui::EndPopup();
    }
    ImGui::SameLine();
    if (ImGui::Checkbox("No Wave Trail", &noTrail)) {
        Astral::Cosmetics::NoTrail::setEnabled(noTrail);
        saveHackData();
    }
    
    ImGui::Separator();
    
    // Quality of Life
    static bool unlockEverythingEnabled = false;
    if (ImGui::Checkbox("Unlock Everything", &unlockEverythingEnabled)) {
        Astral::Cosmetics::UnlockEverything::setEnabled(unlockEverythingEnabled);
        saveHackData();
    }
    
    ImGui::Separator();
    
    if (ImGui::Checkbox("Accurate Percentage", &accuratePercentage)) {
        Astral::QOL::AccuratePercentage::setEnabled(accuratePercentage);
        saveHackData();
    }
    ImGui::SameLine();
    ImGui::Text("Decimals:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(80);
    if (ImGui::InputInt("##DecimalPlaces", &decimalPlaces)) {
        if (decimalPlaces < 0) {
            decimalPlaces = 1;
        } else if (decimalPlaces > 20){
            decimalPlaces = 1;
        }
        saveHackData();
        Astral::QOL::AccuratePercentage::setDecimalPlaces(decimalPlaces);
    }
}

void renderAutoclicker() {
    bool enabled = isEnabled();
    if (ImGui::Checkbox("Autoclicker Enable", &enabled)) {
        setEnabled(enabled);
    }
    
    if (!enabled) return;
    
    ImGui::Separator();
    ImGui::Text("Player 1 Controls:");
    
    // Render each key setting inline
    auto renderKey = [](const char* keyName, char keyCode) {
        if (ImGui::CollapsingHeader(keyName)) {
            ImGui::Indent();
            KeySettings& s = getKeySettings(keyCode);
            ImGui::Checkbox(fmt::format("Enable##{}", keyName).c_str(), &s.enabled);
            if (s.enabled) {
                ImGui::SetNextItemWidth(100);
                ImGui::InputInt(fmt::format("Hold##{}", keyName).c_str(), &s.intervalHold);
                if (s.intervalHold < 0) s.intervalHold = 0;
                ImGui::SameLine(); ImGui::Text("(0=inf)");
                
                ImGui::SetNextItemWidth(100);
                ImGui::InputInt(fmt::format("Release##{}", keyName).c_str(), &s.intervalRelease);
                if (s.intervalRelease < 1) s.intervalRelease = 1;
                
                ImGui::SetNextItemWidth(80);
                ImGui::InputInt(fmt::format("Clicks/F##{}", keyName).c_str(), &s.clicksPerFrame);
                if (s.clicksPerFrame < 1) s.clicksPerFrame = 1;
                
                ImGui::Checkbox(fmt::format("Swift##{}", keyName).c_str(), &s.swiftClick);
                ImGui::SameLine();
                ImGui::Checkbox(fmt::format("Limit##{}", keyName).c_str(), &s.limitFrames);
                if (s.limitFrames) {
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(80);
                    ImGui::InputInt(fmt::format("Max##{}", keyName).c_str(), &s.maxFrames);
                    if (s.maxFrames < 0) s.maxFrames = 0;
                }
                
                if (ImGui::Checkbox(fmt::format("Black Orb##{}", keyName).c_str(), &s.blackOrbModeEnabled)) {}
                if (s.blackOrbModeEnabled) {
                    ImGui::Indent();
                    ImGui::SetNextItemWidth(60);
                    ImGui::InputInt(fmt::format("Clicks##{}", keyName).c_str(), &s.blackOrb_clickCount);
                    if (s.blackOrb_clickCount < 1) s.blackOrb_clickCount = 1;
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(60);
                    ImGui::InputInt(fmt::format("Hold##{}", keyName).c_str(), &s.blackOrb_holdFrames);
                    if (s.blackOrb_holdFrames < 1) s.blackOrb_holdFrames = 1;
                    ImGui::Unindent();
                }
            }
            ImGui::Unindent();
        }
    };
    
    renderKey("W (Jump)", 'W');
    renderKey("A (Left)", 'A');
    renderKey("D (Right)", 'D');
    renderKey("SPACE (Jump Alt)", 'S');
    
    ImGui::Separator();
    ImGui::Text("Player 2 Controls:");
    
    renderKey("UP (Jump)", 'U');
    renderKey("LEFT (Left)", 'L');
    renderKey("RIGHT (Right)", 'R');
    
    ImGui::Separator();
    
    if (ImGui::Button("Disable All", ImVec2(120, 0))) {
        for (char k : {'W', 'A', 'D', 'U', 'L', 'R', 'S'}) {
            getKeySettings(k).enabled = false;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Jump Keys", ImVec2(100, 0))) {
        for (char k : {'W', 'U', 'S'}) {
            getKeySettings(k).enabled = true;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Movement", ImVec2(100, 0))) {
        for (char k : {'A', 'D', 'L', 'R'}) {
            getKeySettings(k).enabled = true;
        }
    }
}
void renderLabelsTab()
{
ImGui::Text("Input Stacking");
    ImGui::Separator();
    
    const char* keys[] = { "W (Jump)", "A (Left)", "S (Jump Alt)", "D (Right)", "SPACE (Jump)", 
                          "UP (Jump)", "DOWN", "LEFT", "RIGHT" };
    
    ImGui::Checkbox("Enable##stacking", &InputStacking::config.enabled);
    
    ImGui::SameLine();
    ImGui::SetNextItemWidth(120);
    ImGui::Combo("Stack Key##stack", &InputStacking::config.stackKey, keys, IM_ARRAYSIZE(keys));
    
    ImGui::SameLine();
    ImGui::SetNextItemWidth(80);
    ImGui::InputInt("Multiplier##stack", &InputStacking::config.multiplier);
    if (InputStacking::config.multiplier < 1) InputStacking::config.multiplier = 1;
    
    ImGui::Text("When you click/hold '%s', it will repeat %dx total", 
                keys[InputStacking::config.stackKey], 
                InputStacking::config.multiplier);
}

void renderRenderTab()
{
    if (ImGui::Button("Start Render", ImVec2(150, 30))) {
        /*
        Astral::Renderer::Recorder::Settings settings;
        settings.width = 1920;
        settings.height = 1080;
        settings.fps = 60;
        settings.bitrate = 30;
        settings.codec = "libx264";
        settings.outputPath = (Astral::RendersDir() / "render.mp4").string();
        
        Astral::Renderer::g_recorder.start(settings);*/
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop Render", ImVec2(150, 30))) {
        // Astral::Renderer::g_recorder.stop();
    }
    ImGui::Spacing();
}

void renderSettingsTab()
{
    ImGui::Text("Toggle GUI Key:");
    
    const char* currentKeyDisplay = getKeyName(capturedCustomKey);
    if (ImGui::Button(isCapturingKeybind ? "Press any key..." : currentKeyDisplay, ImVec2(120, 25))) {
        isCapturingKeybind = true; 
    }
    if (isCapturingKeybind)
    {
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            isCapturingKeybind = false;
        }
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::BeginDisabled();
    //ImGui::Text("Theme: %s", ThemeManager::getCurrentThemeName());
    ImGui::SameLine();
    if (ImGui::Button("Next Theme")) {
        //ThemeManager::nextTheme();
        ImGui::GetIO().Fonts->Build();
        ImGui::GetStyle();  // Force style update
    }
    ImGui::EndDisabled();
    ImGui::Spacing();
    ImGui::Checkbox("Enable Debug Logging", &debugLog);
}

void renderMainGui()
{
    if (!ImGui::GetCurrentContext()) return;
    static bool fontsInitialized = false;
    if (!fontsInitialized) {
        // ThemeManager::initFonts();
        fontsInitialized = true;
    }
    auto& imguiCocos = ImGuiCocos::get();
    if (&imguiCocos == nullptr) return;
    
    guiVisible = imguiCocos.isVisible();
    
    // Handle cursor visibility
    static bool cursorWasHidden = false;
    #ifdef GEODE_IS_WINDOWS
    if (guiVisible) {
        auto view = CCDirector::sharedDirector()->getOpenGLView();
        view->showCursor(true);
        cursorWasHidden = true;
    } else {
        if (cursorWasHidden) {
            // Only hide if not in menu
            auto runningScene = CCDirector::sharedDirector()->getRunningScene();
            if (runningScene && runningScene->getChildByTag(0) != nullptr) {
                CCDirector::sharedDirector()->getOpenGLView()->showCursor(false);
            }
            cursorWasHidden = false;
        }
    }
    #endif
    #ifdef GEODE_IS_MACOS
if (guiVisible) {
    // Show cursor on Mac
    CGDisplayShowCursor(kCGDirectMainDisplay);
    cursorWasHidden = true;
} else {
    if (cursorWasHidden) {
        // Hide cursor on Mac if in a level
        auto runningScene = CCDirector::sharedDirector()->getRunningScene();
        if (runningScene && runningScene->getChildByTag(0) != nullptr) {
            CGDisplayHideCursor(kCGDirectMainDisplay);
        }
        cursorWasHidden = false;
    }
}
#endif
    
    if (tabCount <= 0) return;
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
    if (!ImGui::Begin("Astral [BETA v1.0]", nullptr, flags)) {
        ImGui::End();
        return;
    }
    
    float width = ImGui::GetContentRegionAvail().x;
    const char* title = "Astral [BETA v1.0]";
    ImGui::SetCursorPosX((width - ImGui::CalcTextSize(title).x) * 0.4f);
    ImGui::SetWindowFontScale(1.25f);  // Make it 1.5x bigger
    ImGui::Text("%s", title);
    ImGui::SetWindowFontScale(1.0f);  // Reset to normal
    ImGui::Separator();
    if (!tabNames || tabCount <= 0) {
        ImGui::End();
        return;
    }
    ImGui::SetCursorPosY(50);
    
    for (int i = 0; i < tabCount; i++)
    {
        if (!tabNames[i]) continue;
        if (ImGui::Button(tabNames[i])) currentTab = i;
        if (i < tabCount - 1) ImGui::SameLine();
    }
    ImGui::Separator();  // Add this line here
    
    switch (currentTab) {
        case 0: renderBottingTab(); break;
        case 1: renderHacksTab(); break;
        case 2: renderAutoclicker(); break;
        case 3: renderLabelsTab(); break;
        case 4: renderRenderTab(); break;
        case 5: renderSettingsTab(); break; 
;
    }
    
    ImGui::Separator();
    ImGui::Text("Frame: %.2f", frameCount);
    
    ImGui::End();
}
#endif
#ifdef GEODE_IS_MOBILE
class $modify(MenuLayer) {
    void onMoreGames(CCObject* target) {
        Astral_GUI_Mobile_UI::create()->show();
    }
};
#endif
