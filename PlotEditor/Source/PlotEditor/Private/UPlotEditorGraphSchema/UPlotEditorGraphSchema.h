#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "UPlotEditorGraphSchema.generated.h"

USTRUCT()
struct FPlotGraphAction_NewDialogNode : public FEdGraphSchemaAction
{
	GENERATED_BODY()

	FPlotGraphAction_NewDialogNode()
		: FEdGraphSchemaAction(INVTEXT("PlotGraph"), INVTEXT("添加新对话节点"), INVTEXT("在图中添加一个新的对话节点"), 0)
	{
	}

	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
};

UCLASS()
class UPlotEditorGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	// ~Begin UEdGraphSchema interface

	/** 右键空白处或者拖拽引脚的上下文菜单 */
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	/** 是否能创建引脚连接 */
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	/** 引脚颜色 */
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
	/** 断开操作  */
	virtual void BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotification) const override;
	// ~End UEdGraphSchema
};
