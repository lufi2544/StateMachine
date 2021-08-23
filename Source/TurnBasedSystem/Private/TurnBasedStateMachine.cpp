// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedStateMachine.h"
#include "TBStep.h"
#include "TBEndCondition.h"

UTBStateMachine* UTBStateMachine::SelfReference = nullptr;

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
		StepsToExecute[0]->InitAndExecute(*this);
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

void UTBStateMachine::ContinueExecutingSteps(TArray<UTBStep*>& StepsToExecute, uint8 CurrentStepId)
{
	StepsToExecute[ CurrentStepId + 1 ]->InitAndExecute(*this);
}

bool UTBStateMachine::TryContinueToNextState(uint8 StepId)
{
	bool bHasPassedPhase = false;
	TArray<UTBStep*> CurrentPhaseSteps;
	TArray<UTBStep*> NextPhaseSteps;
	EStateMachineState::Type NextState = EStateMachineState::None;

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

	if (CurrentPhaseSteps.IsValidIndex(StepId + 1))
	{
		ContinueExecutingSteps(CurrentPhaseSteps, StepId);
		bHasPassedPhase = false;
	}else
	{	

		switch (NextState)
		{
		case EStateMachineState::Loop:

			Loop();

			break;
		case EStateMachineState::Finished:

			FinishStateMachine();
			bHasPassedPhase = true;

			break;
		default:
			break;
		}

		// We check if this is the last index of the Finished State
		if (CurrentState == EStateMachineState::Finished && CurrentState > 0)
		{

		}else
		{
			// We continue to the Next State Machine State
			CurrentState = NextState;
			InitAndExecuteSteps(NextPhaseSteps);
			bHasPassedPhase = true;	
		}
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

UTBStateMachine* UTBStateMachine::CreateStateMachine(TSubclassOf<UTBStateMachine> StateMachineClass, FName Name)
{
	return CreateTBStateMachine<UTBStateMachine>(StateMachineClass, Name);
}

void UTBStateMachine::K2_Init()
{
	Init();
}

bool UTBStateMachine::ExecuteEndLoopState()
{
	bool bEndSuccess = false;

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

		for (UTBEndCondition*& EndCondition : EndConditions)
		{	
			if ( EndCondition->GetConditionResult() )
			{	
				bEndSuccess = false;
				break;
			}
		}
		break;

	case EStateMachineEndConditionMode::All:

		for (UTBEndCondition*& EndCondition : EndConditions)
		{
			if ( !EndCondition->GetConditionResult() )
			{
				bEndSuccess = false;
				break;
			}
		}
		break;

	case EStateMachineEndConditionMode::OnlyOne:

		for (UTBEndCondition*& EndCondition : EndConditions)
		{

			if ( EndCondition->GetConditionResult() )
			{
				bEndSuccess = true;
				break;
			}
		}
		break;

	default:

	for(UTBEndCondition*& EndCondition : EndConditions)
	{
		if ( EndCondition->GetConditionResult() )
		{
			bEndSuccess = true;
			break;
		}
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
