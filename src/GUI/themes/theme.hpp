#pragma once
#include <imgui.h>
#include <Geode/Geode.hpp>

enum class Theme {
    DARK,
    ASTRAL_VOID,
    ASTRAL_BLUR,
    COUNT
};

class ThemeManager {
    public:
    static inline void initFonts() {
        auto& io = ImGui::GetIO();
        if (io.Fonts->Fonts.size() <= 1) {  // Only load if not already loaded
            io.Fonts->AddFontFromFileTTF(
                (geode::Mod::get()->getResourcesDir() / ("font" + std::to_string(0) + ".ttf")).string().c_str(), 16.0f
            );
            io.Fonts->Build();
        }
    }
    
    static inline void applyTheme(Theme theme) {
        
        switch (theme) {
            case Theme::DARK:
                applyDarkTheme();
                break;
            case Theme::ASTRAL_VOID:
                applyAstralVoidTheme();
                break;
            case Theme::ASTRAL_BLUR:
                applyAstralBlurTheme();
                break;
            default:
                applyAstralBlurTheme();
        }
    }
    
    static inline void nextTheme() {
        int next = (static_cast<int>(currentTheme) + 1) % static_cast<int>(Theme::COUNT);
        applyTheme(static_cast<Theme>(next));
    }
    
    static inline Theme getCurrentTheme() { return currentTheme; }
    
    static inline const char* getCurrentThemeName() {
        return getThemeName(currentTheme);
    }
    
    static inline const char* getThemeName(Theme theme) {
        switch (theme) {
            case Theme::DARK: return "Dark";
            case Theme::ASTRAL_VOID: return "Astral Void";
            case Theme::ASTRAL_BLUR: return "Astral Blur";
            default: return "Unknown";
        }
    }
    
    private:
    static Theme currentTheme;
    
    static inline void applyDarkTheme() {
        auto& style = ImGui::GetStyle();
        
        style.Alpha = 0.99f;
        style.WindowRounding = 12.0f;
        style.ChildRounding = 8.0f;
        style.FrameRounding = 8.0f;
        style.PopupRounding = 8.0f;
        style.ScrollbarRounding = 12.0f;
        style.GrabRounding = 8.0f;
        style.TabRounding = 6.0f;
        style.WindowPadding = ImVec2(15, 15);
        style.FramePadding = ImVec2(10, 6);
        style.ItemSpacing = ImVec2(12, 8);
        style.AntiAliasedLines = true;
        style.AntiAliasedFill = true;
        
        currentTheme = Theme::DARK;
    }
    
    static inline void applyAstralVoidTheme() {
        auto& style = ImGui::GetStyle();
        
        style.Alpha = 0.95f;
        style.WindowRounding = 14.0f;
        style.ChildRounding = 10.0f;
        style.FrameRounding = 10.0f;
        style.PopupRounding = 10.0f;
        style.GrabRounding = 10.0f;
        style.TabRounding = 8.0f;
        
        style.WindowPadding = ImVec2(18, 18);
        style.FramePadding = ImVec2(12, 8);
        style.ItemSpacing = ImVec2(10, 10);
        style.ItemInnerSpacing = ImVec2(8, 6);
        
        style.WindowBorderSize = 1.0f;
        style.FrameBorderSize = 0.0f;
        style.PopupBorderSize = 0.0f;
        
        style.AntiAliasedLines = true;
        style.AntiAliasedFill = true;
        
        ImVec4* colors = style.Colors;
        
        const ImVec4 CYAN_NEBULA = ImVec4(0.20f, 0.80f, 1.00f, 1.00f);
        const ImVec4 DARK_CYAN   = ImVec4(0.12f, 0.20f, 0.40f, 0.60f);
        
        colors[ImGuiCol_WindowBg]         = ImVec4(0.04f, 0.04f, 0.12f, 0.94f);
        colors[ImGuiCol_ChildBg]          = ImVec4(0.06f, 0.06f, 0.18f, 0.94f);
        colors[ImGuiCol_PopupBg]          = ImVec4(0.06f, 0.06f, 0.18f, 0.98f);
        
        colors[ImGuiCol_Text]             = ImVec4(0.90f, 0.90f, 0.95f, 1.00f);
        colors[ImGuiCol_Border]           = ImVec4(0.20f, 0.70f, 0.90f, 0.50f);
        colors[ImGuiCol_Separator]        = ImVec4(0.25f, 0.25f, 0.45f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]      = ImVec4(0.10f, 0.10f, 0.20f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab]    = ImVec4(0.30f, 0.30f, 0.50f, 0.70f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.60f, 0.90f);
        colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.50f, 0.50f, 0.70f, 1.00f);
        
