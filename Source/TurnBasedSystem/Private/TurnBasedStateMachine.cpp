// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedStateMachine.h"
#include "TBStep.h"
#include "TBEndCondition.h"

UTBStateMachine* UTBStateMachine::SelfReference = nullptr;
uint8 UTBStateMachine::StepsIdCounter = 0;

UTBStateMachine::UTBStateMachine()
{
	CurrentState = EStateMachineState::None;
	EndConditionMode = EStateMachineEndConditionMode::Default;
}

void UTBStateMachine::Init()
{
	CurrentState = EStateMachineState::Init;
	BindStepsCallback(InitSteps, FName("OnInitStepFinishCallback"));

	Init_Internal();
}

void UTBStateMachine::Init_Internal()
{
	InitAndExecuteSteps(InitSteps);
}

void UTBStateMachine::InitAndExecuteSteps(TArray<UTBStep*>& StepsToExecute)
{
	// Just running the fist step, the execution of the rest will be handled by the FinishCallback
	if (StepsToExecute.Num() > 0)
	{
		StepsToExecute[0]->InitAndExecute(*this, ++StepsIdCounter);
	}
}

void UTBStateMachine::BindStepsCallback(TArray<UTBStep*>& StepsArray, FName FunctionToBind)
{
	for (UTBStep*& Step : StepsArray)
	{
		Step->OnStepFinished.AddUFunction(this, FName(FunctionToBind));
	}
}

bool UTBStateMachine::Loop()
{
	CurrentState = EStateMachineState::Loop;
	BindStepsCallback(LoopSteps, FName("OnLoopStepFinishCallback"));
	InitAndExecuteSteps(LoopSteps);

	//Recursively call the Loop until we can end the State Machine
	return true;
}

// TODO Discuss: maybe a bool return and checking here the Array bounds validation??
void UTBStateMachine::ContinueExecutingSteps(TArray<UTBStep*>& StepsToExecute, uint8 CurrentStepId)
{
	StepsToExecute[ CurrentStepId ]->InitAndExecute(*this, ++StepsIdCounter);
}

bool UTBStateMachine::TryContinueToNextState(uint8 StepId)
{
	TArray<UTBStep*> CurrentPhaseSteps;
	TArray<UTBStep*> NextPhaseSteps;
	EStateMachineState::Type NextState = EStateMachineState::None;


	// TODO make this a function to extract the info
	switch (CurrentState)
	{
		case EStateMachineState::Init:
			CurrentPhaseSteps = InitSteps;
			NextPhaseSteps    = LoopSteps;
			NextState		  = EStateMachineState::Loop;
			break;
		
		case EStateMachineState::Loop:
			CurrentPhaseSteps = LoopSteps;
			NextPhaseSteps    = EndSteps;
			NextState		  = EStateMachineState::Finished;
			break;
		
		case EStateMachineState::Finished:
			CurrentPhaseSteps = EndSteps;
			break;
		
		default:;
	}

	// We check if we have enough steps in our Current Phase Steps, 
	// if not, then we pass over to the next phase.

	bool bHasPassedPhase = false;
	if (CurrentPhaseSteps.IsValidIndex(StepId))
	{
		switch (CurrentState)
		{
		case EStateMachineState::Loop:

			if(!CanLoopStateEnd())
			{
				ContinueExecutingSteps(CurrentPhaseSteps, StepId);			
			}

			break;
		default:

			ContinueExecutingSteps(CurrentPhaseSteps, StepId);
			break;
		}


	}else
	{
		// The SM has finished.
		if (CurrentState == EStateMachineState::Finished)
		{
			FinishStateMachine();
			return true;
		}

		// Change to the Next Phase.		
		CurrentState = NextState;
		InitAndExecuteSteps(NextPhaseSteps);
		bHasPassedPhase = true;
		
	}
	
	return bHasPassedPhase;
}

void UTBStateMachine::OnInitStepFinishCallback(uint8 Id)
{
	TryContinueToNextState(Id);
}


void UTBStateMachine::OnLoopStepFinishCallback(uint8 Id)
{
	TryContinueToNextState(Id);
}

void UTBStateMachine::OnFinishStepFinishCallback(uint8 Id)
{
	TryContinueToNextState(Id);
}

UTBStateMachine* UTBStateMachine::CreateStateMachine(TSubclassOf<UTBStateMachine> StateMachineClass, UObject* WorldContext, FName Name)
{
	return CreateTBStateMachine<UTBStateMachine>(StateMachineClass, WorldContext, Name);
}

void UTBStateMachine::K2_Init()
{
	Init();
}

bool UTBStateMachine::CanLoopStateEnd()
{
	bool bEndSuccess = true;

	if (EndConditions.Num() == 0)
	{
		return true;
	}
	// We init the Conditions Objects here maybe the State machine does not reach here
	// so there is no point in Init this before, wasted time.
	for(UTBEndCondition*& EndCondition : EndConditions)
	{
		EndCondition->Init(*this);
	}

	switch ( EndConditionMode )
	{
	case EStateMachineEndConditionMode::None:

		for (UTBEndCondition*& Condition : EndConditions)
		{	
			if ( Condition->GetConditionResult() )
			{	
				bEndSuccess = false;
				break;
			}
		}
		break;

	case EStateMachineEndConditionMode::All:

		for (UTBEndCondition*& Condition : EndConditions)
		{
			if ( !Condition->GetConditionResult() )
			{
				bEndSuccess = false;
				break;
			}
		}
		break;

	case EStateMachineEndConditionMode::OnlyOne:

		for (UTBEndCondition*& Condition : EndConditions)
		{

			if ( Condition->GetConditionResult() )
			{
				break;
			}
		}
		bEndSuccess = false;
		break;

	default:

	// Only One Case
	for(UTBEndCondition*& Condition : EndConditions)
	{
		if ( Condition->GetConditionResult() )
		{
			break;
		}
		bEndSuccess = false;
	}

		break;
	}

	return bEndSuccess;
}


void UTBStateMachine::K2_End() 
{
	 End();
}

void UTBStateMachine::End()
{
	CurrentState = EStateMachineState::Finished;
	BindStepsCallback(EndSteps, FName("OnFinishStepFinishCallback"));
	End_Internal();
}

void UTBStateMachine::End_Internal()
{
	InitAndExecuteSteps( EndSteps );
}
void UTBStateMachine::FinishStateMachine()
{
	
}

bool UTBStateMachine::IsStateMachineAvailable()
{
	return CurrentState != EStateMachineState::Finished && CurrentState != EStateMachineState::None ;
}
