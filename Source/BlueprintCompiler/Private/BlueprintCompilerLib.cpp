#include "BlueprintCompilerLib.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet2/KismetEditorUtilities.h"

TArray<FString> UBlueprintCompilerLib::GetAllBlueprintPaths()
{
	TArray<FString> BlueprintPaths;
	
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	
	FARFilter Filter;
	
	Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName()); 
	Filter.bRecursivePaths = true;
	Filter.PackagePaths.Add("/Game");
	
	TArray<FAssetData> AssetData;
	AssetRegistry.GetAssets(Filter, AssetData);
	
	for (const FAssetData& Data : AssetData)
	{
		FString BlueprintPath = Data.GetSoftObjectPath().ToString();
		BlueprintPaths.Add(BlueprintPath);
	}

	return BlueprintPaths;
}

FString UBlueprintCompilerLib::CompileBlueprints(const TArray<FString>& InPathes)
{
	FString logs = "\n-------- Logs --------";
	FString warnings = "\n-------- Warnings --------";
	FString errors = "\n-------- Errors --------";
	for (auto el : InPathes)
	{
		UBlueprint* bp = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), nullptr, *el));
		if(bp == nullptr)
		{
			continue;
		}

		FCompilerResultsLog result;
		FKismetEditorUtilities::CompileBlueprint(bp, EBlueprintCompileOptions::SkipSave, &result);

		for (TSharedRef<FTokenizedMessage> msg : result.Messages)
		{
			auto messageString = el + ": " + msg.Get().ToText().ToString();
			switch (msg.Get().GetSeverity())
			{
				case EMessageSeverity::Error:
					errors += "\n" + messageString;
					break;
				case EMessageSeverity::PerformanceWarning:
				case EMessageSeverity::Warning:
					warnings += "\n" +messageString;
					break;
				case EMessageSeverity::Info:
					logs += "\n" + messageString;
				default: break;
			}
		}
	}
	return logs + warnings + errors;
}