        colors[ImGuiCol_FrameBg]          = ImVec4(0.12f, 0.12f, 0.30f, 0.80f);
        colors[ImGuiCol_FrameBgHovered]   = ImVec4(0.16f, 0.22f, 0.45f, 0.90f);
        colors[ImGuiCol_FrameBgActive]    = ImVec4(0.10f, 0.15f, 0.35f, 1.00f);
        
        colors[ImGuiCol_Button]           = ImVec4(0.15f, 0.15f, 0.35f, 1.00f);
        colors[ImGuiCol_ButtonHovered]    = ImVec4(0.20f, 0.30f, 0.55f, 1.00f);
        colors[ImGuiCol_ButtonActive]     = ImVec4(0.10f, 0.25f, 0.50f, 1.00f);
        
        colors[ImGuiCol_Header]           = DARK_CYAN;
        colors[ImGuiCol_HeaderHovered]    = ImVec4(0.25f, 0.35f, 0.70f, 1.00f);
        colors[ImGuiCol_HeaderActive]     = ImVec4(0.20f, 0.30f, 0.60f, 1.00f);
        
        colors[ImGuiCol_Tab]              = ImVec4(0.15f, 0.15f, 0.35f, 1.00f);
        colors[ImGuiCol_TabHovered]       = ImVec4(0.20f, 0.30f, 0.55f, 1.00f);
        colors[ImGuiCol_TabActive]        = ImVec4(0.20f, 0.50f, 0.80f, 1.00f);
        colors[ImGuiCol_TabUnfocused]     = ImVec4(0.15f, 0.15f, 0.35f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive]= ImVec4(0.20f, 0.50f, 0.80f, 1.00f);
        
        colors[ImGuiCol_CheckMark]        = CYAN_NEBULA;
        colors[ImGuiCol_SliderGrab]       = ImVec4(0.30f, 0.50f, 0.85f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = CYAN_NEBULA;
        colors[ImGuiCol_TextSelectedBg]   = ImVec4(0.20f, 0.70f, 0.90f, 0.35f);
        
        currentTheme = Theme::ASTRAL_VOID;
    }
    
    static inline void applyAstralBlurTheme() {
        auto& style = ImGui::GetStyle();
        
        style.Alpha = 0.95f;
        style.WindowRounding = 16.0f;
        style.ChildRounding = 12.0f;
        style.FrameRounding = 10.0f;
        style.PopupRounding = 14.0f;
        style.ScrollbarRounding = 12.0f;
        style.GrabRounding = 10.0f;
        style.TabRounding = 10.0f;
        style.WindowPadding = ImVec2(18, 18);
        style.FramePadding = ImVec2(14, 8);
        style.ItemSpacing = ImVec2(14, 10);
        style.AntiAliasedLines = true;
        style.AntiAliasedFill = true;
        
        ImVec4 bgColor = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
        ImVec4 buttonColor = ImVec4(0.20f, 0.25f, 0.55f, 0.85f);
        ImVec4 buttonHoveredColor = ImVec4(0.40f, 0.55f, 1.00f, 1.0f);
        ImVec4 buttonActiveColor = ImVec4(0.60f, 0.75f, 1.0f, 1.0f);
        
        style.Colors[ImGuiCol_WindowBg]     = bgColor;
        style.Colors[ImGuiCol_ChildBg]      = ImVec4(0.10f, 0.10f, 0.20f, 0.80f);
        style.Colors[ImGuiCol_Border]      = ImVec4(0.3f, 0.5f, 1.0f, 0.25f);
        style.Colors[ImGuiCol_Button]      = buttonColor;
        style.Colors[ImGuiCol_ButtonHovered]   = buttonHoveredColor;
        style.Colors[ImGuiCol_ButtonActive]   = buttonActiveColor;
        
        style.Colors[ImGuiCol_Header]      = ImVec4(0.35f, 0.45f, 1.00f, 0.75f);
        style.Colors[ImGuiCol_HeaderHovered]   = ImVec4(0.55f, 0.65f, 1.00f, 0.90f);
        style.Colors[ImGuiCol_HeaderActive]   = ImVec4(0.65f, 0.75f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_Text]       = ImVec4(0.85f, 0.90f, 1.00f, 1.0f);
        
        style.Colors[ImGuiCol_FrameBg]      = ImVec4(0.18f, 0.20f, 0.30f, 0.85f);
        style.Colors[ImGuiCol_FrameBgHovered]  = ImVec4(0.30f, 0.40f, 0.80f, 1.00f);
        style.Colors[ImGuiCol_FrameBgActive]   = ImVec4(0.45f, 0.55f, 1.00f, 1.00f);
        
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.85f);
        
        currentTheme = Theme::ASTRAL_BLUR;
    }
};

inline Theme ThemeManager::currentTheme = Theme::DARK