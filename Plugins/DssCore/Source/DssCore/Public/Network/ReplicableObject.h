#pragma once
#include "CoreMinimal.h"
#include "DssNetworkFunctionLibrary.h"
#include "ReplicableObject.generated.h"

UCLASS(Blueprintable, meta = (ShowWorldContextPin), DefaultToInstanced)
class DSSCORE_API UReplicableObject : public UObject
{
	GENERATED_BODY()

public:
	UReplicableObject(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure)
	UWorld* GetOwnerWorld() const
	{
		return GetWorld();
	}

	/** Follow the Outer chain to get the  AActor  that 'Owns' this object */
	UFUNCTION(BlueprintPure, meta=(Keywords = "Actor Owning Parent"))
	AActor* GetOwner() const
	{
		return GetTypedOuter<AActor>();
	}

	/** Templated version of GetOwner(), will return nullptr if cast fails */
	template <class T>
	T* GetOwner() const
	{
		return GetTypedOuter<T>();
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		// Add any Blueprint properties
		// This is not required if you do not want the class to be "Blueprintable"
		if (const UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass()))
		{
			BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
		}
	}

	UFUNCTION(BlueprintCallable)
	void RegisterAsReplicatedSubObjectInOwner()
	{
		GetOwner()->AddReplicatedSubObject(this);
	}

	UFUNCTION(BlueprintCallable)
	void UnregisterAsReplicatedSubObjectInOwner()
	{
		GetOwner()->RemoveReplicatedSubObject(this);
	}

	UFUNCTION(BlueprintCallable)
	void MarkToDestroy()
	{
		GetOwner()->DestroyReplicatedSubObjectOnRemotePeers(this);
		MarkAsGarbage();
	}

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override
	{
		check(GetOuter() != nullptr);
		return GetOuter()->GetFunctionCallspace(Function, Stack);
	}

	// Call "Remote" (aka, RPC) functions through the actors NetDriver
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, struct FOutParmRec* OutParms, FFrame* Stack) override
	{
		check(!HasAnyFlags(RF_ClassDefaultObject));
		AActor* Owner = GetOwner();
		if (auto NetDriver = Owner->GetNetDriver())
		{
			NetDriver->ProcessRemoteFunction(Owner, Function, Parms, OutParms, Stack, this);
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintPure)
	bool HasAuthority() const
	{
		return GetOwner()->HasAuthority();
	}

	ENetMode GetNetMode() const
	{
		return GetOwner()->GetNetMode();
	}

	UFUNCTION(BlueprintPure)
	bool IsClient() const
	{
		return UDssNetworkFunctionLibrary::IsActorOnClient(GetOwner());
	}

	UFUNCTION(BlueprintPure)
	bool IsServer() const
	{
		return UDssNetworkFunctionLibrary::IsActorOnServer(GetOwner());
	}

	UFUNCTION(BlueprintPure)
	bool IsDedicatedServer() const
	{
		return GetNetMode() == NM_DedicatedServer;
	}

	FString GetDebugId() const
	{
		auto NetModeInfo = TEXT("NONE");
		auto OwnerName = FString(TEXT("NONE"));
		if (GetOuter())
		{
			auto Owner = GetOwner();
			if (Owner->GetNetMode() != NM_Standalone)
			{
				NetModeInfo = IsClient() ? TEXT("CLIENT") : TEXT("SERVER");
			}
			OwnerName = Owner->GetName();
		}
		return FString::Printf(TEXT("(%s) %s %s"), NetModeInfo, *OwnerName, *GetName());
	}
};
