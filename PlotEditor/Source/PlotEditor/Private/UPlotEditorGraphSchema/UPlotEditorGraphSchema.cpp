#include "UPlotEditorGraphSchema.h"

#include "UPlotNode/UPlotNode_Dialog.h"
#include "UEditorContext.h"
#include "FPlotEditorToolkit/FPlotEditorToolkit.h"

void UPlotEditorGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	ContextMenuBuilder.AddAction(MakeShared<FPlotGraphAction_NewDialogNode>());
	Super::GetGraphContextActions(ContextMenuBuilder);
}

const FPinConnectionResponse UPlotEditorGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	// 不能同方向连接，不能自己连自己
	if (A->Direction == B->Direction || A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("You Can NOT Do It."));
	}
	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT("OK!"));
}

FLinearColor UPlotEditorGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FLinearColor::White;
}

void UPlotEditorGraphSchema::BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotification) const
{
	FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_BreakPinLinks", "Break Pin Links"));
	Super::BreakPinLinks(TargetPin, bSendsNodeNotification);
}

UEdGraphNode* FPlotGraphAction_NewDialogNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	UPlotNode_Dialog* NewNode = nullptr;

	if (auto EditorContext = Cast<UEditorContext>(ParentGraph->GetOuter()))
	{
		auto Toolkit = EditorContext->Toolkit.Pin();
		if (Toolkit.IsValid())
		{
			NewNode = Toolkit->Action_NewDialog(ParentGraph, FromPin, Location, bSelectNewNode);
		}
	}
	return NewNode;
}
