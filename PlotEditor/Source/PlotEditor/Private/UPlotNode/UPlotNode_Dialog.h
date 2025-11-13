#pragma once

#include "UPlotNodeBase.h"
#include "UPlotNode_Dialog.generated.h"

USTRUCT()
struct FDialogueLine
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Dialog")
	FString Speaker;

	UPROPERTY(EditAnywhere, Category = "Dialog")
	FString Content;
};

UCLASS()
class UPlotNode_Dialog : public UPlotNodeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Dialog")
	TArray<FDialogueLine> Lines;

	virtual void AllocateDefaultPins() override;
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
};
