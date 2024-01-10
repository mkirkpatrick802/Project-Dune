using UnrealBuildTool;

public class Project_Dune : ModuleRules
{
	public Project_Dune(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "KismetWidgets", "EnhancedInput", "UMG" });
	}
}
