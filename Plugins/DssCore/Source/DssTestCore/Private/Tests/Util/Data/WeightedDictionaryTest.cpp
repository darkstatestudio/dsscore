#if WITH_DEV_AUTOMATION_TESTS

#include "Test/DssAutomationTest.h"
#include "Util/Data/WeightedDictionary.h"

DSS_UNIT_TEST_SUITE(FWeightedStringDictionaryTest, "Unit.Util.Data.WeightedDictionary")
	DSS_DEFINE_TESTS
	{
		Test("GetRandom", [this]
		{
			FWeightedStringDictionary Dict;
			Dict.Add(TFWeightedValue<FString>("test_1", 0.5f));
			Dict.Add(TFWeightedValue<FString>("test_2", 0.5f));

			Dict.RefreshTotalWeight();
			FRandomStream Stream(100);

			auto Current = Dict.GetRandom(Stream);

			TestEqual("Value", Current.Value, TEXT("test_2"));
		});
	}

DSS_TEST_SUITE_END
#endif
