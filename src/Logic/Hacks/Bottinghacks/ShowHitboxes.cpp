#include <ranges>
#include "includes.hpp"

namespace Astral::Hacks::Hitboxes {
    static bool s_isDead = false;
    static GameObject* s_collisionObject = nullptr;
    static std::deque<std::pair<cocos2d::CCRect, cocos2d::CCRect>> s_playerTrail1, s_playerTrail2;
    static std::deque<std::pair<cocos2d::CCRect, cocos2d::CCRect>> s_playerTrailRotated1, s_playerTrailRotated2;
    static bool s_enabled = false;
    static bool s_showTrail = false;
    static bool s_showRotated = true;
    static bool s_showFill = false;
    static bool s_showInner = true;
    static bool s_showCircle = true;
    static int s_trailLength = 240;
    static float s_borderSize = 0.65f;

    void setEnabled(bool enabled) {
        s_enabled = enabled;
        if (!enabled) {
            s_isDead = false;
            s_collisionObject = nullptr;
            s_playerTrail1.clear();
            s_playerTrail2.clear();
        }
    }

    bool isEnabled() {
        return s_enabled;
    }

    void setShowTrail(bool show) {
        s_showTrail = show;
        if (!show) {
            s_playerTrail1.clear();
            s_playerTrail2.clear();
            s_playerTrailRotated1.clear();
            s_playerTrailRotated2.clear();
        }
    }

    bool getShowTrail() {
        return s_showTrail;
    }

    void setShowRotated(bool show) {
        s_showRotated = show;
    }

    bool getShowRotated() {
        return s_showRotated;
    }

    void setShowFill(bool show) {
        s_showFill = show;
    }

    bool getShowFill() {
        return s_showFill;
    }

    void setShowInner(bool show) {
        s_showInner = show;
    }

    bool getShowInner() {
        return s_showInner;
    }

    void setShowCircle(bool show) {
        s_showCircle = show;
    }

    bool getShowCircle() {
        return s_showCircle;
    }

    void setTrailLength(int length) {
        s_trailLength = std::max(1, length);
    }

    int getTrailLength() {
        return s_trailLength;
    }

    void setBorderSize(float size) {
        s_borderSize = std::max(0.1f, size);
    }

    void setHitboxThickness(float thickness) {
        setBorderSize(thickness);
    }

    float getBorderSize() {
        return s_borderSize;
    }

    static void forEachObject(GJBaseGameLayer const* game, std::function<void(GameObject*)> const& callback) {
        int count = game->m_sections.empty() ? -1 : game->m_sections.size();
        for (int i = game->m_leftSectionIndex; i <= game->m_rightSectionIndex && i < count; ++i) {
            auto leftSection = game->m_sections[i];
            if (!leftSection) continue;

            auto leftSectionSize = leftSection->size();
            for (int j = game->m_bottomSectionIndex; j <= game->m_topSectionIndex && j < leftSectionSize; ++j) {
                auto section = leftSection->at(j);
                if (!section) continue;

                auto sectionSize = game->m_sectionSizes[i]->at(j);
                for (int k = 0; k < sectionSize; ++k) {
                    auto obj = section->at(k);
                    if (!obj) continue;

                    callback(obj);
                }
            }
        }
    }

    inline void drawRotatedRect(
        cocos2d::CCDrawNode* node, cocos2d::CCPoint const* corners, cocos2d::ccColor4F const& borderColor,
        float borderWidth
    ) {
        // Draw four line segments for the rotated rectangle
        for (int i = 0; i < 4; i++) {
            int nextI = (i + 1) % 4;
            node->drawSegment(corners[i], corners[nextI], borderWidth, borderColor);
        }
    }

