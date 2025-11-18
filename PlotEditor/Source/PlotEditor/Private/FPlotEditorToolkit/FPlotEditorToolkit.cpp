#include "FPlotEditorToolkit.h"


#include "SGraphPanel.h"
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
#include "UPlotData/UPlotData_Choice.h"
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

	LoadPlotsData(); // 从Json加载数据
	InitPlotGraph(); // 创建节点和连接
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

UPlotNode_Choice* FPlotEditorToolkit::Action_NewChoice(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	static const int32 NodeDistance = 60;

	if (ParentGraph == nullptr) ParentGraph = PlotGraphView->GetGraphObj();

	// 准备数据
	UPlotData_Choice* ChoiceData;
	{
		ChoiceData = NewObject<UPlotData_Choice>(EditorContext.Get());
		ChoiceData->Initialize(GetNextNodeID(), SharedThis(this));
		ChoiceData->IsDeleted = true;

		// 先新增俩选项
		ChoiceData->Options.Add(TEXT("选项1"));
		ChoiceData->Options.Add(TEXT("选项2"));

		ChoiceData->NextPlotList.Add(0);
		ChoiceData->NextPlotList.Add(0);

		ChoiceData->SetFlags(RF_Transactional);
	}

	// 创建节点
	UPlotNode_Choice* ChoiceNode;
	{
		ChoiceNode = DuplicateObject<UPlotNode_Choice>(UPlotNode_Choice::StaticClass()->GetDefaultObject<UPlotNode_Choice>(), ParentGraph);
		ChoiceNode->SetFlags(RF_Transactional);
		ChoiceNode->CreateNewGuid();
		ChoiceNode->PostPlacedNewNode();
		ChoiceNode->AllocateDefaultPins();
	}

	// 绑定数据
	ChoiceNode->Bind(ChoiceData);
	// 根据数据创建引脚
	ChoiceNode->CreateOutputPinsByOptions();

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

	ChoiceNode->NodePosX = XLocation;
	ChoiceNode->NodePosY = Location.Y;
	ChoiceNode->SnapToGrid(GetDefault<UEditorStyleSettings>()->GridSnapSize);

	FScopedTransaction Transaction(INVTEXT("New Dialog"));

	EditorContext->Modify();
	ChoiceData->Modify();
	ChoiceData->IsDeleted = false; // Undo 时，删除对应的编辑器数据文件
	EditorContext->PlotDataMap.Add(ChoiceData->ID, ChoiceData);

	ParentGraph->Modify();
	ParentGraph->AddNode(ChoiceNode, true, bSelectNewNode);
	ChoiceNode->AutowireNewNode(FromPin);

	return ChoiceNode;
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

void FPlotEditorToolkit::LoadPlotsData()
{
	EditorContext->PlotDataMap.Empty();

	const FString FilePath = GetCurrentAssetJsonFilePath();

	if (!FJsonSerializationHelper::DeserializePlotMapFromFile(EditorContext->PlotDataMap, FilePath, EditorContext))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load plot map JSON: %s"), *FilePath);
		return;
	}

	EditorContext->InitializeNextIDByPlotDataMap(); // 初始化ID
}

