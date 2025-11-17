#pragma once
#include "includes.hpp"

namespace Astral::Hacks {
    namespace FrameStepper {
        bool isEnabled();
        void setEnabled(bool enabled);
        void setTPS(float tps);
        float getTPS();
        bool getJustStepped();
        void stepFrame();
        void stepBack();
        void clearHistory();
    }
    
    namespace Noclip {
        bool isNoclipEnabled();
        bool isNoclipP1();
        bool isNoclipP2();
        void setNoclipEnabled(bool enabled);
        void setNoclipP1(bool enabled);
        void setNoclipP2(bool enabled);
    }
    
    namespace Speedhack {
        bool isSpeedhackEnabled();
        void setSpeedhackEnabled(bool enabled);
        bool isSpeedhackAudioEnabled();
        void setSpeedhackAudioEnabled(bool enabled);
        float getSpeedhackMultiplier();
        void setSpeedhackMultiplier(float multiplier);
    }
    
    
    namespace AutoClicker {
        struct KeySettings {
            bool enabled = false;
            int intervalHold = 1;
            int intervalRelease = 1;
            int clicksPerFrame = 1;
            bool swiftClick = false;
            bool limitFrames = false;
            int maxFrames = 0;
            bool blackOrbModeEnabled = false;
            int blackOrb_clickCount = 1;
            int blackOrb_holdFrames = 1;
        };
        
        bool isEnabled();
        void setEnabled(bool enabled);
        KeySettings& getKeySettings(char key);
    }
    namespace Seedhack {
        bool isEnabled();
        void setEnabled(bool enabled);
        int getSeedValue();
        void setSeedValue(int seed);
    }
    namespace Trajectory {
        bool isEnabled();
        void setEnabled(bool enabled);
    }
    namespace Hitboxes {
        bool isEnabled();
        void setEnabled(bool enabled);
        bool getShowTrail();
        void setShowTrail(bool show);
        bool getShowRotated();
        void setShowRotated(bool show);
        bool getShowFill();
        void setShowFill(bool show);
        bool getShowInner();
        void setShowInner(bool show);
        bool getShowCircle();
        void setShowCircle(bool show);
        int getTrailLength();
        void setTrailLength(int length);
        void setHitboxThickness(float size);
    }
    namespace LayoutMode {
        bool isEnabled();
        void setEnabled(bool enabled);
    }
    namespace TPSBypass {
        bool isEnabled();
        void setEnabled(bool enabled);
        float getTPS();
        void setTPS(float tps);
    }
}