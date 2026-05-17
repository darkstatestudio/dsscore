#pragma once
#include "CoreMinimal.h"

struct DSSCORE_API FWeightedValueBase
{
	float Weight = 1.0f;

	FWeightedValueBase() {};
	FWeightedValueBase(const float InWeight): Weight(InWeight) {}
};

template<typename T>
struct TFWeightedValue : FWeightedValueBase
{
	T Value;

	TFWeightedValue() {};
	TFWeightedValue(const T& InValue, const float InWeight): FWeightedValueBase(InWeight), Value(InValue) {}
};

template<typename T>
concept InheritsFromFWeightedEntry = std::is_base_of_v<FWeightedValueBase, T>;

template<InheritsFromFWeightedEntry T>
struct TFWeightedDictionary
{
	void Add(const T& Entry)
	{
		Entries.Add(Entry);
		CachedTotalWeight = -1.f;
	}

	void RefreshTotalWeight()
	{
		CachedTotalWeight = 0.f;
		for (auto& E : Entries)
			CachedTotalWeight += E.Weight;
	}

	const T& GetRandom(const FRandomStream& Stream) const
	{
		if (Entries.Num() == 0) return EmptyEntry;

		float Pick = Stream.FRandRange(0, CachedTotalWeight);
		for (auto& E : Entries)
		{
			Pick -= E.Weight;
			if (Pick <= 0)
			{
				return E;
			}
		}
		return Entries.Last();
	}

private:
	float CachedTotalWeight = -1.f;
	TArray<T> Entries;
	T EmptyEntry;
};

using FWeightedStringDictionary = TFWeightedDictionary<TFWeightedValue<FString>>;
using FWeightedIntDictionary = TFWeightedDictionary<TFWeightedValue<int32>>;
