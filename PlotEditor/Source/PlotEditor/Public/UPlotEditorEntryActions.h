#pragma once

#include "AssetTypeActions_Base.h"
#include "CoreMinimal.h"

class PLOTEDITOR_API UPlotEditorEntryActions : public FAssetTypeActions_Base
{
public:
	UPlotEditorEntryActions(EAssetTypeCategories::Type InAssetCategory);

	// ~Begin IAssetTypeActions implementation
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;
	virtual UClass* GetSupportedClass() const override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	// ~End IAssetTypeActions

private:
	EAssetTypeCategories::Type AssetCategory;
};