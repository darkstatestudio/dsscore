// Dark State Studio

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActorAcceptTarget.generated.h"

UINTERFACE(Blueprintable, BlueprintType)
class UActorAcceptTarget : public UInterface
{
	GENERATED_BODY()
};


class DSSCORE_API IActorAcceptTarget
{
	GENERATED_BODY()

public:

	/**
	 * Try to accept actor.
	 * Returns true if accept process has started.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool TryAcceptActor(AActor* Actor);

	/**
	 * Callback from acceptable actor when pre-accept phase is finished.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnActorPreAcceptDone(AActor* Actor);
};
