// Some copyright should be here...

using UnrealBuildTool;

public class ChromaSDKEditor : ModuleRules
{
#if WITH_FORWARDED_MODULE_RULES_CTOR
    public ChromaSDKEditor(ReadOnlyTargetRules Target) : base(Target) // 4.16 or better
#else
    public ChromaSDKEditor(TargetInfo Target) //4.15 or lower
#endif
    {
#if WITH_FORWARDED_MODULE_RULES_CTOR
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
#endif
		
		PublicIncludePaths.AddRange(
			new string[] {
				"ChromaSDKEditor/Public",
				// ... add public include paths required here ...
				"ChromaSDKPlugin/Public",
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"ChromaSDKEditor/Private",
				
				// ... add other private include paths required here ...
				"AssetTools",
				"ChromaSDKPlugin/Private",
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
				"ChromaSDKPlugin",
				"DesktopPlatform",
				"InputCore",
				"UnrealEd",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",

				// ... add private dependencies that you statically link with here ...	
				"AssetTools",
				"ChromaSDKPlugin",
				"DetailCustomizations",
				"EditorStyle",
				"PropertyEditor",
				"Slate",
				"SlateCore",
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
