#pragma once

#include "UEditorContext.generated.h"

class FPlotEditorToolkit;

UCLASS()
class UEditorContext : public UObject
{
	GENERATED_BODY()

public:
	virtual void PostTransacted(const FTransactionObjectEvent& TransactionEvent) override;

	TWeakPtr<FPlotEditorToolkit> Toolkit;

	UPROPERTY()
	TMap<uint32, class UPlotDataBase*> PlotDataMap;

	UPROPERTY(meta = (IgnoreJsonSerialization))
	int32 NextID = 1;

    // 初始化ID
    void InitializeNextIDByPlotDataMap()
    {
        int32 MaxID = 0;
        for (const auto& It : PlotDataMap)
        {
            MaxID = FMath::Max(MaxID, (int32)It.Key);
        }
        NextID = MaxID + 1;
    }
};
