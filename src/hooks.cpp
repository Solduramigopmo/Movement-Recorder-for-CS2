#include "../include/hooks.h"
#include "../include/recorder.h"
#include "../include/playback.h"
#include "../include/frame.h"
#include <polyhook2/Detour/x64Detour.hpp>
#include <polyhook2/PolyHookOs.hpp>
#include <Psapi.h>
#include <vector>
#include <string>

namespace Hooks {
    static Recorder g_recorder;
    static Playback g_playback;
    static bool keys_pressed[3] = { false, false, false };
    static bool initialized = false;
    void (*OriginalCreateMove)(__int64 pInput, unsigned int cmdNumber, __int64 pCmd) = nullptr;
    
    PLH::x64Detour* g_createMoveDetour = nullptr;

    void CreateMove(__int64 pInput, unsigned int cmdNumber, __int64 pCmd) {

        OriginalCreateMove(pInput, cmdNumber, pCmd);
        CUserCmd* command = reinterpret_cast<CUserCmd*>(pCmd);
        if (!command) return;

        const bool f1_pressed = GetAsyncKeyState(VK_F1) & 0x8000;
        const bool f2_pressed = GetAsyncKeyState(VK_F2) & 0x8000;
        const bool f3_pressed = GetAsyncKeyState(VK_F3) & 0x8000;

        // F1 - Toggle Recording
        if (f1_pressed && !keys_pressed[0]) {
            if (g_recorder.IsRecordingActive()) {
                g_recorder.StopRecording();
            } else {
                g_playback.StopPlayback();
                g_recorder.StartRecording();
            }
        }
        keys_pressed[0] = f1_pressed;

        // F2 - Start Playback
        if (f2_pressed && !keys_pressed[1]) {
            auto& recording = g_recorder.GetActiveRecording();
            if (!recording.empty()) {
                g_recorder.StopRecording();
                g_playback.StartPlayback(recording);
            }
        }
        keys_pressed[1] = f2_pressed;

        // F3 - Clear Recording
        if (f3_pressed && !keys_pressed[2]) {
            g_recorder.StopRecording();
            g_playback.StopPlayback();
            g_recorder.ClearRecording();
        }
        keys_pressed[2] = f3_pressed;



        const bool is_playback_active = g_playback.IsPlaybackActive();
        const bool is_recording_active = g_recorder.IsRecordingActive();
        if (!is_playback_active && !is_recording_active) {
            return;
        }

        FrameContainer& recording = g_recorder.GetActiveRecording();

        if (is_recording_active) {
            recording.emplace_back(nullptr, command);
        }

        if (is_playback_active) {
            const size_t current_playback_frame = g_playback.GetCurrentFrame();
            
            if (current_playback_frame < recording.size()) {
                const Frame& recorded_frame = recording[current_playback_frame];
                recorded_frame.Replay(nullptr, command);
                
                if (current_playback_frame + 1 == recording.size()) {
                    g_playback.StopPlayback();
                } else {
                    g_playback.SetCurrentFrame(current_playback_frame + 1);
                }
            } else {
                g_playback.StopPlayback();
            }
        }
    }

    uintptr_t FindSignature(const char* moduleName, const char* signature) {
        HMODULE module = GetModuleHandleA(moduleName);
        if (!module) return 0;

        MODULEINFO moduleInfo;
        if (!GetModuleInformation(GetCurrentProcess(), module, &moduleInfo, sizeof(moduleInfo)))
            return 0;

        uintptr_t moduleBase = reinterpret_cast<uintptr_t>(module);
        std::vector<int> pattern;
        std::string sig(signature);
        
        std::string token;
        for (size_t i = 0; i <= sig.length(); ++i) {
            if (i == sig.length() || sig[i] == ' ') {
                if (!token.empty()) {
                    if (token == "?") {
                        pattern.push_back(-1);
                    } else {
                        int byte = std::stoi(token, nullptr, 16);
                        pattern.push_back(byte);
                    }
                    token.clear();
                }
            } else {
                token += sig[i];
            }
        }

        for (uintptr_t i = moduleBase; i < moduleBase + moduleInfo.SizeOfImage - pattern.size(); ++i) {
            bool found = true;
            for (size_t j = 0; j < pattern.size(); ++j) {
                if (pattern[j] != -1 && pattern[j] != *reinterpret_cast<unsigned char*>(i + j)) {
                    found = false;
                    break;
                }
            }
            if (found) {
                return i;
            }
        }
        return 0;
    }

    void Initialize() {
        if (initialized) return;
        
        auto pattern = FindSignature("client.dll", "48 8B C4 4C 89 40 ? 48 89 48 ? 55 53 57");
        if (!pattern) return;
        g_createMoveDetour = new PLH::x64Detour(pattern, reinterpret_cast<uintptr_t>(&CreateMove), reinterpret_cast<uintptr_t*>(&OriginalCreateMove));
        
        if (g_createMoveDetour && g_createMoveDetour->hook()) {
            initialized = true;
        }
    }

    void Shutdown() {
        if (g_createMoveDetour) {
            g_createMoveDetour->unHook();
            delete g_createMoveDetour;
            g_createMoveDetour = nullptr;
        }
    }
}