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

	/** index → 下一个节点 ID（不显示在细节面板） */
	UPROPERTY(NonTransactional)
	TArray<uint32> NextPlotList;

	/** index -> 选项文本 */
	UPROPERTY(EditAnywhere, Category = "Plot", meta = (DisplayName = "选项内容"))
	TArray<FString> Options;

	virtual void DoTransacted() override;

	// ~Begin Interface UObject::PostEditChangeProperty
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	// ~End Interface
};
