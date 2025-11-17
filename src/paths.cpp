#include "paths.h"
#include <Geode/loader/Dirs.hpp>

std::filesystem::path AstralPath() {
    return geode::dirs::getGameDir() / "Astral";
}

std::filesystem::path ffmpegDir() {
    return AstralPath() / "ffmpeg";
}

std::filesystem::path RendersDir() {
    return AstralPath() / "Renders";
}

std::filesystem::path MacrosDir() {
    return AstralPath() / "Macros";
}
std::filesystem::path LogsDir() {
    return AstralPath() / "Logs";
}
std::filesystem::path GuisDir() {
    return AstralPath() / "GUIs";
}
std::filesystem::path AutoclickerDir() {
    return AstralPath() / "Autoclicker";
}