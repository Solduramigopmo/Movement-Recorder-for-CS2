#pragma once

#include "common.h"
#include "frame.h"

class Recorder {
private:
    bool is_recording_active = false;
    FrameContainer recording_frames;

public:
    void StartRecording();
    void StopRecording();
    bool IsRecordingActive() const;
    
    FrameContainer& GetActiveRecording();
    
    void ClearRecording();
};