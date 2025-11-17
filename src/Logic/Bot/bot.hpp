#include "vars.h"

namespace Astral::Bot::Recording {
    void StartRecording(const std::string& macroName);
    void StopRecording();
    void RecordFrame(const std::string& keysPressed, const std::string& keysHeld, int clickMultiplier = 1);
    void RecordDeath(const std::string& reason = "");
    void RecordTPSChange(float newTPS);
    void RecordSeedChange(int newSeed);
    bool SaveMacro();
    bool LoadMacro(const std::string& macroName);
    void Record();
    void Playback();
    void PlaybackUpdate(); // Called each frame during playback
    void StopPlayback();
    void Save();
    void SetKeyPressed(int keyIndex, bool pressed);
    void ProcessRecordingFrame(); // Add this line!
    
    // Getters for GUI
    bool IsRecording();
    bool IsPlaying();
}