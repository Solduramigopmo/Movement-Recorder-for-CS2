#pragma once

#include <vector>
#include <stdexcept>
#include <Windows.h>
#include <cstdint>

namespace CS2Offsets {
    constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x18540D0;
    constexpr std::ptrdiff_t dwViewAngles = 0x1A733C0;
    constexpr std::ptrdiff_t m_vecOrigin = 0x88;
}

enum class InputFlags : std::uint64_t {
    Attack    = (1 << 0),
    Jump      = (1 << 1),
    Duck      = (1 << 2),
    Forward   = (1 << 3),
    Back      = (1 << 4),
    Use       = (1 << 5),
    Left      = (1 << 7),
    Right     = (1 << 8),
    MoveLeft  = (1 << 9),
    MoveRight = (1 << 10)
};

struct QAngle_t {
    float x{0.0f}, y{0.0f}, z{0.0f};
    
    QAngle_t() = default;
    QAngle_t(float x_val, float y_val, float z_val) : x(x_val), y(y_val), z(z_val) {}
};

struct Vector {
    float x{0.0f}, y{0.0f}, z{0.0f};
    
    Vector() = default;
    Vector(float x_val, float y_val, float z_val) : x(x_val), y(y_val), z(z_val) {}
};

class CCSGOInput 
{
public:
    char pad_0000[0x0298];
    float flForwardMove;
    float flSideMove;
    float flUpMove;
    char pad_02A4[0x12C];
    QAngle_t vecViewAngle;
    char pad_remaining[0x25C];
};

class CBasePB {
public:
    char pad_vtable[0x8];
    std::uint32_t nHasBits;
    std::uint64_t nCachedBits;
};

class CMsgQAngle : public CBasePB {
public:
    QAngle_t angValue;
};

struct CInButtonState {
public:
    char pad_vtable[0x8];
    std::uint64_t nValue;
    std::uint64_t nValueChanged; 
    std::uint64_t nValueScroll;
};

struct SimpleRepeatedField {
    void* pArena;
    int nCurrentSize;
    int nTotalSize;
    void* pRep;
};

struct CInButtonStatePB : public CBasePB {
    std::uint64_t nValue;
    std::uint64_t nValueChanged;
    std::uint64_t nValueScroll;
};

class CBaseUserCmdPB : public CBasePB {
public:
    SimpleRepeatedField subtickMovesField;
    std::string* strMoveCrc;
    CInButtonStatePB* pInButtonState;
    CMsgQAngle* pViewAngles;
    std::int32_t nLegacyCommandNumber;
    std::int32_t nClientTick;
    float flForwardMove;
    float flSideMove;
    float flUpMove;
    std::int32_t nImpulse;
    std::int32_t nWeaponSelect;
    std::int32_t nRandomSeed;
    std::int32_t nMousedX;
    std::int32_t nMousedY;
    std::uint32_t nConsumedServerAngleChanges;
    std::int32_t nCmdFlags;
    std::uint32_t nPawnEntityHandle;
};

class CCSGOUserCmdPB {
public:
    std::uint32_t nHasBits;
    std::uint64_t nCachedSize;
    SimpleRepeatedField inputHistoryField;
    CBaseUserCmdPB* pBaseCmd;
    bool bLeftHandDesired;
    bool bIsPredictingBodyShotFX;
    bool bIsPredictingHeadShotFX;
    bool bIsPredictingKillRagdolls;
    std::int32_t nAttack3StartHistoryIndex;
    std::int32_t nAttack1StartHistoryIndex;
    std::int32_t nAttack2StartHistoryIndex;
};

class CUserCmd {
public:
    char pad_vtable[0x8];
    char pad_unknown[0x10];
    CCSGOUserCmdPB csgoUserCmd;
    CInButtonState nButtons;
    char pad_middle[0x10];
    bool bHasBeenPredicted;
    char pad_end[0xF];
    
    float GetForwardMove() const {
        return csgoUserCmd.pBaseCmd ? csgoUserCmd.pBaseCmd->flForwardMove : 0.0f;
    }
    
