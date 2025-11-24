#pragma once

#include "UPlotEditorEntry.generated.h"

class UPlotEditorGraph;

UCLASS()
class UPlotEditorEntry : public UObject
{
	GENERATED_BODY()

public:
	/** UPlotEditorGraph引用 */
	UPROPERTY()	
	TObjectPtr<UPlotEditorGraph> Graph;
};