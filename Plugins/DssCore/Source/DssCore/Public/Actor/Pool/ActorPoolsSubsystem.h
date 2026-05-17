// Dark State Studio

#pragma once

#include "CoreMinimal.h"
#include "ActorPool.h"
#include "Subsystems/WorldSubsystem.h"
#include "ActorPoolsSubsystem.generated.h"

UCLASS(BlueprintType)
class DSSCORE_API UActorPoolsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	UActorPool* CreatePool(const FActorPoolConfig& Config);

	UFUNCTION(BlueprintCallable)
	AActor* Acquire(TSubclassOf<AActor> ActorClass);

	UFUNCTION(BlueprintCallable)
	void Release(AActor* Actor);

private:
	UActorPool* GetPool(TSubclassOf<AActor> ActorClass);

	UPROPERTY(Transient)
	TMap<TSubclassOf<AActor>, UActorPool*> Pools;
};
