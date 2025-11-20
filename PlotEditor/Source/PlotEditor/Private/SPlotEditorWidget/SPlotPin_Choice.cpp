#include "SPlotPin_Choice.h"

#include "UPlotNode/UPlotNode_Choice.h"
#include "UPlotData/UPlotData_Choice.h"
#include "Widgets/Text/SMultiLineEditableText.h"
#include "CommonMacros.h"

void SPlotPin_Choice::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
    OptionText = InArgs._OptionText;
    OnTextCommitted = InArgs._OnTextCommitted;
	SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
}

TSharedRef<SWidget> SPlotPin_Choice::GetLabelWidget(const FName& Name)
{
    return SNew(SMultiLineEditableText)
        .Text(FText::FromString(OptionText))
        .TextStyle(&DIALOG_CONTENT_TEXT_STYLE)
        .AutoWrapText(true)
        .OnTextCommitted(OnTextCommitted);
}
