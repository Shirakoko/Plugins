#pragma once

#include "UPlotDataBase.generated.h"

class FPlotEditorToolkit;

UCLASS(meta = (JsonSerialization))
class UPlotDataBase : public UObject
{
	GENERATED_BODY()
public:

	void Initialize(uint32 InID, TSharedPtr<FPlotEditorToolkit> InToolkit)
	{
		ID = InID;
		Toolkit = InToolkit;
	}

	UPROPERTY(VisibleAnywhere, Category = "Plot")
	uint32 ID;

	TWeakPtr<FPlotEditorToolkit> Toolkit;

	// ~Begin Interface UObject::PostTransacted
	virtual void PostTransacted(const FTransactionObjectEvent& TransactionEvent) override;
	// ~End Interfacce

	UPROPERTY(meta = (IgnoreJsonSerialization))
	bool IsDeleted;

	TWeakObjectPtr<class UPlotNodeBase> PlotNode;

	virtual void DoTransacted();
};