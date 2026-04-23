#pragma once

#include "common.h"
#include "frame.h"

class Playback {
private:
    bool is_playback_active = false;
    size_t current_frame = 0;
    FrameContainer* active_demo = nullptr;

public:
    void StartPlayback(FrameContainer& frames);
    void StopPlayback();
    bool IsPlaybackActive() const;
    
    size_t GetCurrentFrame() const;
    void SetCurrentFrame(size_t frame);
    
    FrameContainer* GetActiveDemo() const;
};