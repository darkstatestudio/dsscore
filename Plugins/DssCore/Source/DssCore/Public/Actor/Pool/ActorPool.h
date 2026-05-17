// Dark State Studio

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ActorPool.generated.h"

USTRUCT(BlueprintType)
struct FActorPoolConfig
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<AActor> ActorClass;

	UPROPERTY()
	int32 MaxSize = 4;

	UPROPERTY()
	int32 WarmupCount = 2;

	UPROPERTY()
	bool bDynamicGrow = false;
};

UCLASS(BlueprintType)
class UActorPool : public UObject
{
	GENERATED_BODY()

public:
	void Init(UWorld* InWorld, const FActorPoolConfig& InConfig);

	void Shutdown();

	AActor* Acquire();
	void Release(AActor* Actor);

	int32 GetFreeCount() const { return Free.Num(); }
	int32 GetUsedCount() const { return Used.Num(); }

	bool IsInitialized() const
	{
		return bInitialized;
	}

protected:
	AActor* SpawnNewActor();
	void PrepareActorForUse(AActor* Actor);
	void PrepareActorForPool(AActor* Actor);

private:
	UPROPERTY(Transient)
	bool bInitialized = false;

	UPROPERTY(Transient)
	UWorld* World;

	UPROPERTY(Transient)
	FActorPoolConfig Config;

	UPROPERTY(Transient)
	TArray<AActor*> Free;

	UPROPERTY(Transient)
	TSet<AActor*> Used;
};
