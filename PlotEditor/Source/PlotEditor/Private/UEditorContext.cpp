#include "UEditorContext.h"
#include "FPlotEditorToolkit/FPlotEditorToolkit.h"
#include "SPlotEditorWidget/SPlotGraphView.h"

void UEditorContext::PostTransacted(const FTransactionObjectEvent& TransactionEvent)
{
	if (Toolkit.IsValid())
	{
		Toolkit.Pin()->GetPlotGraphView()->GetGraphEditorPtr()->NotifyGraphChanged();
	}
	Super::PostTransacted(TransactionEvent);

	// 自动修正NextID
	InitializeNextIDByPlotDataMap();
}