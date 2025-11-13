// SPlotNodeBase.cpp
#include "SPlotNodeBase.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

void SPlotNodeBase::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	this->GraphNode = InNode;
	UpdateGraphNode();
}

void SPlotNodeBase::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(RightNodeBox, SVerticalBox)
		];
}

void SPlotNodeBase::CreateBelowPinControls(TSharedPtr<SVerticalBox> MainBox)
{
	MainBox->AddSlot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(STextBlock).Text(FText::FromString("Base Node"))
		];
}
