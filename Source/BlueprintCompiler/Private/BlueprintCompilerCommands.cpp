// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlueprintCompilerCommands.h"

#define LOCTEXT_NAMESPACE "FBlueprintCompilerModule"

void FBlueprintCompilerCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "BlueprintCompiler", "Bring up BlueprintCompiler window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
