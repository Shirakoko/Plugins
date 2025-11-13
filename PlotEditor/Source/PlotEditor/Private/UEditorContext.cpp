#include "UEditorContext.h"
#include "FPlotEditorToolkit/FPlotEditorToolkit.h"
#include "SPlotEditorWidget/SPlotGraphView.h"

void UEditorContext::PostTransacted(const FTransactionObjectEvent& TransactionEvent)
{
	if (Toolkit.IsValid())
	{
		Toolkit.Pin()->GetPlotGraphView()->GetGraphEditorPtr()->NotifyGraphChanged();
	}
	UObject::PostTransacted(TransactionEvent);
}