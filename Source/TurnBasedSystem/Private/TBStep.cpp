// Fill out your copyright notice in the Description page of Project Settings.


#include "TBStep.h"
#include "TurnBasedStateMachine.h"

UTBStep::UTBStep()
{
	Id = 0;
}

void UTBStep::InitAndExecute(UTBStateMachine& InStateMachine)
{
	Init(InStateMachine);
	ExecuteStep();
}


void UTBStep::EndStep()
{
	OnStepFinished.Broadcast(Id);
}

void UTBStep::ExecuteStep_Implementation()
{
	
}


void UTBStep::Native_InitStep()
{

	// Do Stuff Here

	InitStep();


}