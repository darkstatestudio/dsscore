// Dark State Studio


#include "Actor/Pool/ActorPoolsSubsystem.h"

void UActorPoolsSubsystem::Deinitialize()
{
	for (auto& It : Pools)
	{
		if (It.Value)
		{
			It.Value->Shutdown();
		}
	}
	Pools.Empty();

	Super::Deinitialize();
}

UActorPool* UActorPoolsSubsystem::CreatePool(const FActorPoolConfig& Config)
{
	if (Pools.Contains(Config.ActorClass))
	{
		return nullptr;
	}

	auto Pool = NewObject<UActorPool>(this);
	Pool->Init(GetWorld(), Config);

	Pools.Add(Config.ActorClass, Pool);
	return Pool;
}

AActor* UActorPoolsSubsystem::Acquire(TSubclassOf<AActor> ActorClass)
{
	if (!*ActorClass) return nullptr;
	UActorPool* Pool = GetPool(ActorClass);
	return Pool ? Pool->Acquire() : nullptr;
}

void UActorPoolsSubsystem::Release(AActor* Actor)
{
	if (!IsValid(Actor)) return;

	TSubclassOf<AActor> Class = Actor->GetClass();
	if (UActorPool** PoolPtr = Pools.Find(Class))
	{
		(*PoolPtr)->Release(Actor);
	}
	else
	{
		Actor->Destroy(); // fallback safety
	}
}

UActorPool* UActorPoolsSubsystem::GetPool(TSubclassOf<AActor> ActorClass)
{
	if (UActorPool** Existing = Pools.Find(ActorClass))
	{
		return *Existing;
	}

	FActorPoolConfig DefaultConfig;
	return CreatePool(DefaultConfig);
}
