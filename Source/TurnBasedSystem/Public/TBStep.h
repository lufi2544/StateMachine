// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TBStateMachineObject.h"
#include "TBStep.generated.h"

class UTBStateMachine;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStepFinished, uint8);

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew, CollapseCategories)
class TURNBASEDSYSTEM_API UTBStep : public UTBStateMachineObject
{
	GENERATED_BODY()

	//----PROPERTIES----
	uint8 Id;
	//----  ----

public:

	UTBStep();

	/* Inits the Step */
	 virtual void InitAndExecute(UTBStateMachine& StateMachine, uint8 Id);


	//---- DELEGATES ----
	FOnStepFinished OnStepFinished;
	//----  ----


	UFUNCTION(BlueprintCallable)
	void EndStep();

	UFUNCTION(BlueprintImplementableEvent)
	void InitStep();

protected:

	

	//---- UStep Interface ----
	UFUNCTION(BlueprintNativeEvent)
	void ExecuteStep();
	virtual void ExecuteStep_Implementation();

	virtual void Native_InitStep();


};
