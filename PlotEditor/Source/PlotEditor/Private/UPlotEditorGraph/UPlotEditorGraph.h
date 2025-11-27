#pragma once
#pragma once
#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "UPlotEditorGraph.generated.h"

class UEditorContext;
UCLASS()
class UPlotEditorGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UEditorContext> EditorContext;
};