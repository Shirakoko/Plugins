#pragma once

#include "UPlotNodeBase.h"
#include "UPlotNode_Dialog.generated.h"


UCLASS()
class UPlotNode_Dialog : public UPlotNodeBase
{
	GENERATED_BODY()
public:
	virtual void PostTransacted(const FTransactionObjectEvent& TransactionEvent) override;

	// ~Begin UEdGraphNode interface
	virtual void AllocateDefaultPins() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	// ~End UEdGraphNode

	UEdGraphPin* GetPrevPin() const { return PrevPin; }
	UEdGraphPin* GetNextPin() const { return NextPin; }

private:
	UEdGraphPin* PrevPin = nullptr;
	UEdGraphPin* NextPin = nullptr;
};
