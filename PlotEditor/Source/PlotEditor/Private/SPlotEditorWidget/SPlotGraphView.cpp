# include "SPlotGraphView.h"

#include "Framework/Commands/GenericCommands.h"
#include "FPlotEditorToolkit/FPlotEditorToolkit.h"
#include "UPlotEditorGraphSchema/UPlotEditorGraphSchema.h"
#include "UEditorContext.h"

void SPlotGraphView::Construct(const FArguments& InArgs, TSharedPtr<FPlotEditorToolkit> InToolkit)
{
	Toolkit = InToolkit;

	UEditorContext* Context = InToolkit->GetEditorContext();
	check(Context);

	GraphObj = NewObject<UEdGraph>(Context);
	GraphObj->SetFlags(RF_Transactional); // 可被撤销
	GraphObj->Schema = UPlotEditorGraphSchema::StaticClass();

	SGraphEditor::FGraphEditorEvents GraphEvents;
	GraphEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateRaw(this, &SPlotGraphView::OnSelectedNodesChanged); // 选择变化回调
	GraphEvents.OnNodeDoubleClicked = FSingleNodeEvent::CreateRaw(this, &SPlotGraphView::OnNodeDoubleClicked); // 双击回调
	GraphEvents.OnTextCommitted = FOnNodeTextCommitted::CreateRaw(this, &SPlotGraphView::OnNodeTextCommitted); // 文本提交回调

	UICommandList = MakeShareable(new FUICommandList);
	UICommandList->MapAction(FGenericCommands::Get().Delete, FExecuteAction::CreateRaw(this, &SPlotGraphView::DeleteSelectedNodes));

	// 创建 GraphEditor 的 Slate 控件
	GraphEditorPtr = SNew(SGraphEditor)
		.GraphToEdit(GraphObj)
		.GraphEvents(GraphEvents)
		.AdditionalCommands(UICommandList)
		.ShowGraphStateOverlay(false);

	ChildSlot
		[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.FillHeight(1.f)
				.HAlign(HAlign_Fill)
				[
					SNew(SOverlay)
						+ SOverlay::Slot()
						[
							GraphEditorPtr.ToSharedRef()
						]
				]
		];
}

void SPlotGraphView::OnSelectedNodesChanged(const FGraphPanelSelectionSet& SelectionSet) const
{
	if (!Toolkit.IsValid()) return;

	// TODO: 节点切换变换时调用
}

void SPlotGraphView::OnNodeDoubleClicked(UEdGraphNode* ClickedNode)
{
	if (!Toolkit.IsValid() || !ClickedNode) return;

	// TODO: 双击节点
}

void SPlotGraphView::DeleteSelectedNodes()
{
	if (!Toolkit.IsValid()) return;

	const FGraphPanelSelectionSet Selected = GraphEditorPtr->GetSelectedNodes();
	if (Selected.Num() == 0) return;

	//TODO: 删除选中节点

	// 通知图表更新
	if (GraphEditorPtr.IsValid())
	{
		GraphEditorPtr->NotifyGraphChanged();
	}
}

// 当编辑器内提交文本时调用
void SPlotGraphView::OnNodeTextCommitted(const FText& NewText, ETextCommit::Type CommitType, UEdGraphNode* Node)
{
	if (!Node) return;

	// TODO: 编辑器内提交文本时调用
}