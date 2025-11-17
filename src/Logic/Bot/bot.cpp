#include <fstream>
#include <chrono>
#include <sstream>
#include "includes.hpp"
#include <Geode/modify/CCKeyboardDispatcher.hpp>
extern float frameCount;

namespace Astral::Bot::Recording {
    
    // Forward declarations
    void SimulateKeyPress(cocos2d::enumKeyCodes keyCode);
    void SimulateKeyHold(cocos2d::enumKeyCodes keyCode);
    void SimulateKeyRelease(cocos2d::enumKeyCodes keyCode);
    
    struct MacroFrame {
        int frame;
        std::string keys;
        std::string holdKeys;
        int clickMultiplier = 1;
    };
    
    struct MacroHeader {
        std::string macroName;
        std::string author = "User";
        float tpsStart = 240.0f;
        int seed = 1;
        float gameVersion = 2.207f;
        bool recordDeaths = false;
        bool isTwoPlayer = false;
        bool isPlatformer = false;
        bool autoRecordInputs = false;
        bool recordCustomInputSelectionEnabled = true;
        std::string customInputSelection = "111000";
        int macroTypeVersion = 1;
    };
    
    static MacroHeader currentHeader;
    static std::vector<MacroFrame> recordedFrames;
    static std::vector<int> tpsChanges;
    static std::vector<int> seedChanges;
    static std::vector<std::pair<int, std::string>> deaths;
    static int currentFrame = 0;
    static bool isRecording = false;
    static bool isPlaying = false;
    static int playbackFrame = 0;
    static int playbackIndex = 0;
    
    void StartRecording(const std::string& macroName) {
        currentHeader.macroName = macroName;
        
        // Set TPS from current game TPS
        currentHeader.tpsStart = Astral::Hacks::TPSBypass::getTPS();
        
        // Try to load existing macro to append to it
        std::filesystem::path macroPath = MacrosDir() / (macroName + ".ast");
        if (std::filesystem::exists(macroPath)) {
            // Load existing macro to append
            if (LoadMacro(macroName)) {
                // Get the frame count from existing frames
                if (!recordedFrames.empty()) {
                    currentFrame = recordedFrames.back().frame + 1;
                }
                AstralLogger::log("Appending to existing macro: {} at TPS: {}", macroName, currentHeader.tpsStart);
            }
        } else {
            // File doesn't exist, start fresh
            recordedFrames.clear();
            tpsChanges.clear();
            seedChanges.clear();
            deaths.clear();
            currentFrame = 0;
            AstralLogger::log("Started recording new macro: {} at TPS: {}", macroName, currentHeader.tpsStart);
        }
        
        isRecording = true;
    }
    
    void RecordFrame(const std::string& keysPressed, const std::string& keysHeld, int clickMultiplier) {
        if (!isRecording) return;
        
        // Only record if there's actual input
        if (keysPressed == "000000" && keysHeld == "000000") {
            currentFrame++;
            return;
        }
        
        AstralLogger::log("Recording frame {} - Keys: {} Hold: {} Multiplier: {}", 
                         currentFrame, keysPressed, keysHeld, clickMultiplier);
        
        MacroFrame frame;
        frame.frame = currentFrame;
        frame.keys = keysPressed;
        frame.holdKeys = keysHeld;
        frame.clickMultiplier = clickMultiplier;
        
        recordedFrames.push_back(frame);
        currentFrame++;
    }
    
    void RecordDeath(const std::string& reason) {
        if (!isRecording) return;
        deaths.push_back({currentFrame, reason});
        AstralLogger::log("Death recorded at frame: {}", currentFrame);
    }
    
    void RecordTPSChange(float newTPS) {
        if (!isRecording) return;
        tpsChanges.push_back(static_cast<int>(newTPS));
    }
    
    void RecordSeedChange(int newSeed) {
        if (!isRecording) return;
        seedChanges.push_back(newSeed);
    }
    
    void StopRecording() {
        isRecording = false;
        AstralLogger::log("Stopped recording macro: {}", currentHeader.macroName);
    }
    
