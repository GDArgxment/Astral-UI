#pragma once

#include <filesystem>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <cocos2d.h>

using u8 = uint8_t;

namespace Astral::Renderer {
    // OpenGL Framebuffer Object for rendering
    class RenderTexture {
    public:
        unsigned m_width, m_height;
        
        RenderTexture();
        ~RenderTexture();
        
        void begin();  // Bind FBO
        void capture(std::mutex& lock, std::vector<u8>& data, volatile bool& has_data);
        void end();    // Cleanup
        
    private:
        int m_old_fbo, m_old_rbo;
        unsigned m_fbo;
        cocos2d::CCTexture2D* m_texture;
    };

    // Main recorder class
    class Recorder {
    public:
        Recorder();
        ~Recorder();
        
        struct Settings {
            int width = 1920;
            int height = 1080;
            int fps = 60;
            int bitrate = 30;  // Mbps
            std::string codec = "libx264";
            std::string outputPath;
        };
        
        void start(const Settings& settings);
        void stop();
        void captureFrame();
        bool isRecording() const { return m_recording; }
        
    private:
        void encodeThread();
        std::string buildFFmpegCommand() const;
        
        RenderTexture m_renderer;
        Settings m_settings;
        
        std::vector<u8> m_current_frame;
        volatile bool m_frame_has_data = false;
        std::mutex m_frame_lock;
        
        bool m_recording = false;
        std::thread m_encode_thread;
    };

    // Public API
    void InstallFFmpeg();
    void Rendering();
    void SaveRender();
    
    // Global recorder instance
    extern Recorder g_recorder;
    
    // Frame timing for gameplay recording
    inline double g_total_time = 0.0;
    inline double g_last_frame_time = 0.0;
    inline double g_extra_time = 0.0;
}
