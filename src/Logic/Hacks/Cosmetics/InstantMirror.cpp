#include "includes.hpp"

using namespace geode::prelude;

namespace Astral::Cosmetics::InstantMirror {
    static bool instantMirrorEnabled = false;
    
    bool isEnabled() {
        return instantMirrorEnabled;
    }
    
    void setEnabled(bool enabled) {
        instantMirrorEnabled = enabled;
    }
}

class $modify(InstantMirrorGJBGLHook, GJBaseGameLayer) {
    void toggleFlipped(bool p0, bool p1) {
        if (Astral::Cosmetics::InstantMirror::isEnabled()) {
            GJBaseGameLayer::toggleFlipped(p0, true);
        }
        else {
            GJBaseGameLayer::toggleFlipped(p0, p1);
        }
    }
};