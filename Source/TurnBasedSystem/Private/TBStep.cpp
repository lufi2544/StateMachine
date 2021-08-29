// Fill out your copyright notice in the Description page of Project Settings.


#include "TBStep.h"
#include "TurnBasedStateMachine.h"

UTBStep::UTBStep()
{
	Id = -1;
	bInit = false;
}

void UTBStep::InitAndExecute(UTBStateMachine& InStateMachine, uint8 InId)
{
	if(!bInit)
	{
		Id = InId;
		// Init
		InitSMObject(InStateMachine);
		Native_InitStep();
		K2_InitStep();
		bInit = true;
	}

	// Execute
	Native_ExecuteStep();
	K2_ExecuteStep();
}

void UTBStep::EndStep()
{
	OnStepFinished.Broadcast(Id);
}

void UTBStep::Native_ExecuteStep()
{

}

void UTBStep::Native_InitStep()
{
	
}