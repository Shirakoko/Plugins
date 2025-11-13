#include "UPlotNodeBase.h"

#include "SPlotEditorWidget/SPlotNodeBase.h"

UPlotNodeBase::UPlotNodeBase()
{
	NodeID = -1;
	NodeName = TEXT("Unnamed Node");
	Description = TEXT("");
}

void UPlotNodeBase::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "PlotFlow", FName(), TEXT("In"));
	CreatePin(EGPD_Output, "PlotFlow", FName(), TEXT("Out"));
}

FText UPlotNodeBase::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(NodeName);
}

FText UPlotNodeBase::GetTooltipText() const
{
	return FText::FromString(Description);
}

FLinearColor UPlotNodeBase::GetNodeTitleColor() const
{
	return FLinearColor(0.4f, 0.4f, 0.4f);
}

void UPlotNodeBase::AutowireNewNode(UEdGraphPin* FromPin)
{
	// 默认连线逻辑
	//if (FromPin && FromPin->Direction == EGPD_Output)
	//{
	//	FromPin->MakeLinkTo(GetInputPin());
	//}
}

TSharedPtr<SGraphNode> UPlotNodeBase::CreateVisualWidget()
{
	return SNew(SPlotNodeBase, this);
}
