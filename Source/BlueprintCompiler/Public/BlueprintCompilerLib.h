#pragma once
#include "CoreMinimal.h"
#include "BlueprintCompilerLib.generated.h"

UCLASS()
class BLUEPRINTCOMPILER_API UBlueprintCompilerLib : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="UBlueprintCompilerLib")
	static TArray<FString> GetAllBlueprintPaths();

	UFUNCTION(BlueprintCallable, Category="UBlueprintCompilerLib")
	static FString CompileBlueprints(const TArray<FString>& InPathes);
};

