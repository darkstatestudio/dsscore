#pragma once
#include "Dom/JsonObject.h"

inline bool FDssAutomationTestBase::RunTest(const FString& Parameters)
{
	if (!HasAnyTest())
	{
		return false;
	}

	for (const auto& Pair : Tests)
	{
		const auto Func = Pair.Value;

		AddInfo(FString::Printf(TEXT("Test: %s"), *Pair.Key));
		BeforeEachCallback();
		Func();
		AfterEachCallback();
	}

	return true;
}

inline void FDssAutomationTestBase::TestJsonHasExactKeys(const TSharedPtr<FJsonObject>& Json, const TArray<FString>& ExpectedKeys,
                                                         const FString& Context)
{
	TestTrue(*FString::Printf(TEXT("%s should be valid"), *Context), Json.IsValid());

	if (!Json.IsValid())
	{
		return;
	}

	TArray<UE::FSharedString> ActualKeys;
	Json->Values.GetKeys(ActualKeys);

	TArray<UE::FSharedString> SortedActual = ActualKeys;
	TArray<FString> SortedExpected = ExpectedKeys;

	SortedActual.Sort();
	SortedExpected.Sort();

	TestEqual(*FString::Printf(TEXT("%s key count"), *Context), SortedActual.Num(), SortedExpected.Num());

	for (const auto& ExpectedKey : SortedExpected)
	{
		TestTrue(*FString::Printf(TEXT("%s should contain key '%s'"), *Context,*ExpectedKey), Json->HasField(*ExpectedKey));
	}

	for (const auto& ActualKey : SortedActual)
	{
		FString K = FString(ActualKey.ToView());
		TestTrue(*FString::Printf(TEXT("%s should not contain unexpected key '%s'"), *Context, *K), SortedExpected.Contains(K));
	}
}

inline void FDssAutomationTestBase::CreateTestWorld(EWorldType::Type WorldType)
{
	if (TestWorldWrapper.CreateTestWorld(WorldType))
	{
		TestWorld = TestWorldWrapper.GetTestWorld();
	}
}

inline void FDssAutomationTestBase::DestroyTestWorld(bool bForceGC)
{
	TestWorldWrapper.DestroyTestWorld(bForceGC);
	TestWorld = nullptr;
}

inline UWorld* FDssAutomationTestBase::GetTestWorld()
{
	return TestWorld;
}
