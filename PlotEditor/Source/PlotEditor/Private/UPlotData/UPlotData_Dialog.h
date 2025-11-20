#pragma once

#include "UPlotDataBase.h"
#include "UPlotNode/UPlotNode_Dialog.h"
#include "UPlotData_Dialog.generated.h"

USTRUCT(BlueprintType)
struct FPlotDialogLine
{
	GENERATED_BODY()

	/** 说话人 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Speaker;

	/** 说话内容 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Content;
};

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

	/** 对话内容数组 */
	UPROPERTY(EditAnywhere, Category = "Plot", meta = (DisplayName = "对话内容"))
	TArray<FPlotDialogLine> DialogLines;

	virtual void DoTransacted() override;

	// ~Begin Interface UObject::PostEditChangeProperty
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	// ~End Interface
};