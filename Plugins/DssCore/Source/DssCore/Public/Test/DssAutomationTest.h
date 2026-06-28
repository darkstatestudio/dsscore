// Copyright Dark State Studio.All rights reserved.
#pragma once

#if WITH_DEV_AUTOMATION_TESTS
#include "DssAutomationTestBase.h"

#define DSS_TEST_SUITE_PRIVATE(_TClass, _PrettyName, _FileName, _LineNumber, _Flags, _StressTest ) \
namespace { \
	class _TClass; \
	constexpr auto ClassName = TEXT(#_TClass); \
	typedef _TClass TestClass; \
	class _TClass : public FDssAutomationTestBase \
	{ \
	public: \
		_TClass( const FString& InName = TEXT(#_TClass) ): FDssAutomationTestBase(InName) \
		{ \
			DefineTests(); \
		} \
		virtual FString GetTestSourceFileName() const override { return _FileName; } \
		virtual int32 GetTestSourceFileLine() const override { return _LineNumber; } \
		virtual EAutomationTestFlags GetTestFlags() const override { return _Flags; } \
		virtual bool IsStressTest() const { return _StressTest; }\
		virtual uint32 GetRequiredDeviceNum() const override { return 1; }\
	protected: \
		virtual bool RunTest(const FString& Parameters) override {\
			if (!HasAnyTest()) \
			{\
				AddError("No test cases found");\
				return false;\
			}\
			return FDssAutomationTestBase::RunTest(Parameters);\
		}\
		virtual void GetTests(TArray<FString>& OutBeautifiedNames, TArray <FString>& OutTestCommands) const override \
		{ \
			OutBeautifiedNames.Add(TEXT(_PrettyName)); \
			OutTestCommands.Add(TEXT("")); \
		} \
		virtual FString GetBeautifiedTestName() const override { return TEXT(_PrettyName); } \
	public: \
		void DefineTests();

#define DSS_TEST_SUITE(TClass, PrettyName) \
	DSS_TEST_SUITE_PRIVATE(TClass, PrettyName, __FILE__, __LINE__, EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter | EAutomationTestFlags::CriticalPriority, false)

#define DSS_UNIT_TEST_SUITE(TClass, PrettyName ) \
	DSS_TEST_SUITE_PRIVATE(TClass, PrettyName, __FILE__, __LINE__, EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter | EAutomationTestFlags::CriticalPriority, false)

#define DSS_WORLD_TEST_SUITE(TClass, PrettyName) \
	DSS_TEST_SUITE_PRIVATE(TClass, PrettyName, __FILE__, __LINE__, EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter, false)

#define DSS_DEFINE_TESTS \
};\
void TestClass::DefineTests()

#define DSS_TEST_SUITE_END_PRIVATE \
TestClass AutomationTestInstance(ClassName);\
};

#define DSS_TEST_SUITE_END \
	DSS_TEST_SUITE_END_PRIVATE

#endif