    inline void drawRect(
        cocos2d::CCDrawNode* node, cocos2d::CCRect const& rect, cocos2d::ccColor4F const& color,
        float borderWidth, cocos2d::ccColor4F const& borderColor
    ) {
        // Inset all coordinates by borderWidth so lines stay inside the hitbox
        float insetX = borderWidth;
        float insetY = borderWidth;
        
        // Draw filled rectangle if color has alpha > 0
        if (color.a > 0) {
            std::array<cocos2d::CCPoint, 4> vertices = {
                cocos2d::CCPoint{rect.getMinX(), rect.getMinY()},
                cocos2d::CCPoint{rect.getMinX(), rect.getMaxY()},
                cocos2d::CCPoint{rect.getMaxX(), rect.getMaxY()},
                cocos2d::CCPoint{rect.getMaxX(), rect.getMinY()}
            };
            node->drawPolygon(vertices.data(), vertices.size(), color, 0, color);
        }
        
        // Bottom line (at actual bottom of hitbox)
        node->drawSegment(
            cocos2d::CCPoint{rect.getMinX() + insetX, rect.getMinY() + insetY},
            cocos2d::CCPoint{rect.getMaxX() - insetX, rect.getMinY() + insetY},
            borderWidth, borderColor
        );
        
        // Top line (at actual top of hitbox)
        node->drawSegment(
            cocos2d::CCPoint{rect.getMinX() + insetX, rect.getMaxY() - insetY},
            cocos2d::CCPoint{rect.getMaxX() - insetX, rect.getMaxY() - insetY},
            borderWidth, borderColor
        );
        
        // Left line (at actual left of hitbox)
        node->drawSegment(
            cocos2d::CCPoint{rect.getMinX() + insetX, rect.getMinY() + insetY},
            cocos2d::CCPoint{rect.getMinX() + insetX, rect.getMaxY() - insetY},
            borderWidth, borderColor
        );
        
        // Right line (at actual right of hitbox)
        node->drawSegment(
            cocos2d::CCPoint{rect.getMaxX() - insetX, rect.getMinY() + insetY},
            cocos2d::CCPoint{rect.getMaxX() - insetX, rect.getMaxY() - insetY},
            borderWidth, borderColor
        );
    }

    inline float getObjectRadius(GameObject const* obj) {
        return std::max(obj->m_scaleX, obj->m_scaleY) * obj->m_objectRadius;
    }

    class $modify(ShowHitboxesGJBGLHook, GJBaseGameLayer) {
        struct Fields {
            cocos2d::CCDrawNode* m_drawNode;
        };

