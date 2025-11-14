#include "UPlotEditorGraphSchema.h"

#include "UPlotNode/UPlotNode_Dialog.h"
#include "UEditorContext.h"
#include "FPlotEditorToolkit/FPlotEditorToolkit.h"

void UPlotEditorGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	ContextMenuBuilder.AddAction(MakeShared<FPlotGraphAction_NewDialogNode>());
	Super::GetGraphContextActions(ContextMenuBuilder);
}

UEdGraphNode* FPlotGraphAction_NewDialogNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	UPlotNode_Dialog* NewNode = nullptr;

	if (FromPin)
	{
		// 如果 FromPin 是输出 pin，并且它已经连接了别的 pin，则禁止自动创建
		if (FromPin->Direction == EGPD_Output && FromPin->LinkedTo.Num() >= 1)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("[PlotEditor] Cannot create new dialog node: output pin <%s> already has a connection."),
				*FromPin->GetName()
			);
			return NewNode;
		}
	}

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

const FPinConnectionResponse UPlotEditorGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	// 不能同方向 & 不能连自己
	if (A->Direction == B->Direction || A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Invalid connection"));
	}

	// 找到输出引脚（不确定 A 是不是输出，所以判断一下）
	const UEdGraphPin* OutputPin = (A->Direction == EGPD_Output) ? A : B;

	// 输出引脚已经有连接，则不允许继续连
	if (OutputPin->LinkedTo.Num() >= 1)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Output pin can only connect to one input"));
	}

	// 允许创建
	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT("OK"));
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
