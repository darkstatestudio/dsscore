// Copyright Dark State Studio.All rights reserved.

#include "Actor/Pool/ActorPool.h"

#include "GameFramework/Actor.h"
#include "Engine/World.h"

void UActorPool::Init(UWorld* InWorld, const FActorPoolConfig& InConfig)
{
	check(!IsInitialized());
	check(InWorld);
	check(InConfig.ActorClass);

	World = InWorld;
	Config = InConfig;

	const int32 Warmup = FMath::Min(Config.WarmupCount, Config.MaxSize);
	Free.Reserve(Warmup);
	Used.Reserve(Warmup);
	for (int32 i = 0; i < Warmup; ++i)
	{
		auto Actor = SpawnNewActor();
		if (Actor)
		{
			PrepareActorForPool(Actor);
			Free.Add(Actor);
		}
	}

	bInitialized = true;
}

AActor* UActorPool::Acquire()
{
	if (!bInitialized)
	{
		return nullptr;
	}

	AActor* Actor = nullptr;

	if (Free.Num() > 0)
	{
		Actor = Free.Pop(EAllowShrinking::No);
	}
	else if (Used.Num() < Config.MaxSize || Config.bDynamicGrow)
	{
		Used.Reserve(Used.Max() + Config.IncreaseCount);
		Actor = SpawnNewActor();
	}

	if (!Actor)
	{
		return nullptr;
	}

	PrepareActorForUse(Actor);
	Used.Add(Actor);

	return Actor;
}

void UActorPool::Release(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return;
	}
	if (!Used.Contains(Actor))
	{
		return;
	}

	Used.Remove(Actor);

	if (Free.Num() >= Config.MaxSize && !Config.bDynamicGrow)
	{
		Actor->Destroy();
		return;
	}

	PrepareActorForPool(Actor);
	Free.Add(Actor);
}

AActor* UActorPool::SpawnNewActor()
{
	if (!World)
	{
		return nullptr;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	return World->SpawnActor<AActor>(Config.ActorClass, FTransform::Identity, Params);
}

void UActorPool::PrepareActorForUse(AActor* Actor)
{
	Actor->SetActorHiddenInGame(false);
	Actor->SetActorEnableCollision(true);
	Actor->SetActorTickEnabled(false); // default OFF
}

void UActorPool::PrepareActorForPool(AActor* Actor)
{
	Actor->SetActorHiddenInGame(true);
	Actor->SetActorEnableCollision(false);
	Actor->SetActorTickEnabled(false);
	Actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void UActorPool::Shutdown()
{
	for (AActor* Actor : Free)
	{
		if (IsValid(Actor))
		{
			Actor->Destroy();
		}
	}

	for (AActor* Actor : Used)
	{
		if (IsValid(Actor))
		{
			Actor->Destroy();
		}
	}

	Free.Empty();
	Used.Empty();
}
