#include "UPlotNode_Choice.h"

#include "UPlotData/UPlotDataBase.h"
#include "FPlotEditorPinFactory.h"
#include "SPlotEditorWidget/SPlotNode_Choice.h"
#include "UPlotData/UPlotData_Choice.h"

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
    // 创建输入引脚
    PrevPin = CreatePin(EGPD_Input, FPlotEditorPinFactory::PinCategory, NAME_None, TEXT("上一幕"));
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

void UPlotNode_Choice::CreateOutputPinsByOptions()
{
    if (!Source.IsValid()) return;

    UPlotData_Choice* Data = Cast<UPlotData_Choice>(Source.Get());
    if (!Data) return;

    const int32 DesiredCount = Data->Options.Num();

    // 构建 PinName 列表
    TArray<FName> DesiredPinNames;
    DesiredPinNames.Reserve(DesiredCount);

    for (int32 Index = 0; Index < DesiredCount; ++Index)
    {
        DesiredPinNames.Add(FName(*FString::Printf(TEXT("选项_%d"), Index)));
    }

    // 移除多余的 Pin（Data 少于当前 Pin）
    for (int32 i = ChoicePins.Num() - 1; i >= 0; --i)
    {
        UEdGraphPin* Pin = ChoicePins[i];
        if (!Pin) continue;

        if (!DesiredPinNames.Contains(Pin->PinName))
        {
            // 断开自己的连接
            Pin->BreakAllPinLinks();

            // 从 EdGraph 移除
            Pins.Remove(Pin);

            // 从 ChoicePins 移除
            ChoicePins.RemoveAt(i);
        }
    }

    // 补充缺失的 Pin（Options 个数多于当前 Pin 个数）
    for (int32 i = 0; i < DesiredCount; ++i)
    {
        FName DesiredName = DesiredPinNames[i];

        // 查找是否已有这个 Pin
        UEdGraphPin* Existing = nullptr;
        for (UEdGraphPin* P : ChoicePins)
        {
            if (P->PinName == DesiredName)
            {
                Existing = P;
                break;
            }
        }

        if (Existing == nullptr)
        {
            // 创建新 Pin
            UEdGraphPin* NewPin = CreatePin(
                EGPD_Output,
                FPlotEditorPinFactory::PinCategory,
                NAME_None,
                DesiredName
            );

            ChoicePins.Insert(NewPin, i);
        }
        else
        {
            // 已经存在，确保它位置正确
            ChoicePins.Remove(Existing);
            ChoicePins.Insert(Existing, i);
        }
    }
}
