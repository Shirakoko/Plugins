#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "UPlotNodeBase.generated.h"

UCLASS(Abstract)
class UPlotNodeBase : public UEdGraphNode
{
	GENERATED_BODY()
public:
	UPlotNodeBase();

	UPROPERTY(EditAnywhere, Category = "Plot")
	int32 NodeID;

	UPROPERTY(EditAnywhere, Category = "Plot")
	FString NodeName;

	UPROPERTY(EditAnywhere, Category = "Plot")
	FString Description;

	// --- UEdGraphNode interface ---
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	// --- End ---
};