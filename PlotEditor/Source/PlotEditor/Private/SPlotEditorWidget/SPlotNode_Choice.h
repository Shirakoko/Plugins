#pragma once
#include "SGraphNode.h"

class SPlotNode_Choice : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SPlotNode_Choice) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphNode* InNode);

	// ~Begin SGraphNode implement
	virtual TSharedRef<SWidget> CreateNodeContentArea() override;
	virtual void CreateBelowPinControls(TSharedPtr<SVerticalBox> MainBox) override;
	virtual void CreatePinWidgets() override;
	// ~End SGraphNode

private:
	void CreateStandardPinWidget_Choice(UEdGraphPin* CurPin, int32 OutputIndex);
	TSharedPtr<SGraphPin> CreatePinWidget_Choice(UEdGraphPin* Pin, int32 OutputIndex) const;

	TSharedPtr<SWidget> CachedContentArea;
	FReply OnAddOptionClicked();
};