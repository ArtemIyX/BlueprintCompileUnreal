// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlueprintCompiler.h"
#include "BlueprintCompilerStyle.h"
#include "BlueprintCompilerCommands.h"
#include "BlueprintCompilerLib.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName BlueprintCompilerTabName("BlueprintCompiler");

#define LOCTEXT_NAMESPACE "FBlueprintCompilerModule"

void FBlueprintCompilerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FBlueprintCompilerStyle::Initialize();
	FBlueprintCompilerStyle::ReloadTextures();

	FBlueprintCompilerCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FBlueprintCompilerCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FBlueprintCompilerModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FBlueprintCompilerModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(BlueprintCompilerTabName, FOnSpawnTab::CreateRaw(this, &FBlueprintCompilerModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FBlueprintCompilerTabTitle", "BlueprintCompiler"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FBlueprintCompilerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FBlueprintCompilerStyle::Shutdown();

	FBlueprintCompilerCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(BlueprintCompilerTabName);
}

TSharedRef<SDockTab> FBlueprintCompilerModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	auto OnCompileBlueprintsClicked = [this]() mutable -> FReply
	{
		TArray<FString> path = UBlueprintCompilerLib::GetAllBlueprintPaths();
		FString res = UBlueprintCompilerLib::CompileBlueprints(path);
		this->TextBoxContainer->ClearChildren();
		TextBoxContainer->AddSlot()
				.AutoHeight()
				.Padding(2.0f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(*res))
				];
		return FReply::Handled();
	};
	
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SVerticalBox)

			// Кнопка
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(10.0f)
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("Compile Blueprints")))
				.OnClicked_Lambda(OnCompileBlueprintsClicked)
			]

			// Текстовое поле
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			.Padding(10.0f)
			[
				SNew(SScrollBox)
				+ SScrollBox::Slot()
				[
					SAssignNew(TextBoxContainer, SVerticalBox)
				]
			]
		];
}

void FBlueprintCompilerModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(BlueprintCompilerTabName);
}

void FBlueprintCompilerModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FBlueprintCompilerCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FBlueprintCompilerCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBlueprintCompilerModule, BlueprintCompiler)