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
#include "UPlotData/UPlotData_Dialog.h"
#include "UPlotNode/UPlotNode_Dialog.h"
#include "Settings/EditorStyleSettings.h"
#include "FJsonSerializationHelper/FJsonSerializationHelper.h"

static const FName TabID_PlotGraphView = "PlotGraphView";
static const FName TabID_DetailPanel = "DetailPanel";

#define LOCTEXT_NAMESPACE "PlotEditor"

FString FPlotEditorToolkit::GetCurrentAssetJsonFilePath()
{
	FString DataPath = FPaths::ProjectDir() + TEXT("Designer/EditorData/TaskEditor");
	FString RelativePath = DataPath + TEXT("/") + CurrentAsset->GetName() + TEXT(".json");

	// 返回绝对路径
	return FPaths::ConvertRelativePathToFull(RelativePath);
}

void FPlotEditorToolkit::InitPlotEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UPlotEditorEntry* InPlotAsset)
{
	EditorContext = NewObject<UEditorContext>();
	EditorContext->AddToRoot(); // 防止被GC
	EditorContext->Toolkit = SharedThis(this); // 保存变量
	EditorContext->InitializeNextID(); // 初始化ID
	EditorContext->SetFlags(RF_Transactional);

	// 创建编辑器的标签页布局
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("Standalone_PlotEditor_PlotGraph_Layout_v1")
		->AddArea(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Vertical)
				->Split
				(
					FTabManager::NewStack() // 创建标签页堆栈
					->AddTab(TabID_PlotGraphView, ETabState::OpenedTab) // 添加剧情图视图标签页并默认打开
				)
			)
			->Split
			(
				FTabManager::NewSplitter()
				->SetSizeCoefficient(0.25f)
				->SetOrientation(Orient_Vertical)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(TabID_DetailPanel, ETabState::OpenedTab)
				)
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

	// 创建各视图标签页
	CreatePlotGraphView(InTabManager);
	CreateDetailsPanel(InTabManager);

	// 调用父类的注册方法
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

void FPlotEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	// 调用父类的注销方法
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	// 注销各视图的标签页生成器
	InTabManager->UnregisterTabSpawner(TabID_PlotGraphView);
	InTabManager->UnregisterTabSpawner(TabID_DetailPanel);
}

UPlotNode_Dialog* FPlotEditorToolkit::Action_NewDialog(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	static const int32 NodeDistance = 60;

	if (ParentGraph == nullptr) ParentGraph = PlotGraphView->GetGraphObj();

	// TODO: 准备数据
	UPlotData_Dialog* DialogData;
	{
		DialogData = NewObject<UPlotData_Dialog>(EditorContext);
		DialogData->Initialize(GetNextNodeID(), SharedThis(this));
		DialogData->IsDeleted = true; // 后面会设置成False触发Transaction
		// TODO: 其他设置

		DialogData->SetFlags(RF_Transactional);
	}

	// 创建节点
	UPlotNode_Dialog* DialogNode;
	{
		DialogNode = DuplicateObject<UPlotNode_Dialog>(UPlotNode_Dialog::StaticClass()->GetDefaultObject<UPlotNode_Dialog>(), ParentGraph);
		DialogNode->SetFlags(RF_Transactional);
		DialogNode->CreateNewGuid();
		DialogNode->PostPlacedNewNode();
		DialogNode->AllocateDefaultPins();
	}

	// 绑定数据
	DialogNode->Bind(DialogData);

	int32 XLocation = Location.X;

	// 针对从输入Pin创建节点时让节点左移，避免重叠
	if (FromPin && FromPin->Direction == EGPD_Input)
	{
		UEdGraphNode* PinNode = FromPin->GetOwningNode();
		const float XDelta = FMath::Abs(PinNode->NodePosX - Location.X);

		if (XDelta < NodeDistance)
		{
			XLocation = PinNode->NodePosX - NodeDistance;
		}
	}

	DialogNode->NodePosX = XLocation;
	DialogNode->NodePosY = Location.Y;
	DialogNode->SnapToGrid(GetDefault<UEditorStyleSettings>()->GridSnapSize);

	FScopedTransaction Transaction(INVTEXT("New Dialog"));

	EditorContext->Modify();
	DialogData->Modify();
	DialogData->IsDeleted = false; // Undo 时，删除对应的编辑器数据文件
	EditorContext->PlotDataMap.Add(DialogData->ID, DialogData);

	ParentGraph->Modify();
	ParentGraph->AddNode(DialogNode, true, bSelectNewNode);
	DialogNode->AutowireNewNode(FromPin);

	return DialogNode;
}

void FPlotEditorToolkit::Action_DeletePlots(TArray<uint32> InPlotIDList)
{
	if (InPlotIDList.IsEmpty()) return;

	const FScopedTransaction Transaction(INVTEXT("Delete Plot"));
	for (uint32 PlotID : InPlotIDList)
	{
		UPlotDataBase* PlotData;

		EditorContext->Modify(); // 标记Dirty，被Transaction系统记录
		EditorContext->PlotDataMap.RemoveAndCopyValue(PlotID, PlotData);

		PlotData->Modify(); // 标记Dirty，被Transaction系统记录
		PlotData->IsDeleted = true;

		TWeakObjectPtr<UPlotNodeBase> PlotNodeToDelete = PlotData->PlotNode;
		if (PlotNodeToDelete.IsValid() && PlotNodeToDelete->CanUserDeleteNode())
		{
			PlotNodeToDelete->DestroyNode(); // 删除图节点
		}
	}
}

void FPlotEditorToolkit::SerializeAllPlots()
{
	if (!EditorContext.Get()) return;
	FJsonSerializationHelper::SerializePlotMapToFile(EditorContext->PlotDataMap, GetCurrentAssetJsonFilePath());
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

void FPlotEditorToolkit::CreateDetailsPanel(const TSharedRef<FTabManager>& InTabManager)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;

	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObjects(TArray<UObject*>{ });

	InTabManager->RegisterTabSpawner(TabID_DetailPanel,
		FOnSpawnTab::CreateLambda([=, this](const FSpawnTabArgs&)
			{
				return SNew(SDockTab)
					[
						DetailsView.ToSharedRef()
					];
			}))
		.SetDisplayName(INVTEXT("Details"))    // 设置标签页显示名称
		.SetGroup(MenuCategory.ToSharedRef()); // 设置所属菜单分类
}

int32 FPlotEditorToolkit::GetNextNodeID()
{
	check(EditorContext);
	return EditorContext->NextID++;
}

#undef LOCTEXT_NAMESPACE
