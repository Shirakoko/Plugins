// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPlotEditorModule.h"

#include "UPlotEditorEntryActions.h"
#include "IAssetTools.h"

#define LOCTEXT_NAMESPACE "FPlotEditorModule"

void FPlotEditorModule::StartupModule()
{
	// 注册 Asset Actions
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		// 注册自定义资产分类"Plot Editor"
		AssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory("PlotEditor", INVTEXT("Plot Editor"));

		// 创建资产类型动作实例
		TSharedRef<IAssetTypeActions> Action = MakeShareable(new UPlotEditorEntryActions(AssetCategoryBit));
		// 注册到AssetTools系统
		AssetTools.RegisterAssetTypeActions(Action);
		// 保存引用，用于后续注销
		CreatedAssetTypeActions.Add(Action);
	}
}

void FPlotEditorModule::ShutdownModule()
{
	// 注销所有注册的Asset Actions
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (TSharedPtr<IAssetTypeActions> AssetTypeAction : CreatedAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(AssetTypeAction.ToSharedRef());
		}
	}

	CreatedAssetTypeActions.Empty();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPlotEditorModule, PlotEditor)