    bool SaveMacro() {
        if (currentHeader.macroName.empty()) {
            AstralLogger::log("Cannot save macro: name is empty");
            return false;
        }
        
        std::filesystem::path macroPath = MacrosDir() / (currentHeader.macroName + ".ast");
        
        try {
            std::ofstream file(macroPath);
            if (!file.is_open()) {
                AstralLogger::log("Failed to create macro file: {}", macroPath.string());
                return false;
            }
            
            // Write header
            file << currentHeader.macroName << "\n";
            file << currentHeader.author << "\n";
            file << currentHeader.tpsStart << "\n";
            file << currentHeader.seed << "\n";
            file << currentHeader.gameVersion << "\n";
            file << (currentHeader.recordDeaths ? 1 : 0) << "\n";
            file << (currentHeader.isTwoPlayer ? 1 : 0) << "\n";
            file << (currentHeader.isPlatformer ? 1 : 0) << "\n";
            file << (currentHeader.autoRecordInputs ? 1 : 0) << "\n";
            file << (currentHeader.recordCustomInputSelectionEnabled ? 1 : 0) << "\n";
            file << currentHeader.customInputSelection << "\n";
            file << currentHeader.macroTypeVersion << "\n";
            
            // Write ALL frames including "000000" ones
            for (const auto& frame : recordedFrames) {
                file << frame.frame << " " << frame.keys << " " << frame.holdKeys;
                if (frame.clickMultiplier > 1) {
                    file << " " << frame.clickMultiplier;
                }
                file << "\n";
                AstralLogger::log("Saving frame {} - Keys: {} Hold: {}", frame.frame, frame.keys, frame.holdKeys);
            }
            
            // Write TPS changes
            if (!tpsChanges.empty()) {
                file << currentFrame << " Tps:" << tpsChanges[0] << "\n";
            }
            
            // Write seed changes
            if (!seedChanges.empty()) {
                file << currentFrame << " Seed:" << seedChanges[0] << "\n";
            }
            
            // Write deaths
            for (const auto& death : deaths) {
                file << death.first << " Death";
                if (!death.second.empty()) {
                    file << " " << death.second;
                }
                file << "\n";
            }
            
            file.close();
            AstralLogger::log("Successfully saved macro: {} ({} frames)", currentHeader.macroName, recordedFrames.size());
            return true;
        } catch (const std::exception& e) {
            AstralLogger::log("Error saving macro: {}", e.what());
            return false;
        }
    }
    
    bool LoadMacro(const std::string& macroName) {
        std::filesystem::path macroPath = MacrosDir() / (macroName + ".ast");
        
        try {
            std::ifstream file(macroPath);
            if (!file.is_open()) {
                AstralLogger::log("Failed to open macro file: {}", macroPath.string());
                return false;
            }
            
            recordedFrames.clear();
            tpsChanges.clear();
            seedChanges.clear();
            deaths.clear();
            
            // Read header
            std::getline(file, currentHeader.macroName);
            std::getline(file, currentHeader.author);
            
            std::string line;
            std::getline(file, line);
            currentHeader.tpsStart = std::stof(line);
            
            std::getline(file, line);
            currentHeader.seed = std::stoi(line);
            
            std::getline(file, line);
            currentHeader.gameVersion = std::stof(line);
            
            std::getline(file, line);
            currentHeader.recordDeaths = (std::stoi(line) == 1);
            
            std::getline(file, line);
            currentHeader.isTwoPlayer = (std::stoi(line) == 1);
            
            std::getline(file, line);
            currentHeader.isPlatformer = (std::stoi(line) == 1);
            
            std::getline(file, line);
            currentHeader.autoRecordInputs = (std::stoi(line) == 1);
            
            std::getline(file, line);
            currentHeader.recordCustomInputSelectionEnabled = (std::stoi(line) == 1);
            
            std::getline(file, line);
            currentHeader.customInputSelection = line;
            
            std::getline(file, line);
            currentHeader.macroTypeVersion = std::stoi(line);
            
            // Read frames and events
            while (std::getline(file, line)) {
                if (line.empty()) continue;
                
                std::istringstream iss(line);
                int frameNum;
                iss >> frameNum;
                
                std::string token;
                iss >> token;
                
                if (token.find("Tps:") == 0) {
                    int tps = std::stoi(token.substr(4));
                    tpsChanges.push_back(tps);
                } else if (token.find("Seed:") == 0) {
                    int seed = std::stoi(token.substr(5));
                    seedChanges.push_back(seed);
                } else if (token == "Death") {
                    std::string reason;
                    std::getline(iss, reason);
                    reason = reason.empty() ? "" : reason.substr(1);
                    deaths.push_back({frameNum, reason});
                } else {
                    // It's a frame with input
                    std::string holdKeys;
                    int clickMult = 1;
                    iss >> holdKeys;
                    
                    if (iss >> clickMult) {
                        // clickMultiplier was provided
                    } else {
                        clickMult = 1;
                    }
                    
                    MacroFrame frame;
                    frame.frame = frameNum;
                    frame.keys = token;
                    frame.holdKeys = holdKeys;
                    frame.clickMultiplier = clickMult;
                    
                    recordedFrames.push_back(frame);
                }
            }
            
            file.close();
            AstralLogger::log("Successfully loaded macro: {} ({} frames)", macroName, recordedFrames.size());
            return true;
        } catch (const std::exception& e) {
            AstralLogger::log("Error loading macro: {}", e.what());
            return false;
        }
    }
    
