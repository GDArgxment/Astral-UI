#pragma once
#include "includes.hpp"

namespace Astral::QOL {
    namespace AccuratePercentage {
        bool isEnabled();
        void setEnabled(bool enabled);
        int getDecimalPlaces();
        void setDecimalPlaces(int places);
    }
    
    namespace AutoSafeMode {
        bool isEnabled();
        void setEnabled(bool enabled);
    }
    
    namespace RespawnDelay {
        float getDelay();
        void setDelay(float delay);
    }
    
    namespace SafeMode {
        bool isEnabled();
        void setEnabled(bool enabled);
    }
}