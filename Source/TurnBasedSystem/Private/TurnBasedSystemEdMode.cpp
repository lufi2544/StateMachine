// Copyright Epic Games, Inc. All Rights Reserved.

#include "TurnBasedSystemEdMode.h"
#include "TurnBasedSystemEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FTurnBasedSystemEdMode::EM_TurnBasedSystemEdModeId = TEXT("EM_TurnBasedSystemEdMode");

FTurnBasedSystemEdMode::FTurnBasedSystemEdMode()
{

}

FTurnBasedSystemEdMode::~FTurnBasedSystemEdMode()
{

}

void FTurnBasedSystemEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FTurnBasedSystemEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FTurnBasedSystemEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FTurnBasedSystemEdMode::UsesToolkits() const
{
	return true;
}




