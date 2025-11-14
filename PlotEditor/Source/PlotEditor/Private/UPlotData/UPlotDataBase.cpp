#include  "UPlotDataBase.h"

#include "UPlotNode/UPlotNodeBase.h"
#include "FPlotEditorToolkit/FPlotEditorToolkit.h"

void UPlotDataBase::PostTransacted(const FTransactionObjectEvent& TransactionEvent)
{
	Super::PostTransacted(TransactionEvent);
	DoTransacted();
}

void UPlotDataBase::DoTransacted()
{
	// 保存节点位置
	NodePos = FVector2D(PlotNode->NodePosX, PlotNode->NodePosY);

	// 保存所有节点数据
	Toolkit.Pin()->SerializeAllPlots();

	// 通知UEdGraph更新
	if (PlotNode.IsValid())
	{
		PlotNode->GetGraph()->NotifyGraphChanged();
	}
}
