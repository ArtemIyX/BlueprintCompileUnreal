// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "BlueprintCompilerStyle.h"

class FBlueprintCompilerCommands : public TCommands<FBlueprintCompilerCommands>
{
public:

	FBlueprintCompilerCommands()
		: TCommands<FBlueprintCompilerCommands>(TEXT("BlueprintCompiler"), NSLOCTEXT("Contexts", "BlueprintCompiler", "BlueprintCompiler Plugin"), NAME_None, FBlueprintCompilerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};