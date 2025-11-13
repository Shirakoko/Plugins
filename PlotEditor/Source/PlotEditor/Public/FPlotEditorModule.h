// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "AssetToolsModule.h"

class FPlotEditorModule : public IModuleInterface
{
public:

	// ~Begin IModuleInterface implementation
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// ~End IModuleInterface

private:
	TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;
	EAssetTypeCategories::Type AssetCategoryBit = EAssetTypeCategories::Misc;
};
