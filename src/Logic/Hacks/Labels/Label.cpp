#include "Label.hpp"

namespace Astral::Label {

    // ============ LabelContainer Implementation ============
    bool LabelContainer::init(Alignment alignment) {
        if (!cocos2d::CCNode::init()) return false;

        m_alignment = alignment;
        updatePosition();
        return true;
    }

    void LabelContainer::updatePosition() {
        auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
        float padding = 3.f;

        setContentSize({ winSize.width - padding * 2, winSize.height - padding * 2 });

        switch (m_alignment) {
            case Alignment::TopLeft:
                setPosition(padding, winSize.height - padding);
                setAnchorPoint({0, 1});
                break;
            case Alignment::TopCenter:
                setPosition(winSize.width / 2, winSize.height - padding);
                setAnchorPoint({0.5, 1});
                break;
            case Alignment::TopRight:
                setPosition(winSize.width - padding, winSize.height - padding);
                setAnchorPoint({1, 1});
                break;
            case Alignment::CenterLeft:
                setPosition(padding, winSize.height / 2);
                setAnchorPoint({0, 0.5});
                break;
            case Alignment::Center:
                setPosition(winSize.width / 2, winSize.height / 2);
                setAnchorPoint({0.5, 0.5});
                break;
            case Alignment::CenterRight:
                setPosition(winSize.width - padding, winSize.height / 2);
                setAnchorPoint({1, 0.5});
                break;
            case Alignment::BottomLeft:
                setPosition(padding, padding);
                setAnchorPoint({0, 0});
                break;
            case Alignment::BottomCenter:
                setPosition(winSize.width / 2, padding);
                setAnchorPoint({0.5, 0});
                break;
            case Alignment::BottomRight:
                setPosition(winSize.width - padding, padding);
                setAnchorPoint({1, 0});
                break;
        }
    }

    void LabelContainer::recalculateLayout() {
        float yOffset = 0.f;
        
        for (auto& [label, updater] : m_labels) {
            if (!label->isVisible()) continue;

            label->setPosition(0, yOffset);
            auto size = label->getContentSize();
            yOffset -= (size.height + 2.f);
        }
    }

    void LabelContainer::addLabel(SmartLabel* label, const std::function<void(SmartLabel*)>& update) {
        label->setParentContainer(this);
        m_labels.emplace_back(label, update);

        switch (m_alignment) {
            case Alignment::TopLeft:
            case Alignment::CenterLeft:
            case Alignment::BottomLeft:
                label->setAnchorPoint({0, 1});
                break;
            case Alignment::TopCenter:
            case Alignment::Center:
            case Alignment::BottomCenter:
                label->setAnchorPoint({0.5, 1});
                break;
            case Alignment::TopRight:
            case Alignment::CenterRight:
            case Alignment::BottomRight:
                label->setAnchorPoint({1, 1});
                break;
        }

        addChild(label);
    }

    void LabelContainer::removeLabel(SmartLabel* label) {
        auto it = std::find_if(m_labels.begin(), m_labels.end(), 
            [label](const auto& pair) { return pair.first == label; });

        if (it != m_labels.end()) {
            m_labels.erase(it);
            removeChild(label);
        }
    }

    void LabelContainer::update() {
        if (!isVisible()) return;

        for (auto& [label, updater] : m_labels) {
            updater(label);
            label->update();
        }

        if (m_dirty) {
            recalculateLayout();
            m_dirty = false;
        }
    }

    void LabelContainer::setAlignment(Alignment alignment) {
        m_alignment = alignment;
        updatePosition();
        recalculateLayout();
    }

    void SmartLabel::update() {
        if (!isVisible()) {
            if (m_wasVisible) {
                m_wasVisible = false;
                if (m_parentContainer) m_parentContainer->invalidate();
            }
            return;
        }

        if (!m_wasVisible) {
            m_wasVisible = true;
            if (m_parentContainer) m_parentContainer->invalidate();
        }

        auto size = this->getContentSize();
        size.height *= m_fScaleY;

        if (size.height != m_lastHeight) {
            if (m_heightMultiplier != 1.0f) {
                size.height *= m_heightMultiplier;
                this->setContentSize({ size.width, size.height / m_fScaleY });
            }

            m_lastHeight = size.height;
            if (m_parentContainer) {
                m_parentContainer->invalidate();
            }
        }
    }

