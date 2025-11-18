#pragma once
#include "SGraphNode.h"

class SPlotNode_Choice : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SPlotNode_Choice) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphNode* InNode);

	// ~Begin SGraphNode implement
	virtual void CreateBelowPinControls(TSharedPtr<SVerticalBox> MainBox) override;
	// ~End SGraphNode
};