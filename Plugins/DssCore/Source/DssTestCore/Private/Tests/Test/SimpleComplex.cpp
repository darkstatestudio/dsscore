// Test to compare DssAutomation simple tests with complex

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FComplexSpeedTest, "Unit.ComplexSpeed",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void FComplexSpeedTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	OutBeautifiedNames.Add(TEXT("Test1"));
	OutTestCommands.Add(TEXT("Test1"));

	OutBeautifiedNames.Add(TEXT("Test2"));
	OutTestCommands.Add(TEXT("Test2"));

	OutBeautifiedNames.Add(TEXT("Test3"));
	OutTestCommands.Add(TEXT("Test3"));
}

bool FComplexSpeedTest::RunTest(const FString& Parameters)
{
	if (Parameters == TEXT("Test1"))
	{
		TestFalse("Execute", false);
		return true;
	}

	if (Parameters == TEXT("Test2"))
	{
		TestFalse("Execute", false);
		return true;
	}

	if (Parameters == TEXT("Test3"))
	{
		TestFalse("Execute", false);
		return true;
	}

	return false;
}
