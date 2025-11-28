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

	UPROPERTY()
	int32 NextID = 1;

    int32 GetNextNodeID() { return NextID++; }

    // 初始化节点ID
    void InitializeNextIDByPlotDataMap()
    {
        int32 MaxID = 0;
        for (const auto& It : PlotDataMap)
        {
            MaxID = FMath::Max(MaxID, static_cast<int32>(It.Key));
        }
        NextID = MaxID + 1;
    }
};
