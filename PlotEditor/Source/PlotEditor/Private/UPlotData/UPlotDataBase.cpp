#include  "UPlotDataBase.h"

void UPlotDataBase::PostTransacted(const FTransactionObjectEvent& TransactionEvent)
{
	Super::PostTransacted(TransactionEvent);
	DoTransacted();
}

void UPlotDataBase::DoTransacted()
{
	if (IsDeleted)
	{
		// TODO: 删除对象
	}
	else
	{
		// TODO: 序列化保存
	}
}
