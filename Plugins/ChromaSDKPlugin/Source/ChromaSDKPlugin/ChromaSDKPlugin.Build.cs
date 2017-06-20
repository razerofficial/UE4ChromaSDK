// Some copyright should be here...

using UnrealBuildTool;

public class ChromaSDKPlugin : ModuleRules
{
	public ChromaSDKPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"ChromaSDKPlugin/Public"
				
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"ChromaSDKPlugin/Private",
				
				// ... add other private include paths required here ...
				"AssetTools",
				"Editor/DetailCustomizations/Private",
				"EditorStyle",
				"PropertyEditor",
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				
				// ... add other public dependencies that you statically link with here ...
				"AppFramework",
				"InputCore",
				"UnrealEd",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",

				// ... add private dependencies that you statically link with here ...	
				"AssetTools",
				"EditorStyle",
				"PropertyEditor",
				"DetailCustomizations",
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
