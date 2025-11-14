#pragma once

#include "UPlotDataBase.h"
#include "UPlotNode/UPlotNode_Choice.h"
#include "UPlotData_Choice.generated.h"

UCLASS(meta = (JsonSerialization))
class UPlotData_Choice : public UPlotDataBase
{
	GENERATED_BODY()
public:
	UPlotData_Choice()
	{
		NodeType = EPlotNodeType::Choice;
	}

	virtual UClass* GetNodeClass() const override
	{
		return UPlotNode_Choice::StaticClass();
	}

	/** 各选项对应的下一个节点 */
	UPROPERTY(NonTransactional)
	TMap<uint32, uint32> NextPlotMap;

	virtual void DoTransacted() override;
};
