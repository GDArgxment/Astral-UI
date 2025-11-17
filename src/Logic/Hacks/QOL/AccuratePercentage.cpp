#include "includes.hpp"
using namespace geode::prelude;

namespace Astral::QOL::AccuratePercentage {
    static bool accuratePercentage = false;
    static int decimalPlaces = 2;
    
    bool isEnabled() {
        return accuratePercentage;
    }

    void setEnabled(bool enabled) {
        accuratePercentage = enabled;
    }
    
    int getDecimalPlaces() {
        return decimalPlaces;
    }
    
    void setDecimalPlaces(int places) {
        decimalPlaces = places;
    }
}

class $modify(AccuratePercentagePLHook, PlayLayer) {
    float customGetProgress() {
        if (m_player1 && m_levelLength > 0) {
            float percent = (m_player1->getPositionX() / m_levelLength) * 100.0f;
            percent = std::clamp(percent, 0.0f, 100.0f);
            return percent;
        }
        return this->getCurrentPercent();
    }

    void updateProgressbar() {
        PlayLayer::updateProgressbar();

        using namespace Astral::QOL::AccuratePercentage;
        
        if (!isEnabled() || !m_percentageLabel) return;

        float percent = customGetProgress();
        m_percentageLabel->setString(fmt::format("{:.{}f}%", percent, getDecimalPlaces()).c_str());

        if (m_progressFill && m_progressBar) {
            m_progressFill->setTextureRect({
                0, 0,
                (m_progressBar->getTextureRect().getMaxX() - 5) * percent / 100.f,
                m_progressBar->getTextureRect().getMaxY() / 2
            });
        }
    }
};