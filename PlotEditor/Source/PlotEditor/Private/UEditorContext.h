#pragma once

#include "UEditorContext.generated.h"

class FPlotEditorToolkit;

UCLASS()
class UEditorContext : public UObject
{
	GENERATED_BODY()

public:
	virtual void PostTransacted(const FTransactionObjectEvent& TransactionEvent) override;

	TWeakPtr<FPlotEditorToolkit> Toolkit;
};
