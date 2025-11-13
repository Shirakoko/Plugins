#include "UPlotEditorGraphSchema.h"
#include "UPlotNode/UPlotNode_Dialog.h"

void UPlotEditorGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	ContextMenuBuilder.AddAction(MakeShared<FPlotGraphAction_NewDialogNode>());
	Super::GetGraphContextActions(ContextMenuBuilder);
}

UEdGraphNode* FPlotGraphAction_NewDialogNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	// TODO: 实现创建节点
	return nullptr;
}
