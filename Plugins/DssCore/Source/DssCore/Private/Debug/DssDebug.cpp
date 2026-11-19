
#include "Debug/DssDebug.h"
#include "HAL/IConsoleManager.h"
#include "UObject/Class.h"

#if ENABLE_DRAW_DEBUG

namespace
{
    constexpr uint8 AllDebugFlags = (1 << 7) - 1;

    struct FDssDebugState
    {
        uint8 Flags = 0;
        TSet<FName> RegisteredChannels;
        TSet<FName> EnabledChannels;
    };

    FDssDebugState& GetDebugState()
    {
        static FDssDebugState State;
        return State;
    }

    bool TryParseFlag(const FString& Name, EDssDebugFlags& OutFlag)
    {
        const UEnum* Enum = StaticEnum<EDssDebugFlags>();

        for (int32 Index = 0; Index < Enum->NumEnums(); ++Index)
        {
            const FString EnumName = Enum->GetNameStringByIndex(Index);

            if (EnumName.Equals(Name, ESearchCase::IgnoreCase))
            {
                OutFlag = static_cast<EDssDebugFlags>(Enum->GetValueByIndex(Index));
                return true;
            }
        }

        return false;
    }

	IConsoleObject* DebugConsoleCommand = nullptr;
}

#endif

// -----------------------------------------------------------------------------
// Channels
// -----------------------------------------------------------------------------

void FDssDebug::Initialize()
{
#if ENABLE_DRAW_DEBUG
	if (DebugConsoleCommand) return;

	DebugConsoleCommand = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("dss.Debug"),
		TEXT("Manage debug visualization channels and flags"),
		FConsoleCommandWithArgsDelegate::CreateStatic(&FDssDebug::ExecuteCommand),
		ECVF_Default
	);
#endif
}

void FDssDebug::Shutdown()
{
#if ENABLE_DRAW_DEBUG
	if (!DebugConsoleCommand) return;

	IConsoleManager::Get().UnregisterConsoleObject(DebugConsoleCommand);
	DebugConsoleCommand = nullptr;
#endif
}

void FDssDebug::RegisterChannel(const FName& Channel)
{
#if ENABLE_DRAW_DEBUG
    if (!Channel.IsNone())
    {
        GetDebugState().RegisteredChannels.Add(Channel);
    }
#endif
}

void FDssDebug::UnregisterChannel(const FName& Channel)
{
#if ENABLE_DRAW_DEBUG
    FDssDebugState& State = GetDebugState();
    State.RegisteredChannels.Remove(Channel);
    State.EnabledChannels.Remove(Channel);
#endif
}

void FDssDebug::EnableChannel(const FName& Channel)
{
#if ENABLE_DRAW_DEBUG
    FDssDebugState& State = GetDebugState();

    if (State.RegisteredChannels.Contains(Channel))
    {
        State.EnabledChannels.Add(Channel);
    }
#endif
}

void FDssDebug::DisableChannel(const FName& Channel)
{
#if ENABLE_DRAW_DEBUG
    GetDebugState().EnabledChannels.Remove(Channel);
#endif
}

// -----------------------------------------------------------------------------
// Queries
// -----------------------------------------------------------------------------

bool FDssDebug::IsEnabled(const FName& Channel, EDssDebugFlags Flags)
{
#if ENABLE_DRAW_DEBUG
    const FDssDebugState& State = GetDebugState();

    return State.EnabledChannels.Contains(Channel) &&
           (State.Flags & static_cast<uint8>(Flags)) != 0;
#else
    return false;
#endif
}

bool FDssDebug::IsEnabled(EDssDebugFlags Flags)
{
#if ENABLE_DRAW_DEBUG
    return (GetDebugState().Flags & static_cast<uint8>(Flags)) != 0;
#else
    return false;
#endif
}

bool FDssDebug::IsChannelEnabled(const FName& Channel)
{
#if ENABLE_DRAW_DEBUG
    return GetDebugState().EnabledChannels.Contains(Channel);
#else
    return false;
#endif
}

// -----------------------------------------------------------------------------
// Flags
// -----------------------------------------------------------------------------

EDssDebugFlags FDssDebug::GetFlags()
{
#if ENABLE_DRAW_DEBUG
    return static_cast<EDssDebugFlags>(GetDebugState().Flags);
#else
    return EDssDebugFlags::None;
#endif
}

