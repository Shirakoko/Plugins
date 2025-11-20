#include "UPlotData_Dialog.h"

#include "UPlotNode/UPlotNodeBase.h"

void UPlotData_Dialog::DoTransacted()
{
	auto DialogNode = Cast<UPlotNode_Dialog>(PlotNode);
	check(DialogNode);

	auto NextPins = DialogNode->GetNextPin()->LinkedTo;
	check(NextPins.Num() <= 1);

	// 设置NextPlot
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

void UPlotData_Dialog::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (!PlotNode.IsValid()) return;

	const FName PropertyName = PropertyChangedEvent.Property
		? PropertyChangedEvent.Property->GetFName()
		: NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UPlotData_Dialog, DialogLines))
	{
		const EPropertyChangeType::Type ChangeType = PropertyChangedEvent.ChangeType;

		// 让新增的元素默认填入"说话人"和"内容"
		if (ChangeType == EPropertyChangeType::ArrayAdd || ChangeType == EPropertyChangeType::Duplicate)
		{
			int32 NewIndex = DialogLines.Num() - 1;
			if (DialogLines.IsValidIndex(NewIndex))
			{
				// 自动填默认值
				DialogLines[NewIndex].Speaker = TEXT("说话人");
				DialogLines[NewIndex].Content = TEXT("内容");
			}
		}
	}
}