void FPlotEditorToolkit::ConnectDialogNode(UPlotData_Dialog* DialogData)
{
	uint32 SrcID = DialogData->ID;
	uint32 DestID = DialogData->NextPlot;

	if (DestID == 0) return;

	UPlotDataBase** Found = EditorContext->PlotDataMap.Find(DestID);
	if (!Found || !(*Found))
	{
		UE_LOG(LogTemp, Warning, TEXT("Dialog %u -> NextPlot %u not found"), SrcID, DestID);
		return;
	}

	auto SrcNode = Cast<UPlotNode_Dialog>(DialogData->PlotNode.Get());
	auto DestNode = (*Found)->PlotNode.Get();

	if (!SrcNode || !DestNode) return;

	UEdGraphPin* SrcOut = SrcNode->GetNextPin();
	if (!SrcOut) return;

	// 目标节点为 Dialog 节点
	if (auto DestDialog = Cast<UPlotNode_Dialog>(DestNode))
	{
		UEdGraphPin* DestIn = DestDialog->GetPrevPin();
		if (DestIn)
		{
			SrcOut->MakeLinkTo(DestIn);
		}
		return;
	}

	// 目标节点为 Choice 节点
	if (auto DestChoice = Cast<UPlotNode_Choice>(DestNode))
	{
		UEdGraphPin* DestIn = DestChoice->GetPrevPin();
		if (DestIn)
		{
			SrcOut->MakeLinkTo(DestIn);
		}
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Dialog %u cannot connect to unknown node type %u"), SrcID, DestID);
}

void FPlotEditorToolkit::ConnectChoiceNode(UPlotData_Choice* ChoiceData)
{
	uint32 SrcID = ChoiceData->ID;
	auto ChoiceNode = Cast<UPlotNode_Choice>(ChoiceData->PlotNode.Get());
	if (!ChoiceNode) return;

	const TArray<UEdGraphPin*>& OutPins = ChoiceNode->GetChoicePins();

	for (int32 OptionIndex = 0; OptionIndex < ChoiceData->NextPlotList.Num(); OptionIndex++)
	{
		uint32 TargetID = ChoiceData->NextPlotList[OptionIndex];
		if (TargetID == 0) continue; // 无效目标

		// 寻找SrcPin
		if (!OutPins.IsValidIndex(OptionIndex))
		{
			UE_LOG(LogTemp, Warning, TEXT("Choice node %s has no OutPin[%d]"), *ChoiceNode->GetName(), OptionIndex);
			continue;
		}

		UEdGraphPin* SrcPin = OutPins[OptionIndex];
		check(SrcPin);

		// 寻找目标节点数据和目标节点
		UPlotDataBase** TargetDataPtr = EditorContext->PlotDataMap.Find(TargetID);
		if (!TargetDataPtr || !(*TargetDataPtr))
		{
			UE_LOG(LogTemp, Warning, TEXT("Choice option %d -> target %u not found"), OptionIndex, TargetID);
			continue;
		}
		UPlotNodeBase* TargetNode = (*TargetDataPtr)->PlotNode.Get();
		if (!TargetNode)
		{
			UE_LOG(LogTemp, Warning, TEXT("Target node %u has no PlotNode"), TargetID);
			continue;
		}


		// 找目标节点的PrevPin
		UEdGraphPin* TargetPrevPin = nullptr;

		if (UPlotNode_Dialog* TargetDialog = Cast<UPlotNode_Dialog>(TargetNode))
		{
			TargetPrevPin = TargetDialog->GetPrevPin();
		}
		else if (UPlotNode_Choice* TargetChoice = Cast<UPlotNode_Choice>(TargetNode))
		{
			TargetPrevPin = TargetChoice->GetPrevPin();
		}

		if (!TargetPrevPin)
		{
			UE_LOG(LogTemp, Warning, TEXT("Target %u has no PrevPin"), TargetID);
			continue;
		}

		// 创建连接
		SrcPin->MakeLinkTo(TargetPrevPin);
	}
}

void FPlotEditorToolkit::InitPlotGraph()
{
	// 生成节点
	for (const auto& KeyValue : EditorContext->PlotDataMap)
	{
		uint32 NodeID = KeyValue.Key;

		UPlotDataBase* PlotData = KeyValue.Value;
		UClass* NodeClass = PlotData->GetNodeClass();

		UEdGraphNode* Node = FEdGraphSchemaAction_NewNode::CreateNode(
			PlotGraphView->GetGraphObj(),
			nullptr,
			PlotData->NodePos, // 根据Json数据设置位置
			NodeClass->GetDefaultObject<UPlotNodeBase>() // 用类默认对象生成节点
		);

		// 初始化节点数据
		PlotData->Initialize(NodeID, SharedThis(this));

		// 绑定数据
		auto PlotNode = Cast<UPlotNodeBase>(Node);
		PlotNode->Bind(PlotData);

		// 如果是Choice节点，要根据数据构建引脚
		if (auto ChoiceNode = Cast<UPlotNode_Choice>(PlotNode))
		{
			ChoiceNode->CreateOutputPinsByOptions();
		}
	}

	// 创建连接（支持 Dialog 和 Choice）
	for (const auto& KeyValue : EditorContext->PlotDataMap)
	{
		UPlotDataBase* PlotData = KeyValue.Value;

		if (auto DialogData = Cast<UPlotData_Dialog>(PlotData))
		{
			ConnectDialogNode(DialogData);
			continue;
		}
		if (auto ChoiceData = Cast<UPlotData_Choice>(PlotData))
		{
			ConnectChoiceNode(ChoiceData);
			continue;
		}
	}

	// 清空初始选中
	PlotGraphView->GetGraphEditorPtr()->RegisterActiveTimer(0.0f, FWidgetActiveTimerDelegate::CreateLambda(
		[this](double, float)
		{
			for (const auto& KeyValue : EditorContext->PlotDataMap)
			{
				const auto PlotData = KeyValue.Value;
				if (PlotData->PlotNode.IsValid())
				{
					if (!PlotGraphView->GetGraphEditorPtr()->GetGraphPanel()->GetNodeWidgetFromGuid(PlotData->PlotNode->NodeGuid))
						return EActiveTimerReturnType::Continue;
				}
			}
			PlotGraphView->GetGraphEditorPtr()->ClearSelectionSet();
			return EActiveTimerReturnType::Stop;
		}
	));
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
