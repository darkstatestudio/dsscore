// Copyright Dark State Studio.All rights reserved.
#pragma once

#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"

class FDssAutomationTestBase : public FAutomationTestBase
{
public:
	using FTestFunctionPtr = TFunction<void()>;

	explicit FDssAutomationTestBase(const FString& InName) : FAutomationTestBase(InName, false)
	{
		BeforeEachCallback = [this]() {};
		AfterEachCallback = [this]() {};
	}

protected:
	void BeforeEach(const TFunction<void()>& F)
	{
		BeforeEachCallback = F;
	}

	void AfterEach(const TFunction<void()>& F)
	{
		AfterEachCallback = F;
	}

	void Test(const char* Name, const FTestFunctionPtr& F)
	{
		Tests.Add(UTF8_TO_TCHAR(Name), F);
	}

	/**
	 * @internal
	 */
	bool HasAnyTest() const
	{
		return Tests.Num() > 0;
	}

	virtual bool RunTest(const FString& Parameters) override;

	void TestJsonHasExactKeys(const TSharedPtr<FJsonObject>& Json, const TArray<FString>& ExpectedKeys, const FString& Context);

	void CreateTestWorld(EWorldType::Type WorldType = EWorldType::Editor);
	void DestroyTestWorld(bool bForceGC = false);

	UWorld* GetTestWorld();

	FTestWorldWrapper TestWorldWrapper;
	UWorld* TestWorld = nullptr;

private:
	TMap<FString, FTestFunctionPtr> Tests;
	TFunction<void()> BeforeEachCallback;
	TFunction<void()> AfterEachCallback;
};

#include "DssAutomationTestBase.inl"