    // ============ PlayerInfoLabels Implementation ============
    void PlayerInfoLabels::init() {
        auto director = cocos2d::CCDirector::sharedDirector();
        
        m_containerP1 = LabelContainer::create(LabelContainer::Alignment::TopLeft);
        m_containerP1->setID("player1-info");
        director->getRunningScene()->addChild(m_containerP1, 1000);

        m_containerP2 = LabelContainer::create(LabelContainer::Alignment::TopRight);
        m_containerP2->setID("player2-info");
        director->getRunningScene()->addChild(m_containerP2, 1000);

        createLabels();
    }

    void PlayerInfoLabels::createLabels() {
        m_posLabelP1 = SmartLabel::create("Pos: (0, 0)", "bigFont.fnt");
        m_containerP1->addLabel(m_posLabelP1, [](SmartLabel* lbl) {});

        m_velLabelP1 = SmartLabel::create("Vel: (0, 0)", "bigFont.fnt");
        m_containerP1->addLabel(m_velLabelP1, [](SmartLabel* lbl) {});

        m_statusLabelP1 = SmartLabel::create("Status: ALIVE", "bigFont.fnt");
        m_containerP1->addLabel(m_statusLabelP1, [](SmartLabel* lbl) {});

        m_attemptLabel = SmartLabel::create("Attempt: 0", "bigFont.fnt");
        m_containerP1->addLabel(m_attemptLabel, [](SmartLabel* lbl) {});

        m_progressLabel = SmartLabel::create("Progress: 0%", "bigFont.fnt");
        m_containerP1->addLabel(m_progressLabel, [](SmartLabel* lbl) {});

        m_fpsLabel = SmartLabel::create("FPS: 60", "bigFont.fnt");
        m_containerP1->addLabel(m_fpsLabel, [](SmartLabel* lbl) {});

        m_posLabelP2 = SmartLabel::create("P2 Pos: (0, 0)", "bigFont.fnt");
        m_containerP2->addLabel(m_posLabelP2, [](SmartLabel* lbl) {});

        m_velLabelP2 = SmartLabel::create("P2 Vel: (0, 0)", "bigFont.fnt");
        m_containerP2->addLabel(m_velLabelP2, [](SmartLabel* lbl) {});

        m_statusLabelP2 = SmartLabel::create("P2 Status: ALIVE", "bigFont.fnt");
        m_containerP2->addLabel(m_statusLabelP2, [](SmartLabel* lbl) {});
    }

    void PlayerInfoLabels::updatePlayerStats(PlayerObject* player, PlayerStats& stats) {
        if (!player) return;

        stats.position = player->getPosition();
        stats.velocity.x = 0;
        stats.velocity.y = 0;
        stats.rotationX = player->getRotationX();
        stats.rotationY = player->getRotationY();
        stats.isDead = player->m_isDead;
    }

    void PlayerInfoLabels::updateLabels() {
        if (!m_posLabelP1 || !m_velLabelP1 || !m_statusLabelP1) return;

        std::stringstream posStream;
        posStream << std::fixed << std::setprecision(1);
        posStream << "Pos: (" << m_player1Stats.position.x << ", " << m_player1Stats.position.y << ")";
        m_posLabelP1->setString(posStream.str().c_str());

        std::stringstream velStream;
        velStream << std::fixed << std::setprecision(1);
        velStream << "Vel: (" << m_player1Stats.velocity.x << ", " << m_player1Stats.velocity.y << ")";
        m_velLabelP1->setString(velStream.str().c_str());

        std::string status = m_player1Stats.isDead ? "Status: DEAD" : "Status: ALIVE";
        m_statusLabelP1->setString(status.c_str());
        m_statusLabelP1->setColor(m_player1Stats.isDead ? cocos2d::ccRED : cocos2d::ccGREEN);

        if (m_attemptLabel) {
            std::stringstream attStream;
            attStream << "Attempt: " << m_player1Stats.attempt;
            m_attemptLabel->setString(attStream.str().c_str());
        }

        if (m_progressLabel) {
            std::stringstream progStream;
            progStream << std::fixed << std::setprecision(1);
            progStream << "Progress: " << m_player1Stats.progress << "%";
            m_progressLabel->setString(progStream.str().c_str());
        }

        if (m_fpsLabel) {
            std::stringstream fpsStream;
            fpsStream << std::fixed << std::setprecision(0);
            fpsStream << "FPS: " << m_fpsCounter;
            m_fpsLabel->setString(fpsStream.str().c_str());
        }

        if (m_posLabelP2 && m_velLabelP2 && m_statusLabelP2) {
            std::stringstream pos2Stream;
            pos2Stream << std::fixed << std::setprecision(1);
            pos2Stream << "P2 Pos: (" << m_player2Stats.position.x << ", " << m_player2Stats.position.y << ")";
            m_posLabelP2->setString(pos2Stream.str().c_str());

            std::stringstream vel2Stream;
            vel2Stream << std::fixed << std::setprecision(1);
            vel2Stream << "P2 Vel: (" << m_player2Stats.velocity.x << ", " << m_player2Stats.velocity.y << ")";
            m_velLabelP2->setString(vel2Stream.str().c_str());

            std::string status2 = m_player2Stats.isDead ? "P2 Status: DEAD" : "P2 Status: ALIVE";
            m_statusLabelP2->setString(status2.c_str());
            m_statusLabelP2->setColor(m_player2Stats.isDead ? cocos2d::ccRED : cocos2d::ccGREEN);
        }
    }

