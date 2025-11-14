#pragma once

class FJsonSerializationHelper
{

public:
	static void SerializePlotMapToFile(const TMap<uint32, class UPlotDataBase*>& PlotDataMap, const FString& Filepath);
	static bool DeserializePlotMapFromFile(TMap<uint32, UPlotDataBase*>& OutMap, const FString& FilePath, UObject* Outer);
	

private:
	static const FName CLASS_META_JsonSerialization;
	static const FName CLASS_META_PreJsonSerialization;
	static const FName PROPERTY_META_IgnoreJsonSerialization;

	static TSharedPtr<FJsonObject> Serialize(UObject* InObject);
	static TSharedPtr<FJsonObject> SerializePlotMap(const TMap<uint32, class UPlotDataBase*>& PlotDataMap);

	static void Deserialize(UObject* InObject, const TSharedPtr<FJsonObject>& JsonObject);
	static void DeserializePlotMap(TMap<uint32, UPlotDataBase*>& OutMap, const TSharedPtr<FJsonObject>& JsonObject, UObject* Outer);
};