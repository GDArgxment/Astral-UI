#pragma once
#include "includes.hpp"

namespace Astral::Cosmetics {
    namespace KeepWave {
        bool isEnabled();
        void setEnabled(bool enabled);
        bool noFadeOutEnabled();
        void setNoFadeOut(bool enabled);
        bool noResetEnabled();
        void setNoReset(bool enabled);
    }
    
    namespace InstantMirror {
        bool isEnabled();
        void setEnabled(bool enabled);
    }
    
    namespace NoDashFire {
        bool isEnabled();
        void setEnabled(bool enabled);
    }
    
    namespace NoDeathEffect {
        bool isEnabled();
        void setEnabled(bool enabled);
    }
    
    namespace NoGhostTrail {
        bool isEnabled();
        void setEnabled(bool enabled);
        void updateGhostTrail();
    }
    
    namespace NoMirror {
        bool isEnabled();
        void setEnabled(bool enabled);
    }
    
    namespace NoOrbEffects {
        bool isEnabled();
        void setEnabled(bool enabled);
        void setNoOrbRing(bool enabled);
        void setNoOrbHitEffect(bool enabled);
        void setNoDashBoom(bool enabled);
    }
    
    namespace NoRespawnFlash {
        bool isEnabled();
        void setEnabled(bool enabled);
    }
    
    namespace NoShaders {
        bool isEnabled();
        void setEnabled(bool enabled);
    }
    
    namespace UnlockEverything {
        bool isEnabled();
        void setEnabled(bool enabled);
    }
    
    namespace WaveDragFix {
        bool isEnabled();
        void setEnabled(bool enabled);
    }
    namespace NoTrail {
        bool isEnabled();
        void setEnabled(bool enabled);
        bool isAlwaysEnabled();
        void setAlwaysEnabled(bool enabled);
    }
}
