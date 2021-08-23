// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TBStateMachineObject.h"
#include "TBEndCondition.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConditionSuccessful);

class UTBStateMachine;

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew, CollapseCategories)
class TURNBASEDSYSTEM_API UTBEndCondition : public UTBStateMachineObject
{
	GENERATED_BODY()

public:

	UTBEndCondition();

	bool GetConditionResult();

	//----DELEGATES----
	FOnConditionSuccessful OnConditionSuccessful;
	//----  ----


protected:

	/* This function dictates if the condition has passed or not. Must be overrided or implemented in BP
	 *
	 * @return True if the Condition has passed;
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool ExecuteAndOutputCondition();
	virtual bool ExecuteAndOutputCondition_Implementation();

};
