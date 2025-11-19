#include "SPlotNode_Dialog.h"

#include "UPlotNode/UPlotNode_Dialog.h"
#include "UPlotData/UPlotData_Dialog.h"

void SPlotNode_Dialog::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
}

void SPlotNode_Dialog::CreateBelowPinControls(TSharedPtr<SVerticalBox> MainBox)
{
	auto DialogNode = Cast<UPlotNode_Dialog>(GraphNode);
	auto DialogData = Cast<UPlotData_Dialog>(DialogNode->GetSource());

	check(DialogData);

	MainBox->AddSlot()
		.VAlign(VAlign_Center)
		.AutoHeight()
		.Padding(10.f, 2.f)
		[
			SNew(STextBlock)
				.Text(FText::FromString(TEXT("测试Dialog节点")))
				.Font(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 14))  
				.ColorAndOpacity(FLinearColor::White)
		];
}
