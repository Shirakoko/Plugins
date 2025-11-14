#include "FJsonSerializationHelper.h"

#include "JsonObjectConverter.h"
#include "UPlotData/UPlotDataBase.h"
#include "UPlotData/UPlotData_Dialog.h"

/*static*/ const FName FJsonSerializationHelper::CLASS_META_JsonSerialization = "JsonSerialization";
/*static*/ const FName FJsonSerializationHelper::CLASS_META_PreJsonSerialization = "PreJsonSerialization";
/*static*/ const FName FJsonSerializationHelper::PROPERTY_META_IgnoreJsonSerialization = "IgnoreJsonSerialization";

TSharedPtr<FJsonObject> FJsonSerializationHelper::Serialize(UObject* InObject)
{
	TSharedPtr<FJsonObject> Root = MakeShared<FJsonObject>();

	UClass* InClass = InObject->GetClass();
	check(InClass->HasMetaData(CLASS_META_JsonSerialization));

	UE_LOG(LogTemp, Log, TEXT("[FJsonSerializationHelper::Serialize] Export Object <%s>"), *InClass->GetDisplayNameText().ToString());

	// 序列化前调用PreFuncFunction
	if (auto PreFuncName = InClass->FindMetaData(CLASS_META_PreJsonSerialization))
	{
		if (const UFunction* PreFuncFunction = InObject->FindFunction(FName(*PreFuncName)))
		{
			DECLARE_DELEGATE(FPreJsonSerializationDSignature);
			FPreJsonSerializationDSignature::CreateUFunction(InObject, PreFuncFunction->GetFName()).Execute();
		}
	}

	// 序列化所需属性
	for (TFieldIterator<FProperty> PropertyIt(InClass); PropertyIt; ++PropertyIt)
	{
		if (FProperty* Property = *PropertyIt)
		{
			if (Property->HasMetaData(PROPERTY_META_IgnoreJsonSerialization)) continue;
			const void* PropertyValue = Property->ContainerPtrToValuePtr<void>(InObject);
			auto Value = FJsonObjectConverter::UPropertyToJsonValue(Property, PropertyValue);
			Root->SetField(Property->GetName(), Value);
		}
	}
	return Root;
}

TSharedPtr<FJsonObject> FJsonSerializationHelper::SerializePlotMap(const TMap<uint32, class UPlotDataBase*>& PlotDataMap)
{
	TSharedPtr<FJsonObject> MapJson = MakeShared<FJsonObject>();

	for (const auto& It : PlotDataMap)
	{
		uint32 Key = It.Key;
		UPlotDataBase* ValueObj = It.Value;

		if (!ValueObj)
		{
			// 空对象 -> 写 null
			MapJson->SetField(FString::FromInt(Key), MakeShared<FJsonValueNull>());
			continue;
		}

		// 递归序列化 UObject（调用已有的 Serialize）
		TSharedPtr<FJsonObject> DataJson = Serialize(ValueObj);

		MapJson->SetObjectField(FString::FromInt(Key), DataJson);
	}

	return MapJson;
}

void FJsonSerializationHelper::SerializePlotMapToFile(const TMap<uint32, UPlotDataBase*>& PlotDataMap, const FString& Filepath)
{
	// 序列化成 JsonObject
	TSharedPtr<FJsonObject> RootJson = FJsonSerializationHelper::SerializePlotMap(PlotDataMap);

	// 写入字符串
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(RootJson.ToSharedRef(), Writer);

	// 保存文件
	FFileHelper::SaveStringToFile(JsonString, *Filepath, FFileHelper::EEncodingOptions::ForceUTF8);
}

void FJsonSerializationHelper::Deserialize(UObject* InObject, const TSharedPtr<FJsonObject>& JsonObject)
{
	// 序列化时，一定不会序列化带`PROPERTY_META_IgnoreJsonSerialization`标记的属性。所以反序列化时，不用特殊处理
	FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), InObject->GetClass(), InObject);
}

void FJsonSerializationHelper::DeserializePlotMap(
	TMap<uint32, UPlotDataBase*>& OutMap,
	const TSharedPtr<FJsonObject>& JsonObject,
	UObject* Outer)
{
	OutMap.Empty();

	for (const auto& Pair : JsonObject->Values)
	{
		uint32 Key = FCString::Atoi(*Pair.Key);

		if (!Pair.Value.IsValid() || !Pair.Value->AsObject().IsValid())
			continue;

		TSharedPtr<FJsonObject> ValueObj = Pair.Value->AsObject();

		// 用临时对象读取 NodeType，虽然标注了UCLASS(Abstract)，但C++仍然允许创建对象
		UPlotDataBase* TempBase = NewObject<UPlotDataBase>(Outer);
		Deserialize(TempBase, ValueObj);

		EPlotNodeType NodeType = TempBase->NodeType;

		// 根据 NodeType 创建真正子类
		UPlotDataBase* NewObj = nullptr;

		switch (NodeType)
		{
		case EPlotNodeType::Dialog:
			NewObj = NewObject<UPlotData_Dialog>(Outer);
			break;

		case EPlotNodeType::Choice:
			//TODO: NewObj = NewObject<UPlotData_Choice>(Outer);
			break;

		default:
			NewObj = NewObject<UPlotDataBase>(Outer);
			break;
		}

		// 再次执行反序列化，读入所有字段
		Deserialize(NewObj, ValueObj);

		// 放入 Map
		OutMap.Add(Key, NewObj);
	}
}


bool FJsonSerializationHelper::DeserializePlotMapFromFile(TMap<uint32, UPlotDataBase*>& OutMap,  const FString& FilePath, UObject* Outer)
{
	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
		return false;

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
		return false;

	DeserializePlotMap(OutMap, JsonObject, Outer);
	return true;
}
