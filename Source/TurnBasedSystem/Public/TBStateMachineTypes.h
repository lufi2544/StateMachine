// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TBStateMachineTypes.generated.h"

/** Current State of the State Machine. */
namespace EStateMachineState
{
	enum Type
	{

		// Not Init yet
		None,

		Init,

		Loop,

		Finished
	};
};

UENUM(BlueprintType)
enum class EStateMachineEndConditionMode : uint8
{
	Default,
	// None of the Conditions have to be met
	None,

	// All conditions have to be met
	All,

	// Just one condition 
	OnlyOne
};


class UTBStep;

USTRUCT()
struct FTBStateMachineStateInfo
{
	GENERATED_USTRUCT_BODY();

	FTBStateMachineStateInfo():
		CurrentPhaseSteps({}),
		NextPhaseSteps({}),
		CurrentState(EStateMachineState::None),
		NextState(EStateMachineState::None),
		PreviouseStatesStepsNum(0)
	{}

	// Changes completely the State from the Current State to the Next State
	void SubmitState();

	// TODO check size and align bytes for the memory.
	UPROPERTY()
	TArray<UTBStep*> CurrentPhaseSteps;

	UPROPERTY()
	TArray<UTBStep*> NextPhaseSteps;

	EStateMachineState::Type CurrentState;
	EStateMachineState::Type NextState;
	
	uint8 PreviouseStatesStepsNum;

};


