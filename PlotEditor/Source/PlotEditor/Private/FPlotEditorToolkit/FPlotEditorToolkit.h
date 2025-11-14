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

	/** 新增Dialog节点 */
	class UPlotNode_Dialog* Action_NewDialog(UEdGraph* ParentGraph = nullptr, UEdGraphPin* FromPin = nullptr, const FVector2D Location = { 0.0, 0.0 }, bool bSelectNewNode = true);
	class UPlotNode_Choice* Action_NewChoice(UEdGraph* ParentGraph = nullptr, UEdGraphPin* FromPin = nullptr, const FVector2D Location = { 0.0, 0.0 }, bool bSelectNewNode = true);
	/** 删除Plot节点*/
	void Action_DeletePlots(TArray<uint32> InPlotIDList);

	/** 序列化保存所有的Plot数据 */
	void SerializeAllPlots();

	TSharedPtr<SPlotGraphView> GetPlotGraphView() { return PlotGraphView; }
	TSharedPtr<IDetailsView> GetDetailsView() { return DetailsView; }

	UEditorContext* GetEditorContext() { return EditorContext; }

private:
	/** 正在编辑的资产 */
	TObjectPtr<UPlotEditorEntry> CurrentAsset;

	/** 正在编辑器的资产的Json文件路径 */
	FString GetCurrentAssetJsonFilePath();

	/** 图编辑器的Slate控件 */
	TSharedPtr<SPlotGraphView> PlotGraphView;

	/** Detials面板的Slate控件 */
	TSharedPtr<IDetailsView> DetailsView;

	/** 菜单分类项 */
	TSharedPtr<FWorkspaceItem> MenuCategory;

	/** 加载Plot数据 */
	void LoadPlotsData();

	/** 生成Plot图 */
	void InitPlotGraph();

	/** 创建Graph视图 */
	void CreatePlotGraphView(const TSharedRef<FTabManager>& InTabManager);
	/** 创建Detials面板 */
	void CreateDetailsPanel(const TSharedRef<FTabManager>& InTabManager);

	/** 编辑器上下文 */
	TObjectPtr<UEditorContext> EditorContext;

	void ConnectDialogNode(class UPlotData_Dialog* DialogData);

	void ConnectChoiceNode(class UPlotData_Choice* ChoiceData);

	int32 GetNextNodeID();
};