    // Helper functions for input simulation
    void SimulateKeyPress(cocos2d::enumKeyCodes keyCode) {
        CCKeyboardDispatcher* dispatcher = CCDirector::sharedDirector()->getKeyboardDispatcher();
        if (dispatcher) {
            dispatcher->dispatchKeyboardMSG(keyCode, true, false);
            dispatcher->dispatchKeyboardMSG(keyCode, false, false);
        }
    }
    
    void SimulateKeyHold(cocos2d::enumKeyCodes keyCode) {
        CCKeyboardDispatcher* dispatcher = CCDirector::sharedDirector()->getKeyboardDispatcher();
        if (dispatcher) {
            dispatcher->dispatchKeyboardMSG(keyCode, true, false);
        }
    }
    
    void SimulateKeyRelease(cocos2d::enumKeyCodes keyCode) {
        CCKeyboardDispatcher* dispatcher = CCDirector::sharedDirector()->getKeyboardDispatcher();
        if (dispatcher) {
            dispatcher->dispatchKeyboardMSG(keyCode, false, false);
        }
    }
    
    void SimulateInput(const std::string& keysPressed, const std::string& keysHeld, int clickMultiplier) {
        // Map bit positions to cocos2d key codes
        // Position: 0=W, 1=A, 2=D, 3=Up, 4=Left, 5=Right
        const cocos2d::enumKeyCodes keyMap[] = {
            cocos2d::KEY_W, cocos2d::KEY_A, cocos2d::KEY_D, 
            cocos2d::KEY_Up, cocos2d::KEY_Left, cocos2d::KEY_Right
        };
        const char* keyNames[] = {"W", "A", "D", "UP", "LEFT", "RIGHT"};
        
        // Process pressed keys (these execute once per click multiplier)
        for (int click = 0; click < clickMultiplier; ++click) {
            for (size_t i = 0; i < keysPressed.length() && i < 6; ++i) {
                if (keysPressed[i] == '1') {
                    SimulateKeyPress(keyMap[i]);
                }
            }
        }
        
        // Process held keys (these stay pressed until next frame with different hold pattern)
        for (size_t i = 0; i < keysHeld.length() && i < 6; ++i) {
            if (keysHeld[i] == '1') {
                SimulateKeyHold(keyMap[i]);
            } else {
                SimulateKeyRelease(keyMap[i]);
            }
        }
        
        // Log input for debugging
        std::string pressedStr, heldStr;
        for (size_t i = 0; i < keysPressed.length() && i < 6; ++i) {
            if (keysPressed[i] == '1') pressedStr += std::string(keyNames[i]) + " ";
        }
        for (size_t i = 0; i < keysHeld.length() && i < 6; ++i) {
            if (keysHeld[i] == '1') heldStr += std::string(keyNames[i]) + " ";
        }
        if (!pressedStr.empty() || !heldStr.empty()) {
            AstralLogger::log("Input - Press: [{}] Hold: [{}] x{}", pressedStr, heldStr, clickMultiplier);
        }
    }
    
    void Playback() {
        if (recordedFrames.empty()) return;
        
        playbackIndex = 0;
        AstralLogger::log("Started playback: {} with {} frames", currentHeader.macroName, recordedFrames.size());
        
        // Log first few frames for debugging
        for (size_t i = 0; i < std::min(size_t(5), recordedFrames.size()); ++i) {
            AstralLogger::log("Frame {} - Keys: {} Hold: {}", 
                             recordedFrames[i].frame, recordedFrames[i].keys, recordedFrames[i].holdKeys);
        }
    }
    
    void PlaybackUpdate() {
        if (!isPlaying || recordedFrames.empty()) return;
        
        static int lastLoggedFrame = -1;
        
        // Log every 50 frames to avoid spam
        if ((int)frameCount % 50 == 0 && (int)frameCount != lastLoggedFrame) {
            AstralLogger::log("Playback - Game frame: {} / looking for frames starting at {}", 
                             (int)frameCount, recordedFrames[0].frame);
            lastLoggedFrame = (int)frameCount;
        }
        
        // Check if current game frame matches any recorded frames
        for (size_t i = playbackIndex; i < recordedFrames.size(); ++i) {
            const MacroFrame& frame = recordedFrames[i];
            
            if (frame.frame == (int)frameCount) {
                AstralLogger::log("Playback executing frame {} - Keys: {} Hold: {}", 
                                 frame.frame, frame.keys, frame.holdKeys);
                SimulateInput(frame.keys, frame.holdKeys, frame.clickMultiplier);
                playbackIndex = i + 1;
            } else if (frame.frame > (int)frameCount) {
                break;
            }
        }
        
        // Check for deaths
        for (const auto& death : deaths) {
            if (death.first == (int)frameCount) {
                AstralLogger::log("Macro death detected at frame: {}", (int)frameCount);
                StopPlayback();
                return;
            }
        }
    }
    
