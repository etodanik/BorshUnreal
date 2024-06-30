using UnrealBuildTool;
using System.IO;

public class BorshTests : ModuleRules
{
	public BorshTests(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Borsh",
				"Core",
				"CoreUObject"
			});
	}
}