#pragma once
#include "includes.hpp"
using namespace geode::prelude;

// Keybind callback - called when a button with a keybind is triggered
using KeybindCallback = std::function<void()>;

// Represents a single keybind
struct Keybind {
    std::string id;           // Unique identifier (e.g., "record_button")
    std::string displayName;  // Human-readable name
    cocos2d::enumKeyCodes keyCode;  // The actual key bound
    KeybindCallback callback; // Function to call when triggered
    bool enabled;
};

class KeybindManager {
private:
    static KeybindManager* instance;
    std::unordered_map<std::string, Keybind> keybinds;
    std::unordered_map<int, std::string> keyToBindId; // For quick lookup
    std::string capturingBindId;
    bool isCapturing;
    
    KeybindManager();
    
public:
    static KeybindManager* get();
    
    // Register a new keybind
    void registerKeybind(const std::string& id, const std::string& displayName, KeybindCallback callback, cocos2d::enumKeyCodes defaultKey = cocos2d::enumKeyCodes::KEY_None);
    
    // Get a keybind by ID
    Keybind* getKeybind(const std::string& id);
    
    // Get all keybinds
    const std::unordered_map<std::string, Keybind>& getAllKeybinds() const;
    
    // Update a keybind's key
    void setKeybindKey(const std::string& id, cocos2d::enumKeyCodes keyCode);
    
    // Start capturing input for a keybind
    void startCapturing(const std::string& bindId);
    
    // Stop capturing input
    void stopCapturing();
    
    // Check if currently capturing
    bool isCapturingInput() const { return isCapturing; }
    
    // Get the ID of the bind being captured
    const std::string& getCapturingBindId() const { return capturingBindId; }
    
    // Handle key input (call this in your input handler)
    bool handleKeyInput(cocos2d::enumKeyCodes keyCode);
    
    // Get human-readable key name from key code
    static std::string getKeyName(cocos2d::enumKeyCodes keyCode);
    
    // Save/Load keybinds using Geode's saved data
    void saveKeybinds();
    void loadKeybinds();
    
    // Toggle a keybind on/off
    void toggleKeybind(const std::string& id);
};