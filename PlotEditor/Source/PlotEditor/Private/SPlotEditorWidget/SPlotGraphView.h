#pragma once

class FPlotEditorToolkit;
class UPlotEditorGraph;

class SPlotGraphView : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPlotGraphView) {}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, TSharedPtr<FPlotEditorToolkit> InToolkit);

	TSharedPtr<SGraphEditor> GetGraphEditorPtr() { return GraphEditorPtr; }
	TObjectPtr<UPlotEditorGraph> GetGraphObj() { return GraphObj; }

private:
	/** 选择的节点切换时回调（由 SGraphEditor 的 OnSelectedNodesChanged 触发）*/
	void OnSelectedNodesChanged(const FGraphPanelSelectionSet& SelectionSet) const;

	/** 节点文本提交时回调（由 SGraphEditor 的 OnNodeTextCommitted 触发）*/
	void OnNodeTextCommitted(const FText& NewText, ETextCommit::Type CommitType, UEdGraphNode* Node);

	/** 双击节点回调（由 SGraphEditor 的 OnNodeDoubleClicked 触发） */
	void OnNodeDoubleClicked(UEdGraphNode* ClickedNode);

	/** 删除选中节点 */
	void DeleteSelectedNodes();

	TWeakPtr<FPlotEditorToolkit> Toolkit;

	TSharedPtr<SGraphEditor> GraphEditorPtr;
	TObjectPtr<UPlotEditorGraph> GraphObj;

	TSharedPtr<FUICommandList> UICommandList;
};