#include "UPlotData_Choice.h"

void UPlotData_Choice::DoTransacted()
{
    auto ChoiceNode = Cast<UPlotNode_Choice>(PlotNode);
    check(ChoiceNode);

    NextPlotMap.Empty();

    // 遍历所有 choice pins
    const TArray<UEdGraphPin*>& Pins = ChoiceNode->GetChoicePins();
    for (int32 Index = 0; Index < Pins.Num(); ++Index)
    {
        UEdGraphPin* Pin = Pins[Index];
        const auto& Links = Pin->LinkedTo;

        // 每个选项最多一个连接
        check(Links.Num() <= 1);

        if (Links.Num() == 1)
        {
            auto NextNode = Cast<UPlotNodeBase>(Links[0]->GetOwningNode());
            if (NextNode && NextNode->GetSource())
            {
                NextPlotMap.Add(Index, NextNode->GetSource()->ID);
            }
        }
        else
        {
            NextPlotMap.Add(Index, 0);
        }
    }

    // 调用父类触发序列化
    Super::DoTransacted();
}
