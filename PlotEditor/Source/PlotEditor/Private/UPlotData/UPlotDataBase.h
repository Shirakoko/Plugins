#pragma once

#include "UPlotNode/UPlotNodeBase.h"
#include "UPlotDataBase.generated.h"

class FPlotEditorToolkit;

UENUM()
enum class EPlotNodeType : uint8
{
	Dialog,
	Choice,
};


UCLASS(meta = (JsonSerialization))
class UPlotDataBase : public UObject
{
	GENERATED_BODY()
public:

	void Initialize(uint32 InID, const TSharedPtr<FPlotEditorToolkit>& InToolkit)
	{
		ID = InID;
		Toolkit = InToolkit;
	}

	virtual UClass* GetNodeClass() const
	{
		return UPlotNodeBase::StaticClass();
	}

	UPROPERTY(VisibleAnywhere, Category = "Plot", meta = (DisplayPriority = 0))
	uint32 ID;

	UPROPERTY(NonTransactional)
	FVector2D NodePos;

	UPROPERTY(NonTransactional)
	EPlotNodeType NodeType;

	UPROPERTY(meta = (IgnoreJsonSerialization))
	bool IsDeleted;

	TWeakPtr<FPlotEditorToolkit> Toolkit;

	// ~Begin Interface UObject::PostTransacted
	virtual void PostTransacted(const FTransactionObjectEvent& TransactionEvent) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	// ~End Interface

	TWeakObjectPtr<class UPlotNodeBase> PlotNode;

	virtual void DoTransacted();
};