#include "SPlotNode_Choice.h"

#include "UPlotNode/UPlotNode_Choice.h"
#include "UPlotData/UPlotData_Choice.h"
#include "CommonMacros.h"
#include "SPlotEditorWidget/SPlotPin_Choice.h"

#define LOCTEXT_NAMESPACE "SPlotNode_Choice"

void SPlotNode_Choice::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
}

TSharedRef<SWidget> SPlotNode_Choice::CreateNodeContentArea()
{
	CachedContentArea = SGraphNode::CreateNodeContentArea();

	// 限制节点宽度
	return SAssignNew(CachedContentArea, SBox)
		.WidthOverride(WIDTH_NODE)
		[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				[
					CachedContentArea.ToSharedRef()
				]
		];
}

void SPlotNode_Choice::CreateBelowPinControls(TSharedPtr<SVerticalBox> MainBox)
{
	auto ChoiceNode = Cast<UPlotNode_Choice>(GraphNode);
	auto ChoiceData = Cast<UPlotData_Choice>(ChoiceNode->GetSource());
	check(ChoiceData);

	MainBox->AddSlot()
		.AutoHeight()
		.HAlign(HAlign_Fill)
		.Padding(5.f, 4.f)
		[
			SNew(SButton)
				.Text(FText::FromString(TEXT("+ 添加选项")))
				.OnClicked(this, &SPlotNode_Choice::OnAddOptionClicked)
		];
}

void SPlotNode_Choice::CreatePinWidgets()
{
	int32 OutputIndex = 0;

	// Create Pin widgets for each of the pins.
	for (int32 PinIndex = 0; PinIndex < GraphNode->Pins.Num(); ++PinIndex)
	{
		UEdGraphPin* CurPin = GraphNode->Pins[PinIndex];

		if (!ensureMsgf(CurPin->GetOuter() == GraphNode
			, TEXT("Graph node ('%s' - %s) has an invalid %s pin: '%s'; (with a bad %s outer: '%s'); skiping creation of a widget for this pin.")
			, *GraphNode->GetNodeTitle(ENodeTitleType::ListView).ToString()
			, *GraphNode->GetPathName()
			, (CurPin->Direction == EEdGraphPinDirection::EGPD_Input) ? TEXT("input") : TEXT("output")
			, CurPin->PinFriendlyName.IsEmpty() ? *CurPin->PinName.ToString() : *CurPin->PinFriendlyName.ToString()
			, CurPin->GetOuter() ? *CurPin->GetOuter()->GetClass()->GetName() : TEXT("UNKNOWN")
			, CurPin->GetOuter() ? *CurPin->GetOuter()->GetPathName() : TEXT("NULL")))
		{
			continue;
		}

		if (CurPin->Direction == EGPD_Output)
		{
			// 调用 CreateStandardPinWidget，并传 OutputIndex
			CreateStandardPinWidget_Choice(CurPin, OutputIndex);
			OutputIndex++;
		}
		else
		{
			// 输入 pin 仍走原生逻辑
			CreateStandardPinWidget(CurPin);
		}
	}
}

void SPlotNode_Choice::CreateStandardPinWidget_Choice(UEdGraphPin* CurPin, int32 OutputIndex)
{
	const bool bShowPin = ShouldPinBeHidden(CurPin);

	if (bShowPin)
	{
		TSharedPtr<SGraphPin> NewPin = CreatePinWidget_Choice(CurPin, OutputIndex);
		check(NewPin.IsValid());

		this->AddPin(NewPin.ToSharedRef());
	}
}

TSharedPtr<SGraphPin> SPlotNode_Choice::CreatePinWidget_Choice(UEdGraphPin* Pin, int32 OutputIndex) const
{
	if (Pin->Direction == EGPD_Output)
	{
		UPlotNode_Choice* ChoiceNode = Cast<UPlotNode_Choice>(GraphNode);
		if (!ChoiceNode) return SGraphNode::CreatePinWidget(Pin);

		UPlotData_Choice* ChoiceData = Cast<UPlotData_Choice>(ChoiceNode->GetSource());
		if (!ChoiceData) return SGraphNode::CreatePinWidget(Pin);

		// 数组越界检查
		if (!ChoiceData->Options.IsValidIndex(OutputIndex))
		{
			return SGraphNode::CreatePinWidget(Pin);
		}

		return SNew(SPlotPin_Choice, Pin)
			.OptionText(ChoiceData->Options[OutputIndex])
			.OnTextCommitted(FOnTextCommitted::CreateLambda(
				[ChoiceData, OutputIndex](const FText& NewText, ETextCommit::Type CommitType)
				{
					if (ChoiceData && ChoiceData->Options.IsValidIndex(OutputIndex))
					{
						ChoiceData->Options[OutputIndex] = NewText.ToString();
						ChoiceData->DoTransacted();
					}
				}
			));
	}

	return SGraphNode::CreatePinWidget(Pin);
}

FReply SPlotNode_Choice::OnAddOptionClicked()
{
	// 添加事务
	const FScopedTransaction Transaction(LOCTEXT("AddDialog", "Add Dialog"));

	auto ChoiceNode = Cast<UPlotNode_Choice>(GraphNode);
	auto ChoiceData = Cast<UPlotData_Choice>(ChoiceNode->GetSource());
	if (!ChoiceData) return FReply::Handled();

	// 手动标记对象被事务系统记录
	ChoiceData->Modify();

	FString NewOptionName = FString::Printf(TEXT("选项%d"), ChoiceData->Options.Num() + 1);

	// 修改对象
	ChoiceData->Options.Add(NewOptionName);

	// 根据选项个数重新创建引脚
	ChoiceNode->CreateOutputPinsByOptions();

	// 序列化保存
	ChoiceData->DoTransacted();
	return FReply::Handled();
}
#undef LOCTEXT_NAMESPACE

