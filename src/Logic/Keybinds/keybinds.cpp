#include "keybinds.hpp"

KeybindManager* KeybindManager::instance = nullptr;

KeybindManager::KeybindManager() : isCapturing(false) {}

KeybindManager* KeybindManager::get() {
    if (!instance) {
        instance = new KeybindManager();
    }
    return instance;
}

void KeybindManager::registerKeybind(const std::string& id, const std::string& displayName, KeybindCallback callback, cocos2d::enumKeyCodes defaultKey) {
    Keybind bind;
    bind.id = id;
    bind.displayName = displayName;
    bind.keyCode = defaultKey;
    bind.callback = callback;
    bind.enabled = true;
    
    keybinds[id] = bind;
    if (defaultKey != cocos2d::enumKeyCodes::KEY_None) {
        keyToBindId[static_cast<int>(defaultKey)] = id;
    }
}

Keybind* KeybindManager::getKeybind(const std::string& id) {
    auto it = keybinds.find(id);
    if (it != keybinds.end()) {
        return &it->second;
    }
    return nullptr;
}

const std::unordered_map<std::string, Keybind>& KeybindManager::getAllKeybinds() const {
    return keybinds;
}

void KeybindManager::setKeybindKey(const std::string& id, cocos2d::enumKeyCodes keyCode) {
    auto bind = getKeybind(id);
    if (!bind) return;
    
    // Remove old mapping
    auto oldKey = bind->keyCode;
    if (oldKey != cocos2d::enumKeyCodes::KEY_None) {
        keyToBindId.erase(static_cast<int>(oldKey));
    }
    
    // Check if this key is already bound to something else
    auto existing = keyToBindId.find(static_cast<int>(keyCode));
    if (existing != keyToBindId.end()) {
        auto oldBind = getKeybind(existing->second);
        if (oldBind) {
            oldBind->keyCode = cocos2d::enumKeyCodes::KEY_None;
        }
    }
    
    // Set new mapping
    bind->keyCode = keyCode;
    keyToBindId[static_cast<int>(keyCode)] = id;
    
    AstralLogger::log("Keybind '{}' set to key {}", id, static_cast<int>(keyCode));
    saveKeybinds();
}

void KeybindManager::startCapturing(const std::string& bindId) {
    if (getKeybind(bindId)) {
        isCapturing = true;
        capturingBindId = bindId;
    }
}

void KeybindManager::stopCapturing() {
    isCapturing = false;
    capturingBindId = "";
}

bool KeybindManager::handleKeyInput(cocos2d::enumKeyCodes keyCode) {
    if (isCapturing) {
        setKeybindKey(capturingBindId, keyCode);
        stopCapturing();
        return true;
    }
    
    // Check if this key is bound to something
    auto it = keyToBindId.find(static_cast<int>(keyCode));
    if (it != keyToBindId.end()) {
        auto bind = getKeybind(it->second);
        if (bind && bind->enabled && bind->callback) {
            bind->callback();
            return true;
        }
    }
    
    return false;
}

std::string KeybindManager::getKeyName(cocos2d::enumKeyCodes keyCode) {
    // Use the same function from your code
    extern const char* getKeyName(cocos2d::enumKeyCodes keyCode);
    return std::string(getKeyName(keyCode));
}

void KeybindManager::saveKeybinds() {
    try {
        std::unordered_map<std::string, int> keybindData;
        for (const auto& [id, bind] : keybinds) {
            keybindData[id] = static_cast<int>(bind.keyCode);
        }
        Mod::get()->setSavedValue("keybinds", keybindData);
        AstralLogger::log("Keybinds saved");
    } catch (const std::exception& e) {
        AstralLogger::log("Failed to save keybinds: {}", e.what());
    }
}

void KeybindManager::loadKeybinds() {
    try {
        auto keybindData = Mod::get()->getSavedValue<std::unordered_map<std::string, int>>("keybinds");
        
        for (const auto& [id, keyCode] : keybindData) {
            auto bind = getKeybind(id);
            if (bind) {
                bind->keyCode = static_cast<cocos2d::enumKeyCodes>(keyCode);
                if (keyCode != static_cast<int>(cocos2d::enumKeyCodes::KEY_None)) {
                    keyToBindId[keyCode] = id;
                }
            }
        }
        AstralLogger::log("Keybinds loaded");
    } catch (const std::exception& e) {
        AstralLogger::log("No saved keybinds found, using defaults");
    }
}

void KeybindManager::toggleKeybind(const std::string& id) {
    auto bind = getKeybind(id);
    if (bind) {
        bind->enabled = !bind->enabled;
    }
}