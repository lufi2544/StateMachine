// Fill out your copyright notice in the Description page of Project Settings.


#include "TBStateMachineObject.h"

UTBStateMachineObject::UTBStateMachineObject()
{
	StateMachine = nullptr;
	bInit = false;
}

void UTBStateMachineObject::InitSMObject(UTBStateMachine& InStateMachine)
{
	if (!bInit)
	{
		StateMachine = &InStateMachine;
		bInit = true;
	}
	
}
