// Fill out your copyright notice in the Description page of Project Settings.


#include "TBStep.h"
#include "TurnBasedStateMachine.h"

UTBStep::UTBStep()
{
	Id = -1;
}

void UTBStep::InitAndExecute(UTBStateMachine& InStateMachine, uint8 InId)
{
	Id = InId;
	Init(InStateMachine);
	Native_InitStep();
	Native_ExecuteStep();
}


void UTBStep::EndStep()
{
	OnStepFinished.Broadcast(Id);
}

void UTBStep::Native_ExecuteStep()
{
	K2_ExecuteStep();
}


void UTBStep::Native_InitStep()
{
	// Do Stuff Here
	K2_InitStep();


}