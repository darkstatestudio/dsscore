//

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DssNetworkFunctionLibrary.generated.h"

UCLASS()
class DSSCORE_API UDssNetworkFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DSS|Network")
	static bool IsActorOnClient(const AActor* Actor)
	{
		ENetMode NetMode = Actor->GetNetMode();
		if (NetMode == NM_DedicatedServer || NetMode == NM_ListenServer)
		{
			return false;
		}

		return true;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DSS|Network")
	static bool IsActorOnListenServer(const AActor* Actor)
	{
		return Actor->GetNetMode() == NM_ListenServer;
	}

	UFUNCTION(BlueprintCallable, Category = "DSS|Network")
	static bool IsActorOnServer(const AActor* Actor)
	{
		return Actor->HasAuthority();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DSS|Network")
	static FString GetActorDebugId(const AActor* Actor)
	{
		auto NetModeInfo = TEXT("NONE");
		if (Actor->GetNetMode() != NM_Standalone)
		{
			NetModeInfo = IsActorOnClient(Actor) ? TEXT("CLIENT") : TEXT("SERVER");
		}

		return FString::Printf(TEXT("(%s) %s"), NetModeInfo, *Actor->GetName());
	}
};
