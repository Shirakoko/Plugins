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
    // 获取编辑器上下文
    if (UEditorContext* EditorContext = Cast<UEditorContext>(ParentGraph->GetOuter()))
    {
        if (EditorContext->Toolkit.IsValid())
        {
            // 开始事务以支持撤销/重做
            const FScopedTransaction Transaction(NSLOCTEXT("PlotEditor", "AddDialogNode", "Add Dialog Node"));

            // 创建新的对话节点
            UPlotNode_Dialog* NewDialogNode = NewObject<UPlotNode_Dialog>(
                ParentGraph,
                NAME_None,
                RF_Transactional  // 添加事务标志
            );

            ParentGraph->Modify();  // 标记图表为已修改
            NewDialogNode->Modify(); // 标记节点为已修改

            // 添加到图表
            ParentGraph->AddNode(NewDialogNode, true, bSelectNewNode);

            // 设置节点属性
            NewDialogNode->NodeID = 0;
            NewDialogNode->NodeName = TEXT("NewDialogNode");
            NewDialogNode->Description = TEXT("Dialog Node");

            // 分配默认引脚
            NewDialogNode->AllocateDefaultPins();

            // 设置节点位置
            NewDialogNode->NodePosX = Location.X;
            NewDialogNode->NodePosY = Location.Y;

            // 自动连线（如果从现有引脚创建）
            if (FromPin)
            {
                NewDialogNode->AutowireNewNode(FromPin);
            }

            // 通知工具包节点已创建
            //EditorContext->Toolkit.Pin()->OnNodeCreated(NewDialogNode);

            // 刷新图表视图
            //EditorContext->Toolkit.Pin()->RefreshGraphView();

            // 重要：立即标记Package为Dirty
            if (UPackage* Package = ParentGraph->GetOutermost())
            {
                Package->MarkPackageDirty();
            }

            return NewDialogNode;
        }
    }

    return nullptr;
}