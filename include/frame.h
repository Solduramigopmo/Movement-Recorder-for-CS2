#pragma once

#include "common.h"
#include <vector>

class Frame {
public:
    QAngle_t viewangles;
    Vector position;
    float forwardmove;
    float sidemove;
    float upmove;
    std::uint64_t buttons;
    std::int32_t impulse;
    std::int32_t mousedx;
    std::int32_t mousedy;

    Frame() noexcept;
    Frame(CCSGOInput* input, CUserCmd* cmd) noexcept;
    void Replay(CCSGOInput* input, CUserCmd* cmd) const noexcept;
    void Clear() noexcept;
    bool IsEmpty() const noexcept;
    Frame(const Frame& other) noexcept;
    Frame& operator=(const Frame& other) noexcept;
    Frame(Frame&& other) noexcept;
    Frame& operator=(Frame&& other) noexcept;
};

using FrameContainer = std::vector<Frame>;