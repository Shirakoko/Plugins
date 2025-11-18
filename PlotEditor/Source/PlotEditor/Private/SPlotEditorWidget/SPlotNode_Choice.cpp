#include "SPlotNode_Choice.h"
#include "UPlotNode/UPlotNode_Choice.h"

void SPlotNode_Choice::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
}

void SPlotNode_Choice::CreateBelowPinControls(TSharedPtr<SVerticalBox> MainBox)
{
	auto DialogNode = Cast<UPlotNode_Choice>(GraphNode);
	auto DialogData = DialogNode->GetSource();

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