void FDssDebug::SetFlags(EDssDebugFlags Flags)
{
#if ENABLE_DRAW_DEBUG
    GetDebugState().Flags = static_cast<uint8>(Flags) & AllDebugFlags;
#endif
}

void FDssDebug::EnableFlags(EDssDebugFlags Flags)
{
#if ENABLE_DRAW_DEBUG
    GetDebugState().Flags |= static_cast<uint8>(Flags) & AllDebugFlags;
#endif
}

void FDssDebug::DisableFlags(EDssDebugFlags Flags)
{
#if ENABLE_DRAW_DEBUG
    GetDebugState().Flags &= ~static_cast<uint8>(Flags);
#endif
}

// -----------------------------------------------------------------------------
// Global controls
// -----------------------------------------------------------------------------

void FDssDebug::EnableAll()
{
#if ENABLE_DRAW_DEBUG
    FDssDebugState& State = GetDebugState();

    State.Flags = AllDebugFlags;
    State.EnabledChannels = State.RegisteredChannels;
#endif
}

void FDssDebug::Reset()
{
#if ENABLE_DRAW_DEBUG
    FDssDebugState& State = GetDebugState();

    State.Flags = 0;
    State.EnabledChannels.Reset();
#endif
}

void FDssDebug::List()
{
#if ENABLE_DRAW_DEBUG
    const FDssDebugState& State = GetDebugState();
    const UEnum* Enum = StaticEnum<EDssDebugFlags>();

    UE_LOG(LogTemp, Display, TEXT("========== DSS DEBUG =========="));

    UE_LOG(LogTemp, Display, TEXT("Flags:"));

    for (int32 Index = 0; Index < Enum->NumEnums(); ++Index)
    {
        const uint8 Value = static_cast<uint8>(Enum->GetValueByIndex(Index));

        if (Value == 0 || (Value & AllDebugFlags) == 0) continue;

        const bool bEnabled = (State.Flags & Value) != 0;

        UE_LOG(LogTemp, Display, TEXT("  [%s] %s"),
            bEnabled ? TEXT("X") : TEXT(" "),
            *Enum->GetNameStringByIndex(Index));
    }

    UE_LOG(LogTemp, Display, TEXT("Channels:"));

    TArray<FName> Channels = State.RegisteredChannels.Array();
    Channels.Sort(FNameLexicalLess());

    for (const FName Channel : Channels)
    {
        const bool bEnabled = State.EnabledChannels.Contains(Channel);

        UE_LOG(LogTemp, Display, TEXT("  [%s] %s"),
            bEnabled ? TEXT("X") : TEXT(" "),
            *Channel.ToString());
    }

    UE_LOG(LogTemp, Display, TEXT("==============================="));
#endif
}

void FDssDebug::ExecuteCommand(const TArray<FString>& Args)
{
#if ENABLE_DRAW_DEBUG
	if (Args.IsEmpty())
	{
		FDssDebug::List();
		return;
	}

	for (const FString& Arg : Args)
	{
		if (Arg.Equals(TEXT("List"), ESearchCase::IgnoreCase))
		{
			FDssDebug::List();
			continue;
		}

		if (Arg.Equals(TEXT("All"), ESearchCase::IgnoreCase))
		{
			FDssDebug::EnableAll();
			continue;
		}

		if (Arg.Equals(TEXT("None"), ESearchCase::IgnoreCase))
		{
			FDssDebug::Reset();
			continue;
		}

		const bool bDisable = Arg.StartsWith(TEXT("-"));
		const bool bPrefixed = bDisable || Arg.StartsWith(TEXT("+"));
		const FString Name = bPrefixed ? Arg.RightChop(1) : Arg;

		if (Name.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("[DssDebug] Empty debug name"));
			continue;
		}

		EDssDebugFlags Flag;

		if (TryParseFlag(Name, Flag))
		{
			if (bDisable) FDssDebug::DisableFlags(Flag);
			else FDssDebug::EnableFlags(Flag);

			continue;
		}

		const FName Channel(*Name);
		const FDssDebugState& State = GetDebugState();

		if (State.RegisteredChannels.Contains(Channel))
		{
			if (bDisable) FDssDebug::DisableChannel(Channel);
			else FDssDebug::EnableChannel(Channel);

			continue;
		}

		UE_LOG(LogTemp, Warning, TEXT("[DssDebug] Unknown flag or channel: %s"), *Name);
	}
#endif
}
