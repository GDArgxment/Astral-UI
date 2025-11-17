#pragma once

#include <cocos2d.h>
#include <string>
#include <functional>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

namespace Astral::Label {

    class LabelContainer;

    /// @brief A smart label that can be managed by a container
    class SmartLabel : public cocos2d::CCLabelBMFont {
    public:
        static SmartLabel* create(const std::string& text, const std::string& font) {
            auto ret = new SmartLabel();
            if (ret->initWithString(text.c_str(), font.c_str())) {
                ret->autorelease();
                return ret;
            }
            delete ret;
            return nullptr;
        }

        /// @brief Set the parent container
        void setParentContainer(LabelContainer* container) { m_parentContainer = container; }

        /// @brief Set height multiplier for scaling
        void setHeightMultiplier(float multiplier) { m_heightMultiplier = multiplier; }

        /// @brief Set custom position offset
        void setCustomPosition(const cocos2d::CCPoint& position) { m_customPosition = position; }

        /// @brief Get custom position offset
        [[nodiscard]] cocos2d::CCPoint getCustomPosition() const { return m_customPosition; }

        /// @brief Update the label (call this every frame)
        void update();

    private:
        cocos2d::CCPoint m_customPosition = {0, 0};
        float m_heightMultiplier = 1.0f;
        float m_lastHeight = 0.f;
        bool m_wasVisible = false;
        LabelContainer* m_parentContainer = nullptr;
    };

    /// @brief Container that manages multiple labels with automatic layout
    class LabelContainer : public cocos2d::CCNode {
    public:
        enum class Alignment {
            TopLeft = 0, TopCenter = 1, TopRight = 2,
            CenterLeft = 3, Center = 4, CenterRight = 5,
            BottomLeft = 6, BottomCenter = 7, BottomRight = 8
        };

        static LabelContainer* create(Alignment alignment = Alignment::TopLeft) {
            auto ret = new LabelContainer();
            if (ret->init(alignment)) {
                ret->autorelease();
                return ret;
            }
            delete ret;
            return nullptr;
        }

        /// @brief Initialize the container with an alignment
        bool init(Alignment alignment);

        /// @brief Add a label to the container
        /// @param label The label to add
        /// @param update Callback function called every update
        void addLabel(SmartLabel* label, const std::function<void(SmartLabel*)>& update);

        /// @brief Remove a label from the container
        void removeLabel(SmartLabel* label);

        /// @brief Update all labels in the container (call every frame)
        void update();

        /// @brief Mark container as needing layout recalculation
        void invalidate() { m_dirty = true; }

        /// @brief Set the alignment of the container
        void setAlignment(Alignment alignment);

        /// @brief Get the current alignment
        [[nodiscard]] Alignment getAlignment() const { return m_alignment; }

        /// @brief Update the position and size of the container
        void updatePosition();

    private:
        /// @brief Recalculate the layout of all labels
        void recalculateLayout();

        std::vector<std::pair<SmartLabel*, std::function<void(SmartLabel*)>>> m_labels;
        Alignment m_alignment = Alignment::TopLeft;
        bool m_dirty = false;
    };

    // ============ Player Info Labels ============
    struct PlayerStats {
        cocos2d::CCPoint position;
        cocos2d::CCPoint velocity;
        float rotationX = 0.f;
        float rotationY = 0.f;
        bool isDead = false;
        int attempt = 0;
        float progress = 0.f;
        float fps = 0.f;
    };

    class PlayerInfoLabels {
    public:
        static PlayerInfoLabels* getInstance() {
            static PlayerInfoLabels instance;
            return &instance;
        }

        void init();
        void update(float dt);
        void setVisible(bool visible);

        PlayerStats getPlayer1Stats() const { return m_player1Stats; }
        PlayerStats getPlayer2Stats() const { return m_player2Stats; }

        void setPlayer1Alignment(LabelContainer::Alignment alignment) {
            if (m_containerP1) m_containerP1->setAlignment(alignment);
        }

        void setPlayer2Alignment(LabelContainer::Alignment alignment) {
            if (m_containerP2) m_containerP2->setAlignment(alignment);
        }

        void setLabelScale(float scale);
        void setLabelOpacity(float opacity);
        void setLabelVisibility(bool pos, bool vel, bool status, bool att, bool prog, bool fps);

    private:
        PlayerInfoLabels() = default;
        ~PlayerInfoLabels() = default;

        void updatePlayerStats(PlayerObject* player, PlayerStats& stats);
        void createLabels();
        void updateLabels();

        LabelContainer* m_containerP1 = nullptr;
        LabelContainer* m_containerP2 = nullptr;

        SmartLabel* m_posLabelP1 = nullptr;
        SmartLabel* m_velLabelP1 = nullptr;
        SmartLabel* m_statusLabelP1 = nullptr;
        SmartLabel* m_attemptLabel = nullptr;
        SmartLabel* m_progressLabel = nullptr;
        SmartLabel* m_fpsLabel = nullptr;

        SmartLabel* m_posLabelP2 = nullptr;
        SmartLabel* m_velLabelP2 = nullptr;
        SmartLabel* m_statusLabelP2 = nullptr;

        PlayerStats m_player1Stats;
        PlayerStats m_player2Stats;
        float m_frameCounter = 0.f;
        float m_fpsCounter = 0.f;
    };

}
