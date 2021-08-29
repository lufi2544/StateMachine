// Fill out your copyright notice in the Description page of Project Settings.


#include "TBStateMachineTypes.h"

void FTBStateMachineStateInfo::SubmitState()
{
	CurrentState = NextState;
	PreviouseStatesStepsNum += CurrentPhaseSteps.Num();
	CurrentPhaseSteps = NextPhaseSteps;
	NextPhaseSteps.Empty();
}
