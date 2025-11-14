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

FLinearColor UPlotNodeBase::GetNodeTitleColor() const
{
	return FLinearColor(0.4f, 0.4f, 0.4f);
}

void UPlotNodeBase::Bind(UPlotDataBase* InSource)
{
	check(!Source.IsValid());
	check(!InSource->PlotNode.IsValid());

	Source = InSource;
	InSource->PlotNode = this;
}
