#pragma once

#include "UPlotDataBase.generated.h"

class FPlotEditorToolkit;
UCLASS()
class UPlotDataBase : public UObject
{
	GENERATED_BODY()
public:

	UPlotDataBase()
	{

	}

	void Initialize(uint32 InID, TSharedPtr<FPlotEditorToolkit> InToolkit)
	{
		ID = InID;
		Toolkit = InToolkit;
	}

	UPROPERTY(meta = (IgnoreJsonSerialization))
	uint32 ID;

	TWeakPtr<FPlotEditorToolkit> Toolkit;

	// ~Begin Interface UObject::PostTransacted
	virtual void PostTransacted(const FTransactionObjectEvent& TransactionEvent) override;
	// ~End Interfacce

	UPROPERTY(meta = (IgnoreJsonSerialization))
	bool IsDeleted;

	TWeakObjectPtr<class UPlotNodeBase> PlotNode;

private:
	void DoTransacted();
};