        void createDrawNode() {
            auto* parent = m_debugDrawNode->getParent();
            if (!parent) return;

            s_isDead = false;
            s_collisionObject = nullptr;

            auto* drawNode = cocos2d::CCDrawNode::create();
            drawNode->setBlendFunc({GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
            drawNode->setID("hitboxes"_spr);
            drawNode->m_bUseArea = false;
            parent->addChild(drawNode, 1402);

            m_fields->m_drawNode = drawNode;
        }

        cocos2d::CCDrawNode* getDrawNode() {
            return m_fields->m_drawNode;
        }

        void visitHitboxes() {
            auto* drawNode = m_fields->m_drawNode;
            if (!drawNode) return;

            drawNode->clear();

            if (!s_enabled || (m_isEditor && !s_enabled))
                return;

            auto solidColor = cocos2d::ccc4f(0, 0.247f, 1, 1);
            auto dangerColor = cocos2d::ccc4f(1, 0, 0, 1);
            auto passableColor = cocos2d::ccc4f(0, 1, 1, 1);
            auto otherColor = cocos2d::ccc4f(0, 1, 0, 1);
            auto triggersColor = cocos2d::ccc4f(1, 0, 0.9f, 1);
            auto playerColor = cocos2d::ccc4f(1, 0.2f, 0.2f, 0.8f);       // Bright red, slightly transparent
            auto playerColorInner = cocos2d::ccc4f(0.2f, 0.4f, 1, 0.8f);  // Bright blue, slightly transparent
            auto playerColorRotated = cocos2d::ccc4f(0.5f, 0, 0, 0.7f);   // Dark red, more transparent

            float fillAlpha = s_showFill ? 0.15f : 0.0f;
            float borderSize = s_borderSize;      // Use the configurable border size

            auto solidColorFill = cocos2d::ccc4f(solidColor.r, solidColor.g, solidColor.b, fillAlpha);
            auto dangerColorFill = cocos2d::ccc4f(dangerColor.r, dangerColor.g, dangerColor.b, fillAlpha);
            auto passableColorFill = cocos2d::ccc4f(passableColor.r, passableColor.g, passableColor.b, fillAlpha);
            auto otherColorFill = cocos2d::ccc4f(otherColor.r, otherColor.g, otherColor.b, fillAlpha);
            auto triggersColorFill = cocos2d::ccc4f(triggersColor.r, triggersColor.g, triggersColor.b, fillAlpha);
            auto playerColorFill = cocos2d::ccc4f(0.6f, 0.1f, 0.1f, fillAlpha);      // Darker red for fill
            auto playerColorInnerFill = cocos2d::ccc4f(0.1f, 0.2f, 0.6f, fillAlpha); // Darker blue for fill
            auto playerColorRotatedFill = cocos2d::ccc4f(playerColorRotated.r, playerColorRotated.g, playerColorRotated.b, fillAlpha);

            const auto visitObject = [&](GameObject* obj) {
                // skip objects that don't have a hitbox or are not activated
                if (obj->m_objectType == GameObjectType::Decoration || !obj->m_isActivated || obj->m_isGroupDisabled)
                    return;

                switch (obj->m_objectType) {
                    default: { // pretty much everything (portals, orbs, etc.)
                        constexpr auto drawFunc = [](
                            cocos2d::CCDrawNode* node, GameObject* obj,
                            cocos2d::ccColor4F const& colorFill, float borderWidth,
                            cocos2d::ccColor4F const& color
                        ) {
                            if (auto orientedBox = obj->m_orientedBox) {
                                node->drawPolygon(
                                    orientedBox->m_corners.data(), 4,
                                    colorFill, borderWidth, color
                                );
                            } else {
                                auto isObjectRectDirty = obj->m_isObjectRectDirty;
                                auto boxOffsetCalculated = obj->m_boxOffsetCalculated;
                                drawRect(node, obj->getObjectRect(), colorFill, borderWidth, color);
                                obj->m_isObjectRectDirty = isObjectRectDirty;
                                obj->m_boxOffsetCalculated = boxOffsetCalculated;
                            }
                        };

                        if (obj->m_objectType == GameObjectType::Modifier) {
                            if (!static_cast<EffectGameObject*>(obj)->m_isTouchTriggered) return;
                            return drawFunc(
                                drawNode, obj,
                                triggersColorFill, borderSize,
                                triggersColor
                            );
                        }

                        if (obj == m_player1 || obj == m_player2) {
                            return;
                        }

                        drawFunc(
                            drawNode, obj,
                            otherColorFill, borderSize,
                            otherColor
                        );
                        break;
                    }
                    case GameObjectType::Solid: {
                        auto& objColor = obj->m_isPassable ? passableColor : solidColor;
                        auto& objColorFill = obj->m_isPassable ? passableColorFill : solidColorFill;
                        drawRect(
                            drawNode, obj->getObjectRect(),
                            objColorFill, borderSize,
                            objColor
                        );
                        break;
                    }
                    case GameObjectType::Slope: {
                        auto rect = obj->getObjectRect();
                        std::array vertices = {
                            cocos2d::CCPoint{rect.getMinX(), rect.getMinY()}, // Bottom left
                            cocos2d::CCPoint{rect.getMinX(), rect.getMaxY()}, // Top left
                            cocos2d::CCPoint{rect.getMaxX(), rect.getMinY()}, // Bottom right
                        };

                        cocos2d::CCPoint topRight{rect.getMaxX(), rect.getMaxY()};
                        switch (obj->m_slopeDirection) {
                            case 0: case 7:
                                vertices[1] = topRight;
                                break;
                            case 1: case 5:
                                vertices[0] = topRight;
                                break;
                            case 3: case 6:
                                vertices[2] = topRight;
                                break;
                            default: break;
                        }

                        drawNode->drawPolygon(
                            vertices.data(), vertices.size(),
                            obj->m_isPassable ? passableColorFill : solidColorFill,
                            borderSize, obj->m_isPassable ? passableColor : solidColor
                        );
                        break;
                    }
                    case GameObjectType::AnimatedHazard:
                    case GameObjectType::Hazard: {
                        if (!obj->m_isActivated || obj == m_anticheatSpike) break;
                        if (auto radius = getObjectRadius(obj); radius > 0) {
                            drawNode->drawCircle(
                                obj->getPosition(), radius,
                                dangerColorFill, borderSize,
                                dangerColor, 16
                            );
                        } else if (auto orientedBox = obj->m_orientedBox) {
                            drawNode->drawPolygon(
                                orientedBox->m_corners.data(), 4,
                                dangerColorFill, borderSize, dangerColor
                            );
                        } else {
                            auto isObjectRectDirty = obj->m_isObjectRectDirty;
                            auto boxOffsetCalculated = obj->m_boxOffsetCalculated;
                            drawRect(drawNode, obj->getObjectRect(), dangerColorFill, borderSize, dangerColor);
                            obj->m_isObjectRectDirty = isObjectRectDirty;
                            obj->m_boxOffsetCalculated = boxOffsetCalculated;
                        }
                        break;
                    }
                    case GameObjectType::CollisionObject: break;
                }
            };

            forEachObject(this, visitObject);

            // draw player trails
            if (s_showTrail) {
                for (auto const& rect : s_playerTrail1 | std::views::keys) drawRect(
                    drawNode, rect,
                    playerColorFill, borderSize,
                    playerColor
                );

                for (auto const& rect : s_playerTrail2 | std::views::keys) drawRect(
                    drawNode, rect,
                    playerColorFill, borderSize,
                    playerColor
                );

                for (auto const& rect : s_playerTrail1 | std::views::values) drawRect(
                    drawNode, rect,
                    playerColorInnerFill, borderSize,
                    playerColorInner
                );

                for (auto const& rect : s_playerTrail2 | std::views::values) drawRect(
                    drawNode, rect,
                    playerColorInnerFill, borderSize,
                    playerColorInner
                );

                if (s_showRotated) {
                    for (auto const& rect : s_playerTrailRotated1 | std::views::keys) drawRect(
                        drawNode, rect,
                        playerColorRotatedFill, borderSize,
                        playerColorRotated
                    );

                    for (auto const& rect : s_playerTrailRotated2 | std::views::keys) drawRect(
                        drawNode, rect,
                        playerColorRotatedFill, borderSize,
                        playerColorRotated
                    );
                }
            }

            // draw player hitboxes
            const auto drawPlayer = [&](PlayerObject* player) {
                if (!player) return;

                auto rect1 = player->getObjectRect();
                auto rect2 = player->getObjectRect(0.3f, 0.3f);

                // Layer 1: Draw rotated first (bottom layer)
                if (s_showRotated && player->m_orientedBox) {
                    drawRotatedRect(drawNode, player->m_orientedBox->m_corners.data(), playerColorRotated, borderSize);
                }

                // Layer 2: Draw red outer hitbox
                drawRect(drawNode, rect1, playerColorFill, borderSize, playerColor);

                // Layer 3: Draw blue inner hitbox (middle) - on top
                if (s_showInner) {
                    drawRect(drawNode, rect2, playerColorInnerFill, borderSize, playerColorInner);
                }

                // Layer 4: Draw circle hitbox on top (no trail)
                if (s_showCircle) {
                    auto radius = getObjectRadius(player);
                    if (radius > 0) {
                        // Inset the circle by half the border width so outer edge stays at hitbox boundary
                        auto insetRadius = radius - (borderSize / 2.0f);
                        
                        // Draw the circle inset
                        drawNode->drawCircle(player->getPosition(), insetRadius, cocos2d::ccc4f(0, 0, 0, 0), borderSize, playerColor, 32);
                    }
                }
            };

            drawPlayer(m_player1);
            if (m_gameState.m_isDualMode) {
                drawPlayer(m_player2);
            }
        }

        void processCommands(float dt) {
            GJBaseGameLayer::processCommands(dt);

            if (s_isDead || !s_enabled || !s_showTrail)
                return;

            s_playerTrail1.emplace_back(
                m_player1->getObjectRect(),
                m_player1->getObjectRect(0.3f, 0.3f)
            );

            if (s_showRotated && m_player1->m_orientedBox) {
                auto rotatedRect = m_player1->getObjectRect();
                s_playerTrailRotated1.emplace_back(rotatedRect, rotatedRect);
            }

            if (m_gameState.m_isDualMode) {
                s_playerTrail2.emplace_back(
                    m_player2->getObjectRect(),
                    m_player2->getObjectRect(0.3f, 0.3f)
                );

                if (s_showRotated && m_player2->m_orientedBox) {
                    auto rotatedRect = m_player2->getObjectRect();
                    s_playerTrailRotated2.emplace_back(rotatedRect, rotatedRect);
                }
            }

            while (s_playerTrail1.size() > s_trailLength)
                s_playerTrail1.pop_front();

            while (s_playerTrail2.size() > s_trailLength)
                s_playerTrail2.pop_front();

            while (s_playerTrailRotated1.size() > s_trailLength)
                s_playerTrailRotated1.pop_front();

            while (s_playerTrailRotated2.size() > s_trailLength)
                s_playerTrailRotated2.pop_front();
        }
    };

    #define $hitbox static_cast<ShowHitboxesGJBGLHook*>(static_cast<GJBaseGameLayer*>(this))

    class $modify(ShowHitboxesPLHook, PlayLayer) {
        void createObjectsFromSetupFinished() {
            PlayLayer::createObjectsFromSetupFinished();
            $hitbox->createDrawNode();
        }

        void updateVisibility(float dt) override {
            PlayLayer::updateVisibility(dt);
            s_isDead = m_player1->m_isDead;
            $hitbox->visitHitboxes();
        }

        void resetLevel() {
            PlayLayer::resetLevel();

            s_isDead = false;
            s_collisionObject = nullptr;

            s_playerTrail1.clear();
            s_playerTrail2.clear();
            s_playerTrailRotated1.clear();
            s_playerTrailRotated2.clear();
        }

        void destroyPlayer(PlayerObject* player, GameObject* object) override {
            PlayLayer::destroyPlayer(player, object);
            if (m_anticheatSpike == object) return;
            if (object != nullptr)
                s_collisionObject = object;
        }
    };

    class $modify(ShowHitboxesPOHook, PlayerObject) {
        void playerDestroyed(bool p0) {
            PlayerObject::playerDestroyed(p0);
        }

        void collidedWithObject(float p0, GameObject* p1, cocos2d::CCRect p2, bool p3) {
            PlayerObject::collidedWithObject(p0, p1, p2, p3);
            s_collisionObject = p1;
        }
    };

    class $modify(ShowHitboxesLELHook, LevelEditorLayer) {
        bool init(GJGameLevel* level, bool unk) {
            if (!LevelEditorLayer::init(level, unk))
                return false;

            $hitbox->createDrawNode();
            return true;
        }

        void updateVisibility(float dt) override {
            LevelEditorLayer::updateVisibility(dt);
            $hitbox->visitHitboxes();
        }
    };

    class $modify(ShowHitboxesEditorHook, EditorUI) {
        void onPlaytest(CCObject* sender) {
            EditorUI::onPlaytest(sender);

            s_isDead = false;
            s_collisionObject = nullptr;

            // Don't clear trails - keep them for macrobuffing
        }
    };
}