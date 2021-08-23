// Copyright Epic Games, Inc. All Rights Reserved.

#include "TurnBasedSystem.h"
#include "TurnBasedSystemEdMode.h"

#define LOCTEXT_NAMESPACE "ITurnBasedSystemModule" // A must for the LOCTEXT Macro

void ITurnBasedSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FTurnBasedSystemEdMode>(FTurnBasedSystemEdMode::EM_TurnBasedSystemEdModeId, LOCTEXT("TurnBasedSystemEdModeName", "TurnBasedSystemEdMode"), FSlateIcon(), true);
}

void ITurnBasedSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FTurnBasedSystemEdMode::EM_TurnBasedSystemEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(ITurnBasedSystemModule, TurnBasedSystem)