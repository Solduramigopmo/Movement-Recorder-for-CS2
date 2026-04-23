#include "../include/playback.h"

void Playback::StartPlayback(FrameContainer& frames) {
    is_playback_active = true;
    active_demo = &frames;
    current_frame = 0;
}

void Playback::StopPlayback() {
    is_playback_active = false;
    current_frame = 0;
    active_demo = nullptr;
}

bool Playback::IsPlaybackActive() const {
    return is_playback_active;
}

size_t Playback::GetCurrentFrame() const {
    return current_frame;
}

void Playback::SetCurrentFrame(size_t frame) {
    current_frame = frame;
}

FrameContainer* Playback::GetActiveDemo() const {
    return active_demo;
}