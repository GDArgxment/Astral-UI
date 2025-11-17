/*
#include "includes.hpp"
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

namespace Astral::Hacks::Trajectory {
    class TrajectorySimulation {
    private:
        PlayerObject* m_simulationPlayerJump = nullptr;
        PlayerObject* m_simulationPlayerNoJump = nullptr;
        cocos2d::CCDrawNode* m_drawNode = nullptr;
        bool m_simulating = false;
        bool m_enabled = false;
        float m_frameDt = 0.016f;  // ~60fps default

    public:
        cocos2d::CCDrawNode* getDrawNode() {
            if (!m_drawNode) {
                m_drawNode = cocos2d::CCDrawNode::create();
                m_drawNode->retain();
                m_drawNode->setID("astral-trajectory-draw-node"_spr);
            }
            return m_drawNode;
        }

        void init() {
            auto* playLayer = PlayLayer::get();
            if (!playLayer) return;

            m_simulationPlayerJump = PlayerObject::create(1, 1, playLayer, playLayer, true);
            m_simulationPlayerJump->setVisible(false);
            playLayer->addChild(m_simulationPlayerJump, -1);

            m_simulationPlayerNoJump = PlayerObject::create(1, 1, playLayer, playLayer, true);
            m_simulationPlayerNoJump->setVisible(false);
            playLayer->addChild(m_simulationPlayerNoJump, -1);

            playLayer->addChild(getDrawNode(), 1000);
        }

        void quit() {
            m_simulationPlayerJump = nullptr;
            m_simulationPlayerNoJump = nullptr;
        }

        void simulateTrajectory(PlayerObject* basePlayer, PlayerObject* simPlayer, bool shouldJump, cocos2d::ccColor4F color) {
            if (!basePlayer || !simPlayer) return;

            auto* playLayer = PlayLayer::get();
            if (!playLayer) return;

            m_simulating = true;

            simPlayer->setPosition(basePlayer->getPosition());
            simPlayer->setRotation(basePlayer->getRotation());
            simPlayer->copyAttributes(basePlayer);
            simPlayer->setVisible(false);

            bool jumpApplied = false;
            const size_t iterations = 50;  // Much smaller
            cocos2d::CCPoint basePos = basePlayer->getPosition();

            for (size_t i = 0; i < iterations; i++) {
                cocos2d::CCPoint startPos = simPlayer->getPosition();

                if (!jumpApplied) {
                    jumpApplied = true;
                    if (shouldJump) {
                        simPlayer->pushButton(PlayerButton::Jump);
                    }
                }

                simPlayer->update(m_frameDt);
                playLayer->checkCollisions(simPlayer, m_frameDt, false);

                cocos2d::CCPoint endPos = simPlayer->getPosition();
                
                // Only draw if reasonably close to base position
                float dx = endPos.x - basePos.x;
                float dy = endPos.y - basePos.y;
                if (std::abs(dx) < 300 && std::abs(dy) < 300) {
                    getDrawNode()->drawSegment(startPos, endPos, 2.0f, color);
                }

                // Stop if too far away
                if (std::abs(dy) > 300 || std::abs(dx) > 300) {
                    break;
                }
            }

            m_simulating = false;
        }

        void simulate(PlayerObject* basePlayer) {
            if (!m_enabled || !basePlayer) {
                log::info("Simulate called but basePlayer is null or not enabled");
                return;
            }

            auto* playLayer = PlayLayer::get();
            if (!playLayer) return;

            getDrawNode()->clear();

            simulateTrajectory(basePlayer, m_simulationPlayerJump, true, cocos2d::ccColor4F{0.2f, 1.0f, 0.3f, 1.0f});
            simulateTrajectory(basePlayer, m_simulationPlayerNoJump, false, cocos2d::ccColor4F{1.0f, 0.2f, 0.2f, 1.0f});
        }

        bool isEnabled() const {
            return m_enabled;
        }

        void setEnabled(bool enabled) {
            m_enabled = enabled;
            if (m_drawNode) {
                m_drawNode->setVisible(enabled);
                if (!enabled) {
                    m_drawNode->clear();
                }
            }
        }

        void setFrameDelta(float dt) {
            m_frameDt = dt;
        }
    };

    static TrajectorySimulation s_simulation;

    bool isEnabled() {
        return s_simulation.isEnabled();
    }

    void setEnabled(bool enabled) {
        s_simulation.setEnabled(enabled);
    }

    class $modify(TrajectoryGJBGLHook, GJBaseGameLayer) {
        void update(float dt) override {
            GJBaseGameLayer::update(dt);

            if (!s_simulation.isEnabled()) return;

            auto* playLayer = PlayLayer::get();
            if (!playLayer) {
                return;
            }

            bool isPlaying = !playLayer->m_hasCompletedLevel &&
                            !playLayer->m_isPaused &&
                            playLayer->m_gameState.m_currentProgress > 0.0f;

            if (!isPlaying) return;

            // Try different ways to get the player
            PlayerObject* player = nullptr;
            
            // Try m_player1 first (GD style)
            try {
                player = playLayer->m_player1;
                if (player) {
                    AstralLogger::logDebug("Found player via m_player1 at position ({}, {})", player->getPositionX(), player->getPositionY());
                }
            } catch (...) {
                AstralLogger::logDebug("m_player1 not accessible");
            }
            
            // If that didn't work, try other common names
            if (!player) {
                try {
                    // Try to get any child that's a PlayerObject
                    auto children = playLayer->getChildren();
                    if (children && children->count() > 0) {
                        AstralLogger::logDebug("PlayLayer has {} children", children->count());
                        for (int i = 0; i < children->count(); i++) {
                            auto* child = dynamic_cast<PlayerObject*>(children->objectAtIndex(i));
                            if (child && child->isVisible()) {
                                player = child;
                                AstralLogger::logDebug("Found player as visible child at index {}", i);
                                break;
                            }
                        }
                    }
                } catch (...) {
                    AstralLogger::logDebug("Failed to search children");
                }
            }

            if (player) {
                AstralLogger::logDebug("Simulating trajectory for player at ({}, {})", player->getPositionX(), player->getPositionY());
                s_simulation.setFrameDelta(dt);
                s_simulation.simulate(player);
            } else {
                AstralLogger::logDebug("Could not find player object");
            }
        }
    };

    class $modify(TrajectoryPLHook, PlayLayer) {
        bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
            if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
            s_simulation.init();
            return true;
        }
    };
}
*/