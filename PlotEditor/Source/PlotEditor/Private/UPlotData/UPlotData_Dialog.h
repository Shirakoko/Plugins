#pragma once

#include "UPlotDataBase.h"
#include "UPlotData_Dialog.generated.h"

UCLASS(meta = (JsonSerialization))
class UPlotData_Dialog : public UPlotDataBase
{
	GENERATED_BODY()
public:

	/** 下一个节点ID */
	UPROPERTY(NonTransactional)
	uint32 NextPlot = 0;

	virtual void DoTransacted() override;
};