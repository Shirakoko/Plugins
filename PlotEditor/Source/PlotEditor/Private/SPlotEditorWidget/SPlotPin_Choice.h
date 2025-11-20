#pragma once

#include "SGraphPin.h"

class SPlotPin_Choice : public SGraphPin
{
public:
    SLATE_BEGIN_ARGS(SPlotPin_Choice) {}
        SLATE_ARGUMENT(FString, OptionText)
        SLATE_EVENT(FOnTextCommitted, OnTextCommitted)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj);

protected:
    //~ Begin SGraphPin interface
    virtual TSharedRef<SWidget> GetLabelWidget(const FName& Name) override;
    //~ End SGraphPin

private:
    FString OptionText;
    FOnTextCommitted OnTextCommitted;
};