    void PlayerInfoLabels::update(float dt) {
        auto gjbgl = cocos2d::CCDirector::sharedDirector()->getRunningScene();
        if (!gjbgl) return;

        auto playLayer = dynamic_cast<PlayLayer*>(gjbgl);
        if (!playLayer) return;

        m_frameCounter++;
        m_fpsCounter = 1.0f / dt;

        if (playLayer->m_player1) {
            updatePlayerStats(playLayer->m_player1, m_player1Stats);
            m_player1Stats.attempt = 0; // Placeholder - set based on your mod's attempt tracking
            m_player1Stats.progress = 0; // Placeholder - calculate based on level progress
        }

        if (playLayer->m_player2) {
            updatePlayerStats(playLayer->m_player2, m_player2Stats);
        }

        updateLabels();

        if (m_containerP1) m_containerP1->update();
        if (m_containerP2) m_containerP2->update();
    }

    void PlayerInfoLabels::setVisible(bool visible) {
        if (m_containerP1) m_containerP1->setVisible(visible);
        if (m_containerP2) m_containerP2->setVisible(visible);
    }

    void PlayerInfoLabels::setLabelScale(float scale) {
        if (m_posLabelP1) m_posLabelP1->setScale(scale);
        if (m_velLabelP1) m_velLabelP1->setScale(scale);
        if (m_statusLabelP1) m_statusLabelP1->setScale(scale);
        if (m_attemptLabel) m_attemptLabel->setScale(scale);
        if (m_progressLabel) m_progressLabel->setScale(scale);
        if (m_fpsLabel) m_fpsLabel->setScale(scale);

        if (m_posLabelP2) m_posLabelP2->setScale(scale);
        if (m_velLabelP2) m_velLabelP2->setScale(scale);
        if (m_statusLabelP2) m_statusLabelP2->setScale(scale);
    }

    void PlayerInfoLabels::setLabelOpacity(float opacity) {
        GLubyte opacityByte = static_cast<GLubyte>(opacity * 255);

        if (m_posLabelP1) m_posLabelP1->setOpacity(opacityByte);
        if (m_velLabelP1) m_velLabelP1->setOpacity(opacityByte);
        if (m_statusLabelP1) m_statusLabelP1->setOpacity(opacityByte);
        if (m_attemptLabel) m_attemptLabel->setOpacity(opacityByte);
        if (m_progressLabel) m_progressLabel->setOpacity(opacityByte);
        if (m_fpsLabel) m_fpsLabel->setOpacity(opacityByte);

        if (m_posLabelP2) m_posLabelP2->setOpacity(opacityByte);
        if (m_velLabelP2) m_velLabelP2->setOpacity(opacityByte);
        if (m_statusLabelP2) m_statusLabelP2->setOpacity(opacityByte);
    }

    void PlayerInfoLabels::setLabelVisibility(bool pos, bool vel, bool status, bool att, bool prog, bool fps) {
        if (m_posLabelP1) m_posLabelP1->setVisible(pos);
        if (m_velLabelP1) m_velLabelP1->setVisible(vel);
        if (m_statusLabelP1) m_statusLabelP1->setVisible(status);
        if (m_attemptLabel) m_attemptLabel->setVisible(att);
        if (m_progressLabel) m_progressLabel->setVisible(prog);
        if (m_fpsLabel) m_fpsLabel->setVisible(fps);

        if (m_posLabelP2) m_posLabelP2->setVisible(pos);
        if (m_velLabelP2) m_velLabelP2->setVisible(vel);
        if (m_statusLabelP2) m_statusLabelP2->setVisible(status);

        if (m_containerP1) m_containerP1->invalidate();
        if (m_containerP2) m_containerP2->invalidate();
    }

}