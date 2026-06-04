// Copyright Dark State Studio.All rights reserved.
#pragma once

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

template <typename TDerived>
class FDssAutomationTestBase : public FAutomationTestBase
{
public:
	using MethodPtr = void (TDerived::*)();

	FDssAutomationTestBase(const FString& InName) : FAutomationTestBase(InName, false)
	{
	}

	void RegisterTest(const FString& Name, MethodPtr Ptr)
	{
		RegisteredMethods.Add(Name, Ptr);
	}

protected:
	virtual bool RunTest(const FString& Parameters) override
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

	virtual void BeforeEach()
	{
	}

	virtual void AfterEach()
	{
	}

private:
	TMap<FString, MethodPtr> RegisteredMethods;
};

#define DSS_TEST_SUITE_PRIVATE( TClass, PrettyName, FileName, LineNumber ) \
	class TClass : public FDssAutomationTestBase<TClass> \
	{ \
	public: \
		TClass( const FString& InName ): FDssAutomationTestBase(InName) {\
		}\
		virtual FString GetTestSourceFileName() const override { return FileName; } \
		virtual int32 GetTestSourceFileLine() const override { return LineNumber; } \
		virtual EAutomationTestFlags GetTestFlags() const override { return EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter | EAutomationTestFlags::CriticalPriority; } \
		virtual bool IsStressTest() const { return false; }\
		virtual uint32 GetRequiredDeviceNum() const override { return 1; }\
	protected: \
		virtual bool RunTest(const FString& Parameters) override {\
			return FDssAutomationTestBase::RunTest(Parameters);\
		}\
		virtual void GetTests(TArray<FString>& OutBeautifiedNames, TArray <FString>& OutTestCommands) const override \
		{ \
			OutBeautifiedNames.Add(PrettyName); \
			OutTestCommands.Add(FString()); \
		} \
		virtual FString GetBeautifiedTestName() const override { return PrettyName; }

#define DSS_TEST_SUITE( TClass, PrettyName ) \
	DSS_TEST_SUITE_PRIVATE(TClass, PrettyName, __FILE__, __LINE__)

//#define DSS_ADD_TEST(Name) \
//	RegisterTest( TEXT(#Name), static_cast<void (std::remove_reference_t<decltype(*this)>::*)()>(&std::remove_reference_t<decltype(*this)>::TEST_##Name));

#define DSS_TEST(TClass, Name) \
	struct _DSS_RT_##Name { \
		_DSS_RT_##Name(FDssAutomationTestBase* Self) { \
			Self->RegisterTest(TEXT(#Name), &TClass::TEST_##Name); \
		} \
	}; \
	_DSS_RT_##Name _RT_##Name{this}; \
	void TEST_##Name()\
	{\
		AddInfo(FString::Printf(TEXT("Test: %s"), TEXT(#Name)));

#define DSS_TEST_SUITE_END_PRIVATE(TClass) \
	}; namespace { TClass TClass##AutomationTestInstance(TEXT(#TClass));};

#define DSS_TEST_SUITE_END(TClass) \
	DSS_TEST_SUITE_END_PRIVATE( TClass )

#endif
