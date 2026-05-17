// Copyright Dark State Studio.All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "VoiceLine.generated.h"

UENUM(BlueprintType)
enum class EVoiceTone : uint8
{
	Neutral,
	Calm,
	Friendly,
	Curious,
	Playful,

	Focused,
	Informative,
	Warning,
	Urgent,
	Alarm,

	Error,
	Suspicious,
	Aggressive,
	Threatening,

	Sad,
	Angry,
	Excited,

	Whisper,
	Distorted,
	Glitched
};

UENUM(BlueprintType)
enum class EVoicePriority : uint8
{
	Low,
	Normal,
	High,
	Critical
};

USTRUCT(BlueprintType)
struct FVoiceLine : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName LineId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EVoiceTone Tone = EVoiceTone::Neutral;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EVoicePriority Priority = EVoicePriority::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, TSoftObjectPtr<USoundBase>> Sounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float VolumeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PitchMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bInterruptible = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bExclusive = false;
};
