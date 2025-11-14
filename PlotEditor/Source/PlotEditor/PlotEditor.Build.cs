// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PlotEditor : ModuleRules
{
	public PlotEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
				"CoreUObject", // 虚幻对象系统核心，处理UClass、UProperty
                "Engine", // 引擎核心功能
                "Slate", // 虚幻的UI框架，用于构建编辑器界面
                "SlateCore", // Slate的基础组件和核心功能
                "UnrealEd", //  虚幻编辑器核心模块
                "ToolMenus", // 工具菜单系统
                "AssetTools", // 资源工具
                "BlueprintGraph", // 蓝图图表系统，用于可视化脚本
                "EditorFramework", // 图编辑器，用于显示和编辑节点图
				"GraphEditor", // 编辑器框架基础，提供扩展点
				"InputCore", //  输入系统核心，处理键盘鼠标输入
				"Json", // Json
                "JsonUtilities",
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
