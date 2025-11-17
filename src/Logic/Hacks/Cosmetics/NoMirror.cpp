#include "includes.hpp"
using namespace geode::prelude;

namespace Astral::Cosmetics::NoMirror {
    static bool noMirror = false;
    
    bool isEnabled() {
        return noMirror;
    }
    
    void setEnabled(bool enabled) {
        noMirror = enabled;
    }
}

class $modify(NoMirrorHook, GJBaseGameLayer) {
    void toggleFlipped(bool p0, bool p1) {
        if (Astral::Cosmetics::NoMirror::isEnabled()) {
            return;
        }
        else {
            GJBaseGameLayer::toggleFlipped(p0, p1);
        }
    }
};