#include "UPlotNode_Choice.h"

#include "UPlotData/UPlotDataBase.h"
#include "FPlotEditorPinFactory.h"
#include "SPlotEditorWidget/SPlotNode_Choice.h"

void UPlotNode_Choice::PostTransacted(const FTransactionObjectEvent& TransactionEvent)
{
    Super::PostTransacted(TransactionEvent);

    if (Source.IsValid())
    {
        Source->DoTransacted();
    }
}

void UPlotNode_Choice::AllocateDefaultPins()
{
    PrevPin = CreatePin(EGPD_Input, FPlotEditorPinFactory::PinCategory, FName(), TEXT("上一慕"));

    ChoicePins.Empty();

    // TODO：可以改成根据数据层动态生成
    const int32 ChoiceCount = 2;

    for (int32 Index = 0; Index < ChoiceCount; ++Index)
    {
        FString PinName = FString::Printf(TEXT("选项%d"), Index + 1);
        UEdGraphPin* Pin = CreatePin(EGPD_Output, FPlotEditorPinFactory::PinCategory, FName(), *PinName);

        ChoicePins.Add(Pin);
    }
}

void UPlotNode_Choice::AutowireNewNode(UEdGraphPin* FromPin)
{
    if (!FromPin) return;

    Modify();

    if (FromPin->Direction == EGPD_Output)
    {
        // 其他节点的输出节点连接到输入节点
        if (PrevPin)
        {
            FromPin->MakeLinkTo(PrevPin);
        }
    }
    else
    {
        // 第一个选项连接到其他节点的输入节点
        if (ChoicePins.Num() > 0 && ChoicePins[0])
        {
            ChoicePins[0]->MakeLinkTo(FromPin);
        }
    }
}

TSharedPtr<SGraphNode> UPlotNode_Choice::CreateVisualWidget()
{
    return SNew(SPlotNode_Choice, this);
}
