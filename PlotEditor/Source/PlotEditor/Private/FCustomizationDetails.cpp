#include "FCustomizationDetails.h"

#include "Misc/AssertionMacros.h"
#include "DetailLayoutBuilder.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "UPlotData/UPlotData_Dialog.h"

TSharedRef<IPropertyTypeCustomization> FPlotDialogLineCustomization::MakeInstance()
{
    return MakeShareable(new FPlotDialogLineCustomization);
}

void FPlotDialogLineCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    // 获取 Speaker 的句柄
    TSharedPtr<IPropertyHandle> SpeakerHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FPlotDialogLine, Speaker));

    HeaderRow.NameContent()
        [
            StructPropertyHandle->CreatePropertyNameWidget()
        ]
        .ValueContent()
        [
            SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.VAlign(VAlign_Center)
                [
                    SNew(STextBlock)
						.Text(this, &FPlotDialogLineCustomization::GetSpeakerText, SpeakerHandle)
                        .Font(IDetailLayoutBuilder::GetDetailFont())
                        .ColorAndOpacity(FSlateColor::UseSubduedForeground())
                ]
        ];
}

FText FPlotDialogLineCustomization::GetSpeakerText(TSharedPtr<IPropertyHandle> SpeakerHandle) const
{
    if (!SpeakerHandle.IsValid() || !SpeakerHandle->IsValidHandle())
    {
		return FText::GetEmpty();
    }

    FString Speaker;
    if (SpeakerHandle->GetValue(Speaker) == FPropertyAccess::Success)
    {
        return FText::FromString(Speaker);
    }

    return FText::GetEmpty();
}

void FPlotDialogLineCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    uint32 PropertyNum = 0;
    StructPropertyHandle->GetNumChildren(PropertyNum);

    for (uint32 Index = 0; Index < PropertyNum; ++Index)
    {
        TSharedPtr<IPropertyHandle> ChildHandle = StructPropertyHandle->GetChildHandle(Index);
        check(ChildHandle);

        ChildBuilder.AddProperty(ChildHandle.ToSharedRef());
    }
}

TSharedRef<IDetailCustomization> FPlotDataDetailCustomization::MakeInstance()
{
    return MakeShareable(new FPlotDataDetailCustomization);
}

void FPlotDataDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    // 获取正在编辑的对象数组
    TArray<TWeakObjectPtr<UObject>> Objects;
    DetailBuilder.GetObjectsBeingCustomized(Objects);

    // 新增 Category
    IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Plot Info");

    for (TWeakObjectPtr<UObject> Obj : Objects)
    {
        UPlotDataBase* PlotData = Cast<UPlotDataBase>(Obj.Get());
        if (!PlotData) continue;

        const UEnum* EnumPtr = StaticEnum<EPlotNodeType>();
        FText TypeText = FText::Format(
            INVTEXT("节点类型: {0}"),
            EnumPtr->GetDisplayNameTextByValue((int64)PlotData->NodeType)
        );

        FText DisplayName = FText::Format(
            INVTEXT("【{0}】{1}"),
            FText::AsNumber(PlotData->ID),
            FText::FromString(PlotData->Comment)
        );

        // 添加一行
        Category.AddCustomRow(FText::FromString(TEXT("Plot Info")))
        .NameContent()
        [
            SNew(STextBlock)
                .Text(DisplayName)
        ]
        .ValueContent()
        [
            SNew(STextBlock)
                .Text(TypeText)
        ];
    }
}
