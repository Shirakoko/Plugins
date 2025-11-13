#include "UPlotNode_Dialog.h"
#include "SPlotEditorWidget/SPlotNode_Dialog.h"

void UPlotNode_Dialog::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "PlotFlow", FName(), TEXT("In"));
	CreatePin(EGPD_Output, "PlotFlow", FName(), TEXT("Out"));
}

TSharedPtr<SGraphNode> UPlotNode_Dialog::CreateVisualWidget()
{
	return SNew(SPlotNode_Dialog, this);
}
