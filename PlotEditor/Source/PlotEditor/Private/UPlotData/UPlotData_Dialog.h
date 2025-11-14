#pragma once

#include "UPlotDataBase.h"
#include "UPlotNode/UPlotNode_Dialog.h"
#include "UPlotData_Dialog.generated.h"

UCLASS(meta = (JsonSerialization))
class UPlotData_Dialog : public UPlotDataBase
{
	GENERATED_BODY()
public:
	UPlotData_Dialog()
	{
		NodeType = EPlotNodeType::Dialog;
	}

	virtual UClass* GetNodeClass() const override
	{
		return UPlotNode_Dialog::StaticClass();
	}

	/** 下一个节点ID */
	UPROPERTY(NonTransactional)
	uint32 NextPlot = 0;

	virtual void DoTransacted() override;
};