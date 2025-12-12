// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPlotEditorModule.h"

#include "UPlotEditorEntryActions.h"
#include "IAssetTools.h"
#include "FCustomizationDetails.h"

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

	// 注册细节面板定制
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	// 对 FPlotDialogLine 注册属性细节面板定制
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FName("PlotDialogLine"),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FPlotDialogLineCustomization::MakeInstance)
	);
	// 对 UPlotDataBase 注册类细节面板定制
	PropertyModule.RegisterCustomClassLayout(
		FName("PlotDataBase"),
		FOnGetDetailCustomizationInstance::CreateStatic(&FPlotDataDetailCustomization::MakeInstance)
	);
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

	// 注销细节面板定制
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout(FName("PlotDialogLine"));
		PropertyModule.UnregisterCustomClassLayout(FName("PlotDataBase"));
	}
}
	

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPlotEditorModule, PlotEditor)