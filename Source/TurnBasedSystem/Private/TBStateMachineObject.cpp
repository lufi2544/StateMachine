// Fill out your copyright notice in the Description page of Project Settings.


#include "TBStateMachineObject.h"

void UTBStateMachineObject::Init(UTBStateMachine& InStateMachine)
{
	StateMachine = &InStateMachine;
}
