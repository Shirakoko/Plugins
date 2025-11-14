#include "UPlotData_Dialog.h"

#include "UPlotNode/UPlotNodeBase.h"

void UPlotData_Dialog::DoTransacted()
{

	auto NextPins = PlotNode->GetNextPin()->LinkedTo;
	check(NextPins.Num() <= 1);

	if (NextPins.Num() == 1)
	{
		auto NextPlotNode = Cast<UPlotNodeBase>(NextPins[0]->GetOwningNode());
		NextPlot = NextPlotNode->GetSource()->ID;
	}
	else
	{
		NextPlot = 0;
	}

	// 调用父类的DoTransacted触发序列化
	Super::DoTransacted();
}
