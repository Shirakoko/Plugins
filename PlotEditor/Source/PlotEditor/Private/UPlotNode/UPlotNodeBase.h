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
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual UObject* GetJumpTargetForDoubleClick() const override;
	// ~End UEdGraphNode

	void Bind(UPlotDataBase* InSource);
	UPlotDataBase* GetSource() const { return Source.Get(); }

protected:
	TWeakObjectPtr<UPlotDataBase> Source;
};