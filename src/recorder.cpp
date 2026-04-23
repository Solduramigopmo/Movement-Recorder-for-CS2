#include "../include/recorder.h"

void Recorder::StartRecording() {
    is_recording_active = true;
}

void Recorder::StopRecording() {
    is_recording_active = false;
}

bool Recorder::IsRecordingActive() const {
    return is_recording_active;
}

FrameContainer& Recorder::GetActiveRecording() {
    return recording_frames;
}

void Recorder::ClearRecording() {
    recording_frames.clear();
}