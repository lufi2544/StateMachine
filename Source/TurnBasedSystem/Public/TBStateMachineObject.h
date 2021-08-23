// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TBStateMachineObject.generated.h"

class UTBStateMachine;

/**
 * 
 */
UCLASS()
class TURNBASEDSYSTEM_API UTBStateMachineObject : public UObject
{
	GENERATED_BODY()
	
protected:
	
	/* The State Machine that this Object belongs to. */
	UPROPERTY(BlueprintReadOnly)
	UTBStateMachine* StateMachine;

public:

	virtual void Init(UTBStateMachine& InStateMachine);
};