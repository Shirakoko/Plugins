#include "UPlotNodeBase.h"

#include "SPlotEditorWidget/SPlotNode_Dialog.h"
#include "UPlotData/UPlotDataBase.h"

void UPlotNodeBase::PostTransacted(const FTransactionObjectEvent& TransactionEvent)
{
	Super::PostTransacted(TransactionEvent);
}

UObject* UPlotNodeBase::GetJumpTargetForDoubleClick() const
{
	return nullptr;
}

FText UPlotNodeBase::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	// 子类实现
	return FText();
}

FText UPlotNodeBase::GetTooltipText() const
{
	// 子类实现
	return FText();
}

FLinearColor UPlotNodeBase::GetNodeTitleColor() const
{
	return FLinearColor(0.4f, 0.4f, 0.4f);
}

void UPlotNodeBase::AutowireNewNode(UEdGraphPin* FromPin)
{
	// 子类实现
}

void UPlotNodeBase::Bind(UPlotDataBase* InSource)
{
	check(!Source.IsValid());
	check(!InSource->PlotNode.IsValid());

	Source = InSource;
	InSource->PlotNode = this;
}

TSharedPtr<SGraphNode> UPlotNodeBase::CreateVisualWidget()
{
	// 子类实现
	return nullptr;
}
