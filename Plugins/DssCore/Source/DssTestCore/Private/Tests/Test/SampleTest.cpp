#if WITH_DEV_AUTOMATION_TESTS

#include "Test/DssAutomationTest.h"


DSS_UNIT_TEST_SUITE(FSampleTest, "Unit.Sample")
	DSS_DEFINE_TESTS
	{
		BeforeEach([this]
		{
		});

		AfterEach([this]
		{
		});

		Test("Simple1", [this]
		{
			TestEqual("Value", 1, 1);
		});

		Test("Simple2", [this]
		{
			TestEqual("Value", 1, 1);
		});

		Test("Simple3", [this]
		{
			TestEqual("Value", 1, 1);
		});
	}

DSS_TEST_SUITE_END
#endif
