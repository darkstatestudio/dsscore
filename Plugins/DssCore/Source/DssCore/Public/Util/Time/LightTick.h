// Copyright Dark State Studio.All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "LightTick.generated.h"

UENUM(BlueprintType)
enum class EDssLightTickMode : uint8
{
	Disabled,
	EveryFrame,
	FixedRate
};

USTRUCT(BlueprintType)
struct DSSCORE_API FDssLightTickSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDssLightTickMode Mode = EDssLightTickMode::Disabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UpdateRateHz = 30.f;
};

USTRUCT()
struct DSSCORE_API FDssLightTickRuntime
{
	GENERATED_BODY()

	FORCEINLINE bool ShouldTick(const FDssLightTickSettings& Settings, const float DeltaTime)
	{
		switch (Settings.Mode)
		{
		case EDssLightTickMode::Disabled:
			return false;

		case EDssLightTickMode::EveryFrame:
			return true;

		case EDssLightTickMode::FixedRate:
			{
				constexpr float MaxAccumulationTime = 1.0f;
				const float Interval = 1.f / FMath::Max(Settings.UpdateRateHz, 1.f);
				Accumulator += DeltaTime;
				Accumulator = FMath::Min(Accumulator + DeltaTime, MaxAccumulationTime);
				if (Accumulator < Interval) { return false; }

				Accumulator = FMath::Fmod(Accumulator, Interval);
				return true;
			}
		}

		return false;
	}

	FORCEINLINE void Reset()
	{
		Accumulator = 0.f;
	}

	FORCEINLINE float GetAccumulator() const { return Accumulator; }

private:
	UPROPERTY(Transient)
	float Accumulator = 0.f;
};
