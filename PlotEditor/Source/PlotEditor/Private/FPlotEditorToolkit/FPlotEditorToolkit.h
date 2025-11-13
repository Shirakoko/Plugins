#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"

class UPlotEditorEntry;
class SPlotGraphView;
class UEditorContext;

class FPlotEditorToolkit : public FAssetEditorToolkit
{
public:
	void InitPlotEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UPlotEditorEntry* InPlotAsset);

	// ~Begin IToolkit implementation
	virtual FName GetToolkitFName() const override { return FName("UserPlotEditor"); }
	virtual FText GetBaseToolkitName() const override { return FText::FromString(TEXT("Plot Editor")); }
	virtual FString GetWorldCentricTabPrefix() const override { return TEXT("Plot"); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor(0.5f, 0.2f, 0.7f); }

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	// ~End IToolkit

	TSharedPtr<SPlotGraphView> GetPlotGraphView() { return PlotGraphView; }

	UEditorContext* GetEditorContext() { return EditorContext; }

private:
	/** 正在编辑的资产 */
	TObjectPtr<UPlotEditorEntry> CurrentAsset;

	/** 图编辑器的Slate控件 */
	TSharedPtr<SPlotGraphView> PlotGraphView;

	/** 菜单分类项 */
	TSharedPtr<FWorkspaceItem> MenuCategory;

	/** 创建Graph视图 */
	void CreatePlotGraphView(const TSharedRef<FTabManager>& InTabManager);

	/** 编辑器上下文 */
	TObjectPtr<UEditorContext> EditorContext;
};
