#pragma once

#include "UPlotEditorEntry.generated.h"

UCLASS()
class UPlotEditorEntry : public UObject
{
	GENERATED_BODY()
public:
	UPlotEditorEntry(): Graph(nullptr){}

	/** 剧情图引用（实际编辑的数据结构） */
	UPROPERTY()	
	TObjectPtr<UEdGraph> Graph;
};