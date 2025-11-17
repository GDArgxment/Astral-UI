#include "renderer.hpp"
#include "paths.h"
#include "vars.h"
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "includes.hpp"

using namespace geode::prelude;

void Astral::Renderer::InstallFFmpeg() {
    std::filesystem::path FFmpegPath = ffmpegDir() / "ffmpeg.exe";
    
    // now logs if ffmpeg existss
    if (std::filesystem::exists(FFmpegPath)) {
        AstralLogger::log("FFmpeg found at: {}", FFmpegPath.string());
        return;
    }
    
    // if ffmpeg doesn't exist, download it
    AstralLogger::log("FFmpeg not found, downloading...");
    
    web::WebRequest()
        .get("https://github.com/GMDPLUTONIC/test/releases/download/hii/ffmpeg.exe")
        .listen([FFmpegPath](auto res) {
            if (!res) {
                AstralLogger::log("Failed to download FFmpeg: network error");
                return;
            }
            auto intoRes = res->into(FFmpegPath);
            if (!intoRes) {
                AstralLogger::log("Failed to download FFmpeg: {}", intoRes.unwrapErr());
            } else {
                AstralLogger::log("FFmpeg downloaded successfully to: {}", FFmpegPath.string());
            }
        });
}

void Astral::Renderer::Rendering() {
// add functionality later
}

void Astral::Renderer::SaveRender() {
    std::ofstream file(RendersDir() / "hi.mp4");
    if (!file.is_open()) {
        AstralLogger::log("Failed to write render file");
        return;
    }
}