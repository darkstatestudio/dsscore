// Test to compare DssAutomation simple tests with complex

BEGIN_DEFINE_SPEC(TestSpeedSpec, "DSS.Unit.TestSpeedSpec", EAutomationTestFlags::SmokeFilter | EAutomationTestFlags::EditorContext)
END_DEFINE_SPEC(TestSpeedSpec)

void TestSpeedSpec::Define()
{
	Describe("Execute()", [this]()
	{
		It("should return true when successful", [this]()
		{
			TestTrue("Execute", true);
		});

		It("should return false when unsuccessful", [this]()
		{
			TestFalse("Execute", false);
		});
	});
}
