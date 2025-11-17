#include "includes.hpp"
using namespace geode::prelude;


namespace Astral::QOL::AutoSafeMode {
    static bool autoSafeMode = false;
    static bool forceSafeMode = false;
    bool isEnabled() {
        return autoSafeMode;
    }
    
    void setEnabled(bool enabled) {
        autoSafeMode = enabled;
        if (!enabled) {
            forceSafeMode = false;
        }
    }
}
void updateAutoSafe() {
    if (Astral::QOL::AutoSafeMode::isEnabled()) {
        bool shouldBeSafe = Astral::Hacks::Noclip::isNoclipEnabled() ||
        Astral::Hacks::Speedhack::isSpeedhackEnabled() ||
        Astral::Hacks::AutoClicker::isEnabled();
        
        if (shouldBeSafe) {
            // forceSafeMode = true;
            Astral::QOL::AutoSafeMode::isEnabled();
        }
    }
}