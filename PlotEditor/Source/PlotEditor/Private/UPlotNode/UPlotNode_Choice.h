#pragma once

#include "UPlotNodeBase.h"
#include "UPlotNode_Choice.generated.h"

UCLASS()
class UPlotNode_Choice : public UPlotNodeBase
{
    GENERATED_BODY()
public:
    virtual void PostTransacted(const FTransactionObjectEvent& TransactionEvent) override;

    // ~Begin UEdGraphNode interface
    virtual void AllocateDefaultPins() override;
    virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
    virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
    // ~End UEdGraphNode

    void CreateOutputPinsByOptions();

    UEdGraphPin* GetPrevPin() const { return PrevPin; }
    const TArray<UEdGraphPin*>& GetChoicePins() const { return ChoicePins; }

private:
    UEdGraphPin* PrevPin;

    TArray<UEdGraphPin*> ChoicePins;
};