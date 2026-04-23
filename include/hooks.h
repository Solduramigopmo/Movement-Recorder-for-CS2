#pragma once

#include "common.h"

namespace Hooks {
    void CreateMove(__int64 pInput, unsigned int cmdNumber, __int64 pCmd);
    
    void Initialize();
    void Shutdown();
    
    extern void (*OriginalCreateMove)(__int64 pInput, unsigned int cmdNumber, __int64 pCmd);
}