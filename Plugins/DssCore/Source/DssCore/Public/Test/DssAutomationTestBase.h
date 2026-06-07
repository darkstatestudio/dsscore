// Copyright Dark State Studio.All rights reserved.
#pragma once

#include "Misc/AutomationTest.h"

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

protected:
	virtual bool RunTest(const FString& Parameters) override;

	virtual void BeforeEach()
	{
	}

	virtual void AfterEach()
	{
	}

	void TestJsonHasExactKeys(const TSharedPtr<FJsonObject>& Json,const TArray<FString>& ExpectedKeys,const FString& Context);

private:
	TMap<FString, MethodPtr> RegisteredMethods;
};

#include "DssAutomationTestBase.inl"

