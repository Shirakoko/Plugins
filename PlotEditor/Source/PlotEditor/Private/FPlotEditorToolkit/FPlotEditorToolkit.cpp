#include "FPlotEditorToolkit.h"

#include "UPlotEditorEntry/UPlotEditorEntry.h"

#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "GraphEditor.h"
#include "Framework/Docking/TabManager.h"
#include "UPlotEditorGraph/UPlotEditorGraph.h"
#include "UPlotEditorGraphSchema/UPlotEditorGraphSchema.h"
#include "SPlotEditorWidget/SPlotGraphView.h"
#include "UEditorContext.h"

static const FName TabID_PlotGraphView = "PlotGraphView";

#define LOCTEXT_NAMESPACE "PlotEditor"

void FPlotEditorToolkit::InitPlotEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UPlotEditorEntry* InPlotAsset)
{
	EditorContext = NewObject<UEditorContext>();
	EditorContext->AddToRoot(); // 防止被GC
	EditorContext->Toolkit = SharedThis(this); // 保存变量
	EditorContext->SetFlags(RF_Transactional);

	// 创建编辑器的标签页布局
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("Standalone_PlotEditor_PlotGraph_Layout_v1")
		->AddArea(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split(
				FTabManager::NewStack() // 创建标签页堆栈
				->AddTab(TabID_PlotGraphView, ETabState::OpenedTab) // 添加剧情图视图标签页并默认打开
			)
		);

	// 保存当前编辑的资产引用
	CurrentAsset = InPlotAsset;

	// 初始化资产编辑器
	InitAssetEditor(
		Mode,                    // 编辑器模式
		InitToolkitHost,         // 工具包宿主（决定是否嵌入关卡编辑器）
		"PlotEditor",            // 编辑器应用标识符
		Layout,                  // 标签页布局
		true,                    // 创建默认独立菜单
		true,                    // 创建默认工具栏
		CurrentAsset             // 要编辑的资产对象
	);
}

void FPlotEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	// 在"窗口 > 工作区"菜单下创建一个分类
	MenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(INVTEXT("Plot Editor"));

	// 创建并注册 PlotGraphView 视图标签页
	CreatePlotGraphView(InTabManager);

	// 调用父类的注册方法
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

void FPlotEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	// 调用父类的注销方法
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	// 注销剧情图视图标签页生成器
	InTabManager->UnregisterTabSpawner(TabID_PlotGraphView);
}

void FPlotEditorToolkit::CreatePlotGraphView(const TSharedRef<FTabManager>& InTabManager)
{
	SAssignNew(PlotGraphView, SPlotGraphView, SharedThis(this));

	InTabManager->RegisterTabSpawner(TabID_PlotGraphView,
		FOnSpawnTab::CreateLambda([=, this](const FSpawnTabArgs&)
			{
				// 创建可停靠标签页，将Slate控件添加到标签页内容
				return SNew(SDockTab)
					[
						PlotGraphView.ToSharedRef()
					];
			}
		)
	)
	.SetDisplayName(INVTEXT("Plots Graph")) // 设置标签页显示名称
	.SetGroup(MenuCategory.ToSharedRef());  // 设置所属菜单分类
}

#undef LOCTEXT_NAMESPACE
