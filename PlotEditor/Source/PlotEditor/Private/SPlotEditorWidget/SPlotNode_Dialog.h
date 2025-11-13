#pragma once
#include "SPlotNodeBase.h"

class SPlotNode_Dialog : public SPlotNodeBase
{
public:
	SLATE_BEGIN_ARGS(SPlotNode_Dialog) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphNode* InNode);

	// ~Begin SGraphNode implement
	virtual void CreateBelowPinControls(TSharedPtr<SVerticalBox> MainBox) override;
	virtual void UpdateGraphNode() override;
	// ~End SGraphNode

	FText GetNodeTitle() const;
	FSlateColor GetBorderBackgroundColor() const;

	TSharedRef<SWidget> CreateNodeContent();
};
#pragma once
