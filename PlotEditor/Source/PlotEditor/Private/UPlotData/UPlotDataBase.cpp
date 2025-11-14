#include  "UPlotDataBase.h"

#include "UPlotNode/UPlotNodeBase.h"
#include "FPlotEditorToolkit/FPlotEditorToolkit.h"

void UPlotDataBase::PostTransacted(const FTransactionObjectEvent& TransactionEvent)
{
	Super::PostTransacted(TransactionEvent);
	DoTransacted();
}

void UPlotDataBase::DoTransacted()
{
	Toolkit.Pin()->SerializeAllPlots();

	if (PlotNode.IsValid())
	{
		PlotNode->GetGraph()->NotifyGraphChanged();
	}
}
