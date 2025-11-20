#include "UPlotData_Choice.h"

#include "Misc/AssertionMacros.h"


void UPlotData_Choice::DoTransacted()
{
    auto ChoiceNode = Cast<UPlotNode_Choice>(PlotNode);
    check(ChoiceNode);

    const TArray<UEdGraphPin*>& Pins = ChoiceNode->GetChoicePins();
    const int32 PinCount = Pins.Num();

    NextPlotList.SetNum(Options.Num());

    // 根据引脚读取连接关系，同步到 NextPlotList
    for (int32 Index = 0; Index < Pins.Num() && Index < NextPlotList.Num(); Index++)
    {
        UEdGraphPin* Pin = Pins[Index];

        if (Pin->LinkedTo.Num() == 1)
        {
            auto* NextNode = Cast<UPlotNodeBase>(Pin->LinkedTo[0]->GetOwningNode());
            NextPlotList[Index] = NextNode && NextNode->GetSource()
                ? NextNode->GetSource()->ID
                : 0;
        }
        else
        {
            NextPlotList[Index] = 0;
        }
    }

    // 调用父类触发序列化
    Super::DoTransacted();
}

void UPlotData_Choice::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (!PlotNode.IsValid()) return;

    // 判断是否修改了 Options，以此更新引脚数量
    const FName PropertyName = PropertyChangedEvent.Property
        ? PropertyChangedEvent.Property->GetFName()
        : NAME_None;

    Modify();

    if (PropertyName == GET_MEMBER_NAME_CHECKED(UPlotData_Choice, Options))
    {
        const EPropertyChangeType::Type ChangeType = PropertyChangedEvent.ChangeType;

        // 让新增的Options数组元素文本是"选项x"
        if (ChangeType == EPropertyChangeType::ArrayAdd || ChangeType == EPropertyChangeType::Duplicate)
        {
            int32 NewIndex = Options.Num() - 1;
            if (Options.IsValidIndex(NewIndex))
            {
                Options[NewIndex] = FString::Printf(TEXT("选项%d"), NewIndex + 1);
            }
        }
    }

    if (auto ChoiceNode = Cast<UPlotNode_Choice>(PlotNode))
    {
        // 引脚数量需要更新
        ChoiceNode->CreateOutputPinsByOptions();

        // 通知图更新
        if (UEdGraph* Graph = PlotNode->GetGraph())
        {
            Graph->NotifyGraphChanged();
        }
    }
}

