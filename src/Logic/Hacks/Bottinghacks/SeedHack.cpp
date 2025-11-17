#include "includes.hpp"
using namespace geode::prelude;
namespace Astral::Hacks::Seedhack {
    static bool seedhackEnabled = false;
    static int seedValue = 0;
    
    bool isEnabled() {
        return seedhackEnabled;
    }
    
    void setEnabled(bool enabled) {
        seedhackEnabled = enabled;
    }
    
    int getSeedValue() {
        return seedValue;
    }
    
    void setSeedValue(int seed) {
        seedValue = seed;
    }
    void setSeedhackEnabled(bool enabled) {
        seedhackEnabled = enabled;
    }
}
class $modify(SeedhackPlayLayer, PlayLayer) {
    void setupHasCompleted() {
        PlayLayer::setupHasCompleted();
        if (Astral::Hacks::Seedhack::isEnabled()) {
            GameToolbox::fast_srand(Astral::Hacks::Seedhack::getSeedValue());
            log::info("Seed set to {} on PlayLayer setup complete", seedValue);
        }
    }
    void resetLevel() {
        PlayLayer::resetLevel();
        if (Astral::Hacks::Seedhack::isEnabled()) {
            GameToolbox::fast_srand(Astral::Hacks::Seedhack::getSeedValue());
            log::info("Seed set to {} on level reset", seedValue);
        }
    }
    
    void loadFromCheckpoint(CheckpointObject* checkpoint) {
        PlayLayer::loadFromCheckpoint(checkpoint);
        if (Astral::Hacks::Seedhack::isEnabled()) {
            GameToolbox::fast_srand(Astral::Hacks::Seedhack::getSeedValue());
            log::info("Seed set to {} on checkpoint load", seedValue);
        }
    }
};