#include "SPlotNode_Dialog.h"
#include "UPlotNode/UPlotNode_Dialog.h"
#include "Widgets/Text/STextBlock.h"

void SPlotNode_Dialog::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	// 确保调用基类的正确构造
	GraphNode = InNode;
	this->SetCursor(EMouseCursor::CardinalCross);
	this->UpdateGraphNode();
}

void SPlotNode_Dialog::CreateBelowPinControls(TSharedPtr<SVerticalBox> MainBox)
{
	if (UPlotNode_Dialog* Node = Cast<UPlotNode_Dialog>(GraphNode))
	{
		for (auto& Line : Node->Lines)
		{
			MainBox->AddSlot()
				.AutoHeight()
				.Padding(2)
				[
					SNew(STextBlock)
						.Text(FText::FromString(FString::Printf(TEXT("%s: %s"), *Line.Speaker, *Line.Content)))
				];
		}
	}
}

void SPlotNode_Dialog::UpdateGraphNode()
{
    // 清除现有的内容
    ContentScale.Bind(this, &SGraphNode::GetContentScale);
    GetOrAddSlot(ENodeZone::Center)
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        [
            SNew(SBorder)
                .BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
                .Padding(0.0f)
                .BorderBackgroundColor(this, &SPlotNode_Dialog::GetBorderBackgroundColor)
                [
                    SNew(SOverlay)

                        // 主内容区域
                        + SOverlay::Slot()
                        .HAlign(HAlign_Fill)
                        .VAlign(VAlign_Fill)
                        [
                            SNew(SVerticalBox)

                                // 标题栏
                                + SVerticalBox::Slot()
                                .AutoHeight()
                                .HAlign(HAlign_Fill)
                                .VAlign(VAlign_Center)
                                .Padding(4.0f, 2.0f)
                                [
                                    SNew(STextBlock)
                                        .Text(this, &SPlotNode_Dialog::GetNodeTitle)
                                        .TextStyle(FAppStyle::Get(), "Graph.StateNode.NodeTitle")
                                        .Justification(ETextJustify::Center)
                                ]

                                // 内容区域
                                + SVerticalBox::Slot()
                                .AutoHeight()
                                .HAlign(HAlign_Fill)
                                .VAlign(VAlign_Fill)
                                .Padding(4.0f)
                                [
                                    CreateNodeContent()
                                ]
                        ]
                ]
        ];

    // 创建引脚控件
    //CreatePinWidgets();
}

TSharedRef<SWidget> SPlotNode_Dialog::CreateNodeContent()
{
    TSharedPtr<SVerticalBox> ContentBox;
    SAssignNew(ContentBox, SVerticalBox);

    if (UPlotNode_Dialog* DialogNode = Cast<UPlotNode_Dialog>(GraphNode))
    {
        if (DialogNode->Lines.Num() > 0)
        {
            for (const auto& Line : DialogNode->Lines)
            {
                ContentBox->AddSlot()
                    .AutoHeight()
                    .Padding(2.0f)
                    [
                        SNew(SBorder)
                            .BorderImage(FAppStyle::GetBrush("Menu.Background"))
                            .Padding(4.0f)
                            [
                                SNew(SVerticalBox)

                                    + SVerticalBox::Slot()
                                    .AutoHeight()
                                    [
                                        SNew(STextBlock)
                                            .Text(FText::FromString(FString::Printf(TEXT("Speaker: %s"), *Line.Speaker)))
                                            .Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
                                    ]

                                    + SVerticalBox::Slot()
                                    .AutoHeight()
                                    [
                                        SNew(STextBlock)
                                            .Text(FText::FromString(Line.Content))
                                            .AutoWrapText(true)
                                    ]
                            ]
                    ];
            }
        }
        else
        {
            // 如果没有对话内容，显示提示
            ContentBox->AddSlot()
                .AutoHeight()
                .Padding(4.0f)
                [
                    SNew(STextBlock)
                        .Text(NSLOCTEXT("PlotEditor", "NoDialogue", "No dialogue lines"))
                        .ColorAndOpacity(FLinearColor::Gray)
                ];
        }
    }

    return ContentBox.ToSharedRef();
}

// 获取节点标题
FText SPlotNode_Dialog::GetNodeTitle() const
{
    if (UPlotNode_Dialog* DialogNode = Cast<UPlotNode_Dialog>(GraphNode))
    {
        if (!DialogNode->NodeName.IsEmpty())
        {
            return FText::FromString(DialogNode->NodeName);
        }
    }
    return NSLOCTEXT("PlotEditor", "DialogNode", "Dialog Node");
}

// 获取边框颜色
FSlateColor SPlotNode_Dialog::GetBorderBackgroundColor() const
{
    return FLinearColor(0.1f, 0.1f, 0.4f);
}
