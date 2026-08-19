#include <windows.h>
#include <cstdint>

// Memory addresses l-GTA San Andreas v1.0 US
#define GAME_STATE_ADDR      0xC8D4C0
#define HANDLING_ARRAY_ADDR  0xC1B594
#define HANDLING_ENTRY_SIZE  0xE0

// Handling Structure f-memory mta3 GTA SA
struct tHandlingData {
    int32_t   m_nVehicleId;
    float     m_fMass;
    float     m_fTurnMass;
    float     m_fDragMult;
    float     m_fCentreOfMassX;
    float     m_fCentreOfMassY;
    float     m_fCentreOfMassZ;
    uint8_t   m_nPercentSubmerged;
    float     m_fTractionMultiplier;
    float     m_fTractionLoss;
    float     m_fTractionBias;
    uint32_t  m_nNumberOfGears;
    float     m_fMaxVelocity;        // Speed Max (Default Infernus: ~240.0)
    float     m_fEngineAcceleration; // Acceleration (Default Infernus: ~28.0)
    float     m_fEngineInertia;
    uint8_t   m_nDriveType;
    uint8_t   m_nEngineType;
};

// Function bech t-jeb handling entry mta3 karhaba b-ID mta3ha f-handling.cfg
tHandlingData* GetHandlingData(int handlingId) {
    uintptr_t handlingBase = *reinterpret_cast<uintptr_t*>(HANDLING_ARRAY_ADDR);
    if (!handlingBase) return nullptr;
    
    return reinterpret_cast<tHandlingData*>(handlingBase + (handlingId * HANDLING_ENTRY_SIZE));
}

// Function bech t-baddel el-values f-memory b-sécurité
void ModifyVehicleHandling(int handlingId, float maxSpeed, float acceleration, float traction) {
    tHandlingData* handling = GetHandlingData(handlingId);
    if (!handling) return;

    DWORD oldProtect;
    VirtualProtect(handling, sizeof(tHandlingData), PAGE_EXECUTE_READWRITE, &oldProtect);

    handling->m_fMaxVelocity = maxSpeed;
    handling->m_fEngineAcceleration = acceleration;
    handling->m_fTractionMultiplier = traction;

    VirtualProtect(handling, sizeof(tHandlingData), oldProtect, &oldProtect);
}

// Main logic
DWORD WINAPI MainThread(LPVOID lpParam) {
    // N-estannaw el-game hta y-loadi complètement
    while (*reinterpret_cast<uint32_t*>(GAME_STATE_ADDR) != 9) {
        Sleep(500);
    }

    Sleep(2000); // 2 seconds safety delay

    // Exemples mta3 modifications:
    // Handling IDs: 3 = INFERNUS, 13 = ELEGY, 14 = SULTAN
    ModifyVehicleHandling(3, 300.0f, 45.0f, 1.3f); // Boost Infernus
    ModifyVehicleHandling(13, 260.0f, 38.0f, 1.4f); // Boost Elegy

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, MainThread, NULL, 0, NULL);
    }
    return TRUE;
}