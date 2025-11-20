#include "SPlotNode_Dialog.h"

#include "UPlotNode/UPlotNode_Dialog.h"
#include "UPlotData/UPlotData_Dialog.h"
#include "CommonMacros.h"
#include "Widgets/Text/SMultiLineEditableText.h"

#define LOCTEXT_NAMESPACE "SPlotNode_Dialog"

void SPlotNode_Dialog::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
}

TSharedRef<SWidget> SPlotNode_Dialog::CreateNodeContentArea()
{
	CachedContentArea = SGraphNode::CreateNodeContentArea();

	// 限制节点宽度
	return SAssignNew(CachedContentArea, SBox)
		.WidthOverride(WIDTH_NODE)
		[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				[
					CachedContentArea.ToSharedRef()
				]
		];
}

void SPlotNode_Dialog::CreateBelowPinControls(TSharedPtr<SVerticalBox> MainBox)
{
	auto DialogNode = Cast<UPlotNode_Dialog>(GraphNode);
	auto DialogData = Cast<UPlotData_Dialog>(DialogNode->GetSource());

	check(DialogData);

	// 渲染所有对话行
	for (int32 Index = 0; Index < DialogData->DialogLines.Num(); Index++)
	{
		FPlotDialogLine& LineRef = DialogData->DialogLines[Index];

		MainBox->AddSlot()
			.AutoHeight()
			.Padding(5)
			[
				SNew(SVerticalBox)

					// 说话人（可编辑）
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						[
							SNew(SMultiLineEditableText)
								.Text(FText::FromString(LineRef.Speaker))
								.OnTextCommitted_Lambda([&LineRef](const FText& NewText, ETextCommit::Type CommitType)
									{
										LineRef.Speaker = NewText.ToString();
									})
								.TextStyle(&DIALOG_SPEAKER_TEXT_STYLE)
								.Font(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 10))
						]
					]

					// 内容（可编辑）
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						[
							SNew(SMultiLineEditableText)
								.Text(FText::FromString(LineRef.Content))
								.OnTextCommitted_Lambda([&LineRef](const FText& NewText, ETextCommit::Type CommitType)
									{
										LineRef.Content = NewText.ToString();
									})
								.TextStyle(&DIALOG_CONTENT_TEXT_STYLE)
								.AutoWrapText(true) // 自动换行
								.Font(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 8))
						]
					]
			];
	}

	// "+" 按钮
	MainBox->AddSlot()
		.AutoHeight()
		.Padding(5.f)
		[
			SNew(SButton)
				.Text(NSLOCTEXT("PlotNodeDialog", "AddDialogLine", "+ 添加对话行"))
				.OnClicked(this, &SPlotNode_Dialog::OnAddDialogLineClicked)
		];
}

FReply SPlotNode_Dialog::OnAddDialogLineClicked()
{
	// 添加事务
	const FScopedTransaction Transaction(LOCTEXT("AddDialog", "Add Dialog"));

	auto DialogNode = Cast<UPlotNode_Dialog>(GraphNode);
	auto DialogData = Cast<UPlotData_Dialog>(DialogNode->GetSource());
	if (!DialogData) return FReply::Handled();

	// 手动标记，可被事务系统记录
	DialogData->Modify();

	FPlotDialogLine NewLine;
	NewLine.Speaker = TEXT("说话人");
	NewLine.Content = TEXT("内容");

	DialogData->DialogLines.Add(NewLine);

	// 序列化保存
	DialogData->DoTransacted();
	return FReply::Handled();
}
#undef LOCTEXT_NAMESPACE
