#if WITH_DEV_AUTOMATION_TESTS

#include "Test/DssAutomationTest.h"

#define TClass FSampleTests
DSS_TEST_SUITE(TClass, "DSS.Unit.Sample")

	DSS_TEST(TClass, GetRandom)
		TestEqual("Value", 1, 1);
	}

DSS_TEST_SUITE_END(TClass)

#endif
