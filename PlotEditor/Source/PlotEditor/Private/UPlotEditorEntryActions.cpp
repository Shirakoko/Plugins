#include "UPlotEditorEntryActions.h"

#include "UPlotEditorEntry/UPlotEditorEntry.h"
#include "FPlotEditorToolkit/FPlotEditorToolkit.h"

UPlotEditorEntryActions::UPlotEditorEntryActions(EAssetTypeCategories::Type InAssetCategory) : AssetCategory(InAssetCategory)
{
}

FText UPlotEditorEntryActions::GetName() const
{
	return INVTEXT("Plot Editor Entry");
}

FColor UPlotEditorEntryActions::GetTypeColor() const
{
	return FColor::Cyan;
}

uint32 UPlotEditorEntryActions::GetCategories()
{
	return AssetCategory;
}

UClass* UPlotEditorEntryActions::GetSupportedClass() const
{
	return UPlotEditorEntry::StaticClass();
}

void UPlotEditorEntryActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto& Object : InObjects)
	{
		if (const auto PlotAsset = Cast<UPlotEditorEntry>(Object))
		{
			TSharedRef<FPlotEditorToolkit> EditorToolkit = MakeShared<FPlotEditorToolkit>();
			EditorToolkit->InitPlotEditor(Mode, EditWithinLevelEditor, PlotAsset);
		}
	}
}
