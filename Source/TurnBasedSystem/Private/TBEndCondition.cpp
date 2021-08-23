// Fill out your copyright notice in the Description page of Project Settings.


#include "TBEndCondition.h"
#include "TurnBasedStateMachine.h"

UTBEndCondition::UTBEndCondition()
{
	
}

bool UTBEndCondition::GetConditionResult()
{
	return ExecuteAndOutputCondition();
}

bool UTBEndCondition::ExecuteAndOutputCondition_Implementation()
{
	return true;
}