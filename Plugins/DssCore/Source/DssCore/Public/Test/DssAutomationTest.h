// Copyright Dark State Studio.All rights reserved.
#pragma once

#if WITH_DEV_AUTOMATION_TESTS
#include "DssAutomationTestBase.h"

#define DSS_TEST_SUITE_PRIVATE( _TClass, _PrettyName, _FileName, _LineNumber, _Flags, _StressTest ) \
	class _TClass : public TFDssAutomationTestBase<_TClass> \
	{ \
	public: \
		_TClass( const FString& InName ): TFDssAutomationTestBase(InName) {} \
		virtual FString GetTestSourceFileName() const override { return _FileName; } \
		virtual int32 GetTestSourceFileLine() const override { return _LineNumber; } \
		virtual EAutomationTestFlags GetTestFlags() const override { return _Flags; } \
		virtual bool IsStressTest() const { return _StressTest; }\
		virtual uint32 GetRequiredDeviceNum() const override { return 1; }\
	protected: \
		virtual bool RunTest(const FString& Parameters) override {\
			return TFDssAutomationTestBase::RunTest(Parameters);\
		}\
		virtual void GetTests(TArray<FString>& OutBeautifiedNames, TArray <FString>& OutTestCommands) const override \
		{ \
			OutBeautifiedNames.Add(_PrettyName); \
			OutTestCommands.Add(FString()); \
		} \
		virtual FString GetBeautifiedTestName() const override { return _PrettyName; }

#define DSS_UNIT_TEST_SUITE(TClass, PrettyName ) \
	DSS_TEST_SUITE_PRIVATE(TClass, PrettyName, __FILE__, __LINE__, EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter | EAutomationTestFlags::CriticalPriority, false)

#define DSS_TEST_SUITE(TClass, PrettyName) \
DSS_TEST_SUITE_PRIVATE(TClass, PrettyName, __FILE__, __LINE__, EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter | EAutomationTestFlags::CriticalPriority, false)

#define DSS_WORLD_TEST_SUITE(TClass, PrettyName) \
	DSS_TEST_SUITE_PRIVATE(TClass, PrettyName, __FILE__, __LINE__, EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter, false)

//#define DSS_ADD_TEST(Name) \
//	RegisterTest( TEXT(#Name), static_cast<void (std::remove_reference_t<decltype(*this)>::*)()>(&std::remove_reference_t<decltype(*this)>::TEST_##Name));

#define DSS_TEST(TClass, Name) \
	struct _DSS_RT_##Name { \
		_DSS_RT_##Name(TFDssAutomationTestBase* Self) { \
			Self->RegisterTest(TEXT(#Name), &TClass::TEST_##Name); \
		} \
	}; \
	_DSS_RT_##Name _RT_##Name{this}; \
	void TEST_##Name()\
	{\
		AddInfo(FString::Printf(TEXT("Test: %s \n"), TEXT(#Name)));

#define DSS_TEST_SUITE_END_PRIVATE(TClass) \
	}; namespace { TClass TClass##AutomationTestInstance(TEXT(#TClass));};

#define DSS_TEST_SUITE_END(TClass) \
	DSS_TEST_SUITE_END_PRIVATE( TClass )

#endif
