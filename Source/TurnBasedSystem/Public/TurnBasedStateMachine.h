// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TurnBasedStateMachine.generated.h"

class UTBStep;
class UTBEndCondition; 


/** Current State of the State Machine. */
namespace EStateMachineState
{
	enum Type
	{

		// Not Init yet
		None,

		Init,

		Loop,

		Finished 
	};
};

UENUM(BlueprintType)
enum class EStateMachineEndConditionMode: uint8
{
	Default,
	// None of the Conditions have to be met
	None,

	// All conditions have to be met
	All,

	// Just one condition 
	OnlyOne
};


/**
 * Principal State Machine for the TurnBased plug-in
 */
UCLASS(Blueprintable)
class TURNBASEDSYSTEM_API UTBStateMachine : public UObject
{
	GENERATED_BODY()


	/* Singleton Pointer */
	static UTBStateMachine* SelfReference;

protected:

	/* Main Logic */
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly)
	TArray<UTBStep*> InitSteps;

	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly)
	TArray<UTBStep*> LoopSteps;

	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly)
	TArray<UTBStep*> EndSteps;

	UPROPERTY(EditDefaultsOnly)
	EStateMachineEndConditionMode EndConditionMode;

	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly)
	TArray<UTBEndCondition*> EndConditions;

	/* Current State Machine State */
	TEnumAsByte<EStateMachineState::Type> CurrentState;

public:

	UTBStateMachine();

	// Singleton-kind function to access the TB State machine
	static UTBStateMachine& GetInstance()
	{
		if (!SelfReference)
		{
			SelfReference = NewObject< UTBStateMachine >( GetTransientPackage(), TEXT( " TBStateMachine " ) );
		}

		return *SelfReference;
	}

	static UTBStateMachine& GetInstance(TSubclassOf<UTBStateMachine> InstanceClass)
	{
		if (!SelfReference)
		{
			SelfReference = NewObject< UTBStateMachine >( GetTransientPackage(), InstanceClass, TEXT(" TBStateMachine ") );
		}

		return *SelfReference;
	}

	UFUNCTION(BlueprintCallable)
	static UTBStateMachine* CreateStateMachine(TSubclassOf<UTBStateMachine> StateMachineClass, FName Name);

	/** Inits the State Machine. */
	UFUNCTION(BlueprintCallable, Category = "TurnBased", DisplayName = "Init")
	void K2_Init();

	/** Ends the State Machine. */
	UFUNCTION(BlueprintCallable, Category = "TurnBased", DisplayName = "End")
	void K2_End();

	UPROPERTY(BlueprintReadOnly)
	AActor* Owner;


protected:

	template<typename T>
	static T* CreateTBStateMachine(TSubclassOf<T> StateMachineClass, FName InName);

	void BindStepsCallback(TArray<UTBStep*>& StepsArray, FName FunctionToBind);
	void InitAndExecuteSteps(TArray<UTBStep*>& StepsToExecute);
	/** True if the State Machine can continue to the next State (Init, Loop, End) */
	bool TryContinueToNextState(uint8 StepId);
	void ContinueExecutingSteps(TArray<UTBStep*>& StepsToExecute, uint8 CurrentStepId);

	/** Returns true if the State Machine has not ended. */
	bool IsStateMachineAvailable();
	
	/** Init **/
	void Init();
	virtual void Init_Internal();
	UFUNCTION()
	void OnInitStepFinishCallback(uint8 Id);
	

	/** Loop **/
	virtual bool Loop();
	virtual bool ExecuteEndLoopState();
	UFUNCTION()
	void OnLoopStepFinishCallback(uint8 Id);
	

	/** End **/
	void End();
	virtual void End_Internal();
	UFUNCTION()
	void OnFinishStepFinishCallback(uint8 Id);
	void FinishStateMachine();



};

template<typename T>
T* UTBStateMachine::CreateTBStateMachine(TSubclassOf<T> StateMachineClass, FName InName)
{
	T* StateMachine = nullptr;

	StateMachine = NewObject<T>(GetTransientPackage(), StateMachineClass, InName);

	return StateMachine;
}



