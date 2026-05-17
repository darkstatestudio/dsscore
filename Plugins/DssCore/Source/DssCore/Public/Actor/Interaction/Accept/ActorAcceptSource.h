// Dark State Studio

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActorAcceptSource.generated.h"


UINTERFACE(Blueprintable, BlueprintType)
class UActorAcceptSource : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class DSSCORE_API IActorAcceptSource
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnBeginOverlapOnActorAcceptTarget(AActor* AcceptingActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnEndOverlapOnActorAcceptTarget(AActor* AcceptingActor);

	/**
	 * Called when accepting actor starts accepting this actor.
	 * Acceptable actor may delay and MUST call
	 * Acceptor->OnActorPreAcceptReady(this)
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnPreAccept(AActor* AcceptingActor);

	/**
	 * Called after actor has been fully accepted and attached.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnAccepted(AActor* AcceptingActor);



};
