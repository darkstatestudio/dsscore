#pragma once

#include "DssAutomationTestBase.h"

template <typename TDerived>
bool TFDssAutomationTestBase<TDerived>::RunTest(const FString& Parameters)
{
	if (RegisteredMethods.Num() == 0)
	{
		AddError("No test cases found");
		return false;
	}

	for (const auto& Pair : RegisteredMethods)
	{
		const MethodPtr Func = Pair.Value;

		static_cast<TDerived*>(this)->BeforeEach();

		(static_cast<TDerived*>(this)->*Func)();

		static_cast<TDerived*>(this)->AfterEach();
	}

	return true;
}

template <typename TDerived>
void TFDssAutomationTestBase<TDerived>::TestJsonHasExactKeys(const TSharedPtr<FJsonObject>& Json, const TArray<FString>& ExpectedKeys,
	const FString& Context)
{
	TestTrue(*FString::Printf(TEXT("%s should be valid"), *Context), Json.IsValid());

	if (!Json.IsValid())
	{
		return;
	}

	TArray<FString> ActualKeys;
	Json->Values.GetKeys(ActualKeys);

	TArray<FString> SortedActual = ActualKeys;
	TArray<FString> SortedExpected = ExpectedKeys;

	SortedActual.Sort();
	SortedExpected.Sort();

	TestEqual(*FString::Printf(TEXT("%s key count"), *Context), SortedActual.Num(), SortedExpected.Num());

	for (const FString& ExpectedKey : SortedExpected)
	{
		TestTrue(*FString::Printf(TEXT("%s should contain key '%s'"), *Context, *ExpectedKey), Json->HasField(ExpectedKey));
	}

	for (const FString& ActualKey : SortedActual)
	{
		TestTrue(*FString::Printf(TEXT("%s should not contain unexpected key '%s'"), *Context, *ActualKey), SortedExpected.Contains(ActualKey));
	}
}

template <typename TDerived>
void TFDssAutomationTestBase<TDerived>::CreateTestWorld(EWorldType::Type WorldType)
{
	if (TestWorldWrapper.CreateTestWorld(WorldType))
	{
		TestWorld = TestWorldWrapper.GetTestWorld();
	}
}

template <typename TDerived>
void TFDssAutomationTestBase<TDerived>::DestroyTestWorld(bool bForceGC)
{
	TestWorldWrapper.DestroyTestWorld(bForceGC);
	TestWorld = nullptr;
}

template <typename TDerived>
UWorld* TFDssAutomationTestBase<TDerived>::GetTestWorld()
{
	return TestWorld;
}
