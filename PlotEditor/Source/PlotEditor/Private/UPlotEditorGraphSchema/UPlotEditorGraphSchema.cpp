#include "UPlotEditorGraphSchema.h"

#include "UPlotNode/UPlotNode_Dialog.h"
#include "UPlotNode/UPlotNode_Choice.h"
#include "UEditorContext.h"
#include "FPlotEditorToolkit/FPlotEditorToolkit.h"
#include "UPlotEditorGraph/UPlotEditorGraph.h"

#define LOCTEXT_NAMESPACE "PlotEditorGraphSchema"

void UPlotEditorGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	ContextMenuBuilder.AddAction(MakeShared<FPlotGraphAction_NewDialogNode>());
	ContextMenuBuilder.AddAction(MakeShared<FPlotGraphAction_NewChoiceNode>());

	Super::GetGraphContextActions(ContextMenuBuilder);
}

void UPlotEditorGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	if (!Context || !Context->Node)
	{
		return;
	}

	// 节点右键菜单
	if (Context->Node)
	{
		// 添加“创建注释框”菜单项
		FToolMenuSection& CommentSection = Menu->AddSection("SchemaActionComment", LOCTEXT("MultiCommentHeader", "Comment Group"));
		CommentSection.AddMenuEntry(
			"MultiCommentDesc",
			LOCTEXT("MultiCommentDesc", "为选中的节点创建注释"),
			LOCTEXT("CommentToolTip", "Create a resizable comment box around selection."),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateLambda([this, Context]()
				{
					UE_LOG(LogTemp, Warning, TEXT("Comment menu item clicked"));
					if (Context->Graph)
					{
						this->AddComment(const_cast<UEdGraph*>(ToRawPtr(Context->Graph)), FVector2D::ZeroVector, true);
					}
				}))
		);
	}
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

	// 从 ParentGraph 找到 EditorContext，然后找到 Toolkit，调用 Toolkit 里实际创建节点的方法
	auto EditorContext = Cast<UPlotEditorGraph>(ParentGraph)->EditorContext;
	check(EditorContext);
	
	auto Toolkit = EditorContext->Toolkit.Pin();
	if (Toolkit.IsValid())
	{
		NewNode = Toolkit->Action_NewDialog(ParentGraph, FromPin, Location, bSelectNewNode);
	}
	return NewNode;
}

UEdGraphNode* FPlotGraphAction_NewChoiceNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	UPlotNode_Choice* NewNode = nullptr;

	if (FromPin)
	{
		// 如果 FromPin 是输出 pin，并且它已经连接了别的 pin，则禁止自动创建
		if (FromPin->Direction == EGPD_Output && FromPin->LinkedTo.Num() >= 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("[PlotEditor] Cannot create new choice node: output pin <%s> already has a connection."), *FromPin->GetName()
			);
			return nullptr;
		}
	}

	// 从 ParentGraph 找到 EditorContext，然后找到 Toolkit，调用 Toolkit 里实际创建节点的方法
	auto EditorContext = Cast<UPlotEditorGraph>(ParentGraph)->EditorContext;
	check(EditorContext);
	
	auto Toolkit = EditorContext->Toolkit.Pin();
	if (Toolkit.IsValid())
	{
		// 调用 Toolkit 里实际创建节点的方法
		NewNode = Toolkit->Action_NewChoice(ParentGraph, FromPin, Location, bSelectNewNode);
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

void UPlotEditorGraphSchema::AddComment(UEdGraph* ParentGraph, const FVector2D& Location, bool bSelectNewNode)
{
	auto PlotEditorGraph = Cast<UPlotEditorGraph>(ParentGraph);
	check(PlotEditorGraph);

	auto EditorContext = PlotEditorGraph->EditorContext;
	check(EditorContext);

	TWeakPtr<FPlotEditorToolkit> Toolkit = EditorContext->Toolkit;
	if(Toolkit.IsValid())
	{
		Toolkit.Pin()->Action_NewComment(PlotEditorGraph, Location, bSelectNewNode);
	}
}
#undef LOCTEXT_NAMESPACE