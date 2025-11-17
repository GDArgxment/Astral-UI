#include "includes.hpp"
#include "PracticeFix.hpp"

using namespace geode::prelude;

class CheckpointData {
public:
    CheckpointData() = default;

    CheckpointData(PlayerObject* player1, PlayerObject* player2, PlayLayer* playLayer) {
        m_checkpointPlayLayer = astral::utils::FixPlayLayerCheckpoint(playLayer);
        m_checkpointPlayer1 = astral::utils::FixPlayerCheckpoint(player1);
        if (player2)
            m_checkpointPlayer2 = astral::utils::FixPlayerCheckpoint(player2);
        
        m_tpsValue = tpsValue;
        m_frameCount = frameCount;
        m_seedValue = seedValue;
        m_initialized = initialized;
        m_lastLevelTime = lastLevelTime;
        m_lastProgress = lastProgress;
        
        AstralLogger::log("Checkpoint CREATED - TPS: {}, FrameCount: {} (adjusted from {})", m_tpsValue, m_seedValue, m_frameCount, frameCount);
    }

    void apply(PlayerObject* player1, PlayerObject* player2, PlayLayer* playLayer) {
        m_checkpointPlayLayer.apply(playLayer);
        m_checkpointPlayer1.apply(player1);
        if (player2)
            m_checkpointPlayer2.apply(player2);
        
        // Restore TPS and frame count to global variables
        tpsValue = m_tpsValue;
        frameCount = m_frameCount;
        seedValue = m_seedValue;
        
        // IMPORTANT: Also restore the frame counting state variables
        initialized = m_initialized;
        lastLevelTime = m_lastLevelTime;
        lastProgress = m_lastProgress;
        
        AstralLogger::log("Checkpoint loaded - TPS: {}, FrameCount: {}", m_tpsValue, m_seedValue, m_frameCount);
    }

private:
    astral::utils::FixPlayerCheckpoint m_checkpointPlayer1;
    astral::utils::FixPlayerCheckpoint m_checkpointPlayer2;
    astral::utils::FixPlayLayerCheckpoint m_checkpointPlayLayer;
    
    // Checkpoint values
    float m_tpsValue = 0.0f;
    float m_frameCount = 0.0f;
    float m_seedValue = 0.0f;
    
    // IMPORTANT: Also save the frame counting state
    bool m_initialized = false;
    float m_lastLevelTime = 0.0f;
    float m_lastProgress = 0.0f;
};

class $modify(FixPlayLayer, PlayLayer) {
    struct Fields {
        std::unordered_map<CheckpointObject*, CheckpointData> m_checkpoints;
    };

    void resetLevel() {
        if (m_checkpointArray->count() <= 0)
            m_fields->m_checkpoints.clear();

        PlayLayer::resetLevel();
        AstralLogger::log("resetLevel called via checkpoints");

    }

    void loadFromCheckpoint(CheckpointObject* checkpoint) {
        auto fields = m_fields.self();
        if (fields->m_checkpoints.contains(checkpoint)) {
            PlayLayer::loadFromCheckpoint(checkpoint);

            CheckpointData& data = fields->m_checkpoints[checkpoint];
            data.apply(m_player1, m_gameState.m_isDualMode ? m_player2 : nullptr, this);

            return;
        }

        PlayLayer::loadFromCheckpoint(checkpoint);
        AstralLogger::log("loadFromCheckpoint called");
    }

    CheckpointObject* createCheckpoint() {
        auto checkpoint = PlayLayer::createCheckpoint();
        if (!checkpoint)
            return checkpoint;

        if (m_gameState.m_currentProgress > 0) {
            CheckpointData data(m_player1, m_gameState.m_isDualMode ? m_player2 : nullptr, this);
            m_fields->m_checkpoints[checkpoint] = std::move(data);
        }

        return checkpoint;
        AstralLogger::log("createCheckpoint called");
    }

    void removeCheckpoint(bool first) {
        CheckpointObject* checkpoint = nullptr;
        if (m_checkpointArray->count()) {
            if (first) checkpoint = static_cast<CheckpointObject*>(m_checkpointArray->objectAtIndex(0));
            else checkpoint = static_cast<CheckpointObject*>(m_checkpointArray->lastObject());
        }

        auto fields = m_fields.self();
        if (checkpoint && fields->m_checkpoints.contains(checkpoint)) {
            fields->m_checkpoints.erase(checkpoint);
        }

        PlayLayer::removeCheckpoint(first);
        AstralLogger::log("removeCheckpoint called");
    }

    #ifdef GEODE_IS_DESKTOP
    void storeCheckpoint(CheckpointObject* checkpoint) {
        auto oldCheckpoint = static_cast<CheckpointObject*>(m_checkpointArray->objectAtIndex(0));

        PlayLayer::storeCheckpoint(checkpoint);

        auto fields = m_fields.self();
        if (!m_checkpointArray->containsObject(oldCheckpoint) && fields->m_checkpoints.contains(oldCheckpoint)) {
            fields->m_checkpoints.erase(oldCheckpoint);
        }
    }
    #endif
};