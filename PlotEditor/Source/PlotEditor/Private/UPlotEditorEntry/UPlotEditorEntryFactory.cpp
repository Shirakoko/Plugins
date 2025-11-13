#include "UPlotEditorEntryFactory.h"

#include "UPlotEditorEntry.h"

UPlotEditorEntryFactory::UPlotEditorEntryFactory()
{
	// 允许在编辑器中创建新资源（内容浏览器右键菜单中显示"创建"选项）
	bCreateNew = true;
	// 创建资源后立即打开编辑器进行编辑
	bEditAfterNew = true;
	// 指定工厂负责创建 UPlotEditorEntry 类的对象
	SupportedClass = UPlotEditorEntry::StaticClass();
}

UObject* UPlotEditorEntryFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UPlotEditorEntry* AssetObject = NewObject<UPlotEditorEntry>(InParent, Class, Name, Flags, Context);
	return AssetObject;
}
