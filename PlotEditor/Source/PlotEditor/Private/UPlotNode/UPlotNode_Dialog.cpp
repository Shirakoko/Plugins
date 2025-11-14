#include "UPlotNode_Dialog.h"

#include "SPlotEditorWidget/SPlotNode_Dialog.h"
#include "FPlotEditorPinFactory.h"
#include "UPlotData/UPlotDataBase.h"

void UPlotNode_Dialog::PostTransacted(const FTransactionObjectEvent& TransactionEvent)
{
	Super::PostTransacted(TransactionEvent);
	if (Source.IsValid())
	{
		Source->DoTransacted();
	}
}

void UPlotNode_Dialog::AllocateDefaultPins()
{
	PrevPin = CreatePin(EGPD_Input, FPlotEditorPinFactory::PinCategory, FName(), TEXT("上个节点"));
	NextPin = CreatePin(EGPD_Output, FPlotEditorPinFactory::PinCategory, FName(), TEXT("下个节点"));
}

void UPlotNode_Dialog::AutowireNewNode(UEdGraphPin* FromPin)
{
	if (!FromPin) return;

	Modify();
	if (FromPin->Direction == EGPD_Input)
	{
		NextPin->MakeLinkTo(FromPin);
	}
	else
	{
		PrevPin->MakeLinkTo(FromPin);
	}
}

TSharedPtr<SGraphNode> UPlotNode_Dialog::CreateVisualWidget()
{
	return SNew(SPlotNode_Dialog, this);
}
