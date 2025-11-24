#pragma once

#include "UPlotEditorEntry.generated.h"

class UPlotEditorGraph;

UCLASS()
class UPlotEditorEntry : public UObject
{
	GENERATED_BODY()

public:
	/** 剧情图引用（实际编辑的数据结构） */
	UPROPERTY()	
	TObjectPtr<UPlotEditorGraph> Graph;
};