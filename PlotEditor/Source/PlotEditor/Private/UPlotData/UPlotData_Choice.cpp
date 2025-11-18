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

    if (!PlotNode.IsValid())
        return;

    // 判断是否修改了 Options
    const FName PropertyName = PropertyChangedEvent.Property
        ? PropertyChangedEvent.Property->GetFName()
        : NAME_None;

    if (PropertyName == GET_MEMBER_NAME_CHECKED(UPlotData_Choice, Options))
    {
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
}

