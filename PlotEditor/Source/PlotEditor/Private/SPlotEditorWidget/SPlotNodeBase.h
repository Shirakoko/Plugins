#pragma once
#include "SGraphNode.h"

class SPlotNodeBase : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SPlotNodeBase) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphNode* InNode);

	virtual void UpdateGraphNode() override;
	virtual void CreateBelowPinControls(TSharedPtr<SVerticalBox> MainBox) override;
};
