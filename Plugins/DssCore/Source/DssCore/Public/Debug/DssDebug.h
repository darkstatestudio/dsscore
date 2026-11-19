
#pragma once

#include "CoreMinimal.h"
#include "DssDebug.generated.h"

/**
 * Generic debug visualization categories.
 * Channels determine WHAT to debug.
 * Flags determine HOW to visualize it.
 */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EDssDebugFlags : uint8
{
    None      = 0 UMETA(Hidden),
    Geometry  = 1 << 0, // Bounds, planes, collision shapes
    Positions = 1 << 1, // Points, anchors, sockets, targets
    Paths     = 1 << 2, // Routes, trajectories, connections
    Vectors   = 1 << 3, // Directions, velocities, forces
    Labels    = 1 << 4, // Names, identifiers, numeric values
    States    = 1 << 5, // Activation states, statuses, results
    Details   = 1 << 6  // Additional detailed diagnostics
};
ENUM_CLASS_FLAGS(EDssDebugFlags);

/**
 * Global, game-independent debug visualization manager.
 *
 * All operations are intended for the game thread.
 * Debug functionality is disabled when ENABLE_DRAW_DEBUG is false.
 *
 * Console:
 *   dss.Debug +Geometry +Paths
 *   dss.Debug +Teth.ScanGates
 *   dss.Debug -Teth.Rails
 *   dss.Debug List
 *   dss.Debug All
 *   dss.Debug None
 */
class DSSCORE_API FDssDebug
{
public:

	static void Initialize();
	static void Shutdown();

    // Channel registration
    static void RegisterChannel(const FName& Channel);
    static void UnregisterChannel(const FName& Channel);

    // Queries
    static bool IsEnabled(const FName& Channel, EDssDebugFlags Flags);
    static bool IsEnabled(EDssDebugFlags Flags);
    static bool IsChannelEnabled(const FName& Channel);

    // Global visualization flags
    static EDssDebugFlags GetFlags();
    static void SetFlags(EDssDebugFlags Flags);
    static void EnableFlags(EDssDebugFlags Flags);
    static void DisableFlags(EDssDebugFlags Flags);

    // Channels
    static void EnableChannel(const FName& Channel);
    static void DisableChannel(const FName& Channel);

    // Global controls
    static void EnableAll();
    static void Reset();
    static void List();

private:

	static void ExecuteCommand(const TArray<FString>& Args);
};