    void SetForwardMove(float value) {
        if (csgoUserCmd.pBaseCmd) csgoUserCmd.pBaseCmd->flForwardMove = value;
    }
    
    float GetSideMove() const {
        return csgoUserCmd.pBaseCmd ? csgoUserCmd.pBaseCmd->flSideMove : 0.0f;
    }
    
    void SetSideMove(float value) {
        if (csgoUserCmd.pBaseCmd) csgoUserCmd.pBaseCmd->flSideMove = value;
    }
    
    float GetUpMove() const {
        return csgoUserCmd.pBaseCmd ? csgoUserCmd.pBaseCmd->flUpMove : 0.0f;
    }
    
    void SetUpMove(float value) {
        if (csgoUserCmd.pBaseCmd) csgoUserCmd.pBaseCmd->flUpMove = value;
    }
    
    std::uint64_t GetButtons() const {
        return nButtons.nValue;
    }
    
    void SetButtons(std::uint64_t btn) {
        nButtons.nValue = btn;
    }
    
    QAngle_t GetViewAngles() const {
        if (csgoUserCmd.pBaseCmd && csgoUserCmd.pBaseCmd->pViewAngles) {
            return csgoUserCmd.pBaseCmd->pViewAngles->angValue;
        }
        return QAngle_t();
    }
    
    void SetViewAngles(const QAngle_t& angles) {
        if (csgoUserCmd.pBaseCmd && csgoUserCmd.pBaseCmd->pViewAngles) {
            csgoUserCmd.pBaseCmd->pViewAngles->angValue = angles;
        }
    }
    
    int GetCommandNumber() const {
        return csgoUserCmd.pBaseCmd ? csgoUserCmd.pBaseCmd->nLegacyCommandNumber : 0;
    }
    
    std::int32_t GetImpulse() const {
        return csgoUserCmd.pBaseCmd ? csgoUserCmd.pBaseCmd->nImpulse : 0;
    }
    
    void SetImpulse(std::int32_t imp) {
        if (csgoUserCmd.pBaseCmd) csgoUserCmd.pBaseCmd->nImpulse = imp;
    }
    
    std::int32_t GetMousedX() const {
        return csgoUserCmd.pBaseCmd ? csgoUserCmd.pBaseCmd->nMousedX : 0;
    }
    
    void SetMousedX(std::int32_t mdx) {
        if (csgoUserCmd.pBaseCmd) csgoUserCmd.pBaseCmd->nMousedX = mdx;
    }
    
    std::int32_t GetMousedY() const {
        return csgoUserCmd.pBaseCmd ? csgoUserCmd.pBaseCmd->nMousedY : 0;
    }
    
    void SetMousedY(std::int32_t mdy) {
        if (csgoUserCmd.pBaseCmd) csgoUserCmd.pBaseCmd->nMousedY = mdy;
    }
};


inline uintptr_t GetClientDLLBase() {
    static uintptr_t clientBase = 0;
    if (!clientBase) {
        clientBase = reinterpret_cast<uintptr_t>(GetModuleHandleA("client.dll"));
    }
    return clientBase;
}

inline Vector GetPlayerPosition() {
    uintptr_t clientBase = GetClientDLLBase();
    if (!clientBase) return Vector();
    
    uintptr_t localPlayerPawn = *reinterpret_cast<uintptr_t*>(clientBase + CS2Offsets::dwLocalPlayerPawn);
    if (!localPlayerPawn) return Vector();
    
    float* origin = reinterpret_cast<float*>(localPlayerPawn + CS2Offsets::m_vecOrigin);
    return Vector(origin[0], origin[1], origin[2]);
}

inline void SetEngineViewAngles(const QAngle_t& angles) {
    uintptr_t clientBase = GetClientDLLBase();
    if (!clientBase) return;
    
    float* viewAngles = reinterpret_cast<float*>(clientBase + CS2Offsets::dwViewAngles);
    viewAngles[0] = angles.x;
    viewAngles[1] = angles.y;
    viewAngles[2] = angles.z;
}