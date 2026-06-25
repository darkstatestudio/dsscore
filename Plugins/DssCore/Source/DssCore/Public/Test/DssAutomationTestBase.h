// Copyright Dark State Studio.All rights reserved.
#pragma once

#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"

template <typename TDerived>
class TFDssAutomationTestBase : public FAutomationTestBase
{
public:
	using MethodPtr = void (TDerived::*)();

	explicit TFDssAutomationTestBase(const FString& InName) : FAutomationTestBase(InName, false)
	{
	}

	void RegisterTest(const FString& Name, MethodPtr Ptr)
	{
		RegisteredMethods.Add(Name, Ptr);
	}

	virtual void BeforeEach()
	{
	}

	virtual void AfterEach()
	{
	}

protected:
	virtual bool RunTest(const FString& Parameters) override;
	void TestJsonHasExactKeys(const TSharedPtr<FJsonObject>& Json,const TArray<FString>& ExpectedKeys,const FString& Context);

	void CreateTestWorld(EWorldType::Type WorldType = EWorldType::Editor);
	void DestroyTestWorld(bool bForceGC = false);
	UWorld* GetTestWorld();

	FTestWorldWrapper TestWorldWrapper;
	UWorld* TestWorld = nullptr;

private:
	TMap<FString, MethodPtr> RegisteredMethods;
};

#include "DssAutomationTestBase.inl"

