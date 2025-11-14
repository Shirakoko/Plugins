#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "UPlotNodeBase.generated.h"

class UPlotDataBase;

UCLASS(Abstract)
class UPlotNodeBase : public UEdGraphNode
{
	GENERATED_BODY()
public:
	virtual void PostTransacted(const FTransactionObjectEvent& TransactionEvent) override;

	// ~Begin UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetTooltipText() const override;
	virtual UObject* GetJumpTargetForDoubleClick() const override;
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	// ~End UEdGraphNode

	void Bind(UPlotDataBase* InSource);
	UPlotDataBase* GetSource() const { return Source.Get(); }

	UEdGraphPin* GetNextPin() const { return NextPin; }
	UEdGraphPin* GetPrevPin() const { return PrevPin; }

protected:
	TWeakObjectPtr<UPlotDataBase> Source;
	UEdGraphPin* NextPin;
	UEdGraphPin* PrevPin;
};