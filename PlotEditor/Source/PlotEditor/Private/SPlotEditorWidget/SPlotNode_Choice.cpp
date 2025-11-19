#include "SPlotNode_Choice.h"

#include "UPlotNode/UPlotNode_Choice.h"
#include "UPlotData/UPlotData_Choice.h"

void SPlotNode_Choice::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
}

void SPlotNode_Choice::CreateBelowPinControls(TSharedPtr<SVerticalBox> MainBox)
{
	auto ChoiceNode = Cast<UPlotNode_Choice>(GraphNode);
	auto ChoideData = Cast<UPlotData_Choice>(ChoiceNode->GetSource());

	check(ChoideData);

	MainBox->AddSlot()
		.VAlign(VAlign_Center)
		.AutoHeight()
		.Padding(10.f, 2.f)
		[
			SNew(STextBlock)
				.Text(FText::FromString(TEXT("测试Choice节点")))
				.Font(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 14))
				.ColorAndOpacity(FLinearColor::White)
		];
}