#include "../include/frame.h"
#include <cstring>
#include <utility>

Frame::Frame() noexcept {
    std::memset(this, 0, sizeof(Frame));
}

Frame::Frame(CCSGOInput* input, CUserCmd* cmd) noexcept {
    std::memset(this, 0, sizeof(Frame));
    if (!cmd) return;
    
    buttons = cmd->GetButtons();
    
    if (cmd->csgoUserCmd.pBaseCmd) {
        const auto* base = cmd->csgoUserCmd.pBaseCmd;
        
        if (base->pViewAngles) {
            viewangles = base->pViewAngles->angValue;
        }
        
        position = GetPlayerPosition();
        forwardmove = base->flForwardMove;
        sidemove = base->flSideMove;
        upmove = base->flUpMove;
        impulse = base->nImpulse;
        mousedx = base->nMousedX;
        mousedy = base->nMousedY;
    }
}

void Frame::Replay(CCSGOInput* input, CUserCmd* cmd) const noexcept {
    if (!cmd) return;
    
    cmd->nButtons.nValue = buttons;
    
    if (cmd->csgoUserCmd.pBaseCmd) {
        auto* base = cmd->csgoUserCmd.pBaseCmd;
        
        if (base->pViewAngles) {
            base->pViewAngles->angValue = viewangles;
        }

        SetEngineViewAngles(viewangles);
        base->flForwardMove = forwardmove;
        base->flSideMove = sidemove;
        base->flUpMove = upmove;
        base->nImpulse = impulse;
        base->nMousedX = mousedx;
        base->nMousedY = mousedy;
    }
}

void Frame::Clear() noexcept {
    std::memset(this, 0, sizeof(Frame));
}

bool Frame::IsEmpty() const noexcept {
    return (buttons == 0 && forwardmove == 0.0f && sidemove == 0.0f && upmove == 0.0f);
}

Frame::Frame(const Frame& other) noexcept {
    std::memcpy(this, &other, sizeof(Frame));
}

Frame& Frame::operator=(const Frame& other) noexcept {
    if (this != &other) {
        std::memcpy(this, &other, sizeof(Frame));
    }
    return *this;
}

Frame::Frame(Frame&& other) noexcept {
    std::memcpy(this, &other, sizeof(Frame));
    other.Clear();
}

Frame& Frame::operator=(Frame&& other) noexcept {
    if (this != &other) {
        std::memcpy(this, &other, sizeof(Frame));
        other.Clear();
    }
    return *this;
}