    void StopPlayback() {
        isPlaying = false;
        playbackFrame = 0;
        playbackIndex = 0;
        AstralLogger::log("Stopped playback: {}", currentHeader.macroName);
    }
    
    // Track currently pressed keys
    static bool keysPressed[6] = {false}; // W, A, D, Up, Left, Right
    static std::string lastRecordedInput = "";
    
    void SetKeyPressed(int keyIndex, bool pressed) {
        if (keyIndex >= 0 && keyIndex < 6) {
            keysPressed[keyIndex] = pressed;
        }
    }
    
    void UpdateRecording(const std::string& keysPressed, const std::string& keysHeld, int clickMultiplier) {
        if (!isRecording) return;
        
        // Only record if input changed
        if (keysPressed != lastRecordedInput) {
            AstralLogger::log("Input changed: {} -> {} (was: {})", 
                             lastRecordedInput, keysPressed, "updating");
            RecordFrame(keysPressed, keysHeld, clickMultiplier);
            lastRecordedInput = keysPressed;
        }
    }
    
    void ProcessRecordingFrame() {
        if (!isRecording) return;
        
        // Build key strings from current key states: W, A, D, Up, Left, Right
        std::string keysPressed = "";
        std::string keysHeld = "";
        
        for (int i = 0; i < 6; ++i) {
            keysPressed += ::Astral::Bot::Recording::keysPressed[i] ? '1' : '0';
            keysHeld += ::Astral::Bot::Recording::keysPressed[i] ? '1' : '0';
        }
        
        // Record every frame change, even if no input
        if (keysPressed != lastRecordedInput) {
            AstralLogger::log("Recording frame {} - Keys: {} Hold: {}", currentFrame, keysPressed, keysHeld);
            RecordFrame(keysPressed, keysHeld, 1);
            lastRecordedInput = keysPressed;
        }
        
        currentFrame++;
    }
    
    void Save() {
        SaveMacro();
    }
    
    bool IsRecording() {
        return isRecording;
    }
    
    bool IsPlaying() {
        return isPlaying;
    }
}

// Keyboard hook to capture input for recording
#include <Geode/modify/CCKeyboardDispatcher.hpp>

class $modify(BotKeyboardDispatcher, CCKeyboardDispatcher) {
    bool dispatchKeyboardMSG(cocos2d::enumKeyCodes key, bool isKeyDown, bool isKeyRepeat) {
        // Debug: log every key
        AstralLogger::log("Keyboard event: key={} down={} repeat={}", (int)key, isKeyDown, isKeyRepeat);
        
        // Map keys to indices: 0=W, 1=A, 2=D, 3=Up, 4=Left, 5=Right
        int keyIndex = -1;
        const char* keyName = "UNKNOWN";
        if (key == cocos2d::KEY_W) { keyIndex = 0; keyName = "W"; }
        else if (key == cocos2d::KEY_A) { keyIndex = 1; keyName = "A"; }
        else if (key == cocos2d::KEY_D) { keyIndex = 2; keyName = "D"; }
        else if (key == cocos2d::KEY_Up) { keyIndex = 3; keyName = "UP"; }
        else if (key == cocos2d::KEY_Left) { keyIndex = 4; keyName = "LEFT"; }
        else if (key == cocos2d::KEY_Right) { keyIndex = 5; keyName = "RIGHT"; }
        
        if (keyIndex >= 0) {
            AstralLogger::log("Matched key: {}", keyName);
            Astral::Bot::Recording::SetKeyPressed(keyIndex, isKeyDown);
        }
        
        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, isKeyDown, isKeyRepeat);
    }
};
#include <Geode/modify/CCScheduler.hpp>

class $modify(BotScheduler, CCScheduler) {
    void update(float dt) {
        CCScheduler::update(dt);
        
        // Update recording each frame
        Astral::Bot::Recording::ProcessRecordingFrame();
        
        // Update playback each frame
        if (Astral::Bot::Recording::IsPlaying()) {
            Astral::Bot::Recording::PlaybackUpdate();
        }
    }
};