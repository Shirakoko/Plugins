#include "SPlotNode_Dialog.h"
#include "UPlotNode/UPlotNode_Dialog.h"
#include "Widgets/Text/STextBlock.h"

void SPlotNode_Dialog::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	SPlotNodeBase::Construct(SPlotNodeBase::FArguments(), InNode);
}

void SPlotNode_Dialog::CreateBelowPinControls(TSharedPtr<SVerticalBox> MainBox)
{
	if (UPlotNode_Dialog* Node = Cast<UPlotNode_Dialog>(GraphNode))
	{
		for (auto& Line : Node->Lines)
		{
			MainBox->AddSlot()
				.AutoHeight()
				.Padding(2)
				[
					SNew(STextBlock)
						.Text(FText::FromString(FString::Printf(TEXT("%s: %s"), *Line.Speaker, *Line.Content)))
				];
		}
	}
}
