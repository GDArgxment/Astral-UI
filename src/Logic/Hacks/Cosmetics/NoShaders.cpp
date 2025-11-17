#include "includes.hpp"
using namespace geode::prelude;

namespace Astral::Cosmetics::NoShaders {
    static bool noShadersEnabled = false;
    
    bool isEnabled() {
        return noShadersEnabled;
    }
    
    void setEnabled(bool enabled) {
        noShadersEnabled = enabled;
    }
}

class $modify(NoShadersHook, ShaderLayer) {
    void performCalculations() {
        if (Astral::Cosmetics::NoShaders::isEnabled()) {
            m_state.m_usesShaders = false;
            return;
        }
        ShaderLayer::performCalculations();
    }
};