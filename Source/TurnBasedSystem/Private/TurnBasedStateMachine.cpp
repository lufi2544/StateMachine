// Fill out your copyright notice in the Description page of Project Settings.


// TODO Handle the State Machine End.( Erasing delegates and killing objects? )

#include "TurnBasedStateMachine.h"
#include "TBStep.h"
#include "TBEndCondition.h"

#include <Kismet/KismetSystemLibrary.h>

UTBStateMachine::UTBStateMachine()
{
	StateInfo = FTBStateMachineStateInfo();
	EndConditionMode = EStateMachineEndConditionMode::Default;
	StateMachineName = FName("");
	Owner = nullptr;
	bEndConditionsInit = false;
	StepsIdCounter = 0;
	LastStatesSteps = 0;
}

UTBStateMachine* UTBStateMachine::CreateStateMachine(TSubclassOf<UTBStateMachine> StateMachineClass, UObject* WorldContext, FName Name)
{
	return CreateTBStateMachine<UTBStateMachine>(StateMachineClass, WorldContext, Name);
}

bool UTBStateMachine::IsStateMachineAvailable()
{
	return StateInfo.CurrentState != EStateMachineState::Finished && StateInfo.CurrentState != EStateMachineState::None ;
}

void UTBStateMachine::K2_Init()
{
	Init();
}

void UTBStateMachine::K2_End()
{
	// Cancel ? or end ? if we call this, that would mean that the State Machine has not finished on its own.
}

void UTBStateMachine::Init()
{

	StateInfo.CurrentState = EStateMachineState::Init;
	HandleCurrentStepsInitialisation();

	// Could Add Stuff Here
	Init_Internal();
}

void UTBStateMachine::Init_Internal()
{
}

void UTBStateMachine::InitAndExecuteSteps()
{
	// Just running the fist step, the execution of the rest will be handled by the FinishCallback
	if (StateInfo.CurrentPhaseSteps.Num() > 0)
	{
		BindStepsCallback(StateInfo.CurrentPhaseSteps, [&]()->FName
		{
				switch (StateInfo.CurrentState)
				{
				case EStateMachineState::Init:
				return FName("OnInitStepFinishCallback");
				break;

				case EStateMachineState::Loop:
				return FName("OnLoopStepFinishCallback");
				break;

				case EStateMachineState::Finished:
				return FName("OnFinishStepFinishCallback");
				break;

				default:
					break;
				}
			return FName("");
		}()
		);
		StateInfo.CurrentPhaseSteps[0]->InitAndExecute(*this, ++StepsIdCounter);
	}else
	{
		ChangeToNextState();
	}
}

void UTBStateMachine::UpdateStateInfoValues()
{
	switch (StateInfo.CurrentState)
	{
	case EStateMachineState::Init:
		StateInfo.CurrentPhaseSteps = InitSteps;
		StateInfo.NextState = EStateMachineState::Loop;
		StateInfo.NextPhaseSteps = LoopSteps;
		break;

	case EStateMachineState::Loop:
		StateInfo.CurrentPhaseSteps = LoopSteps;
		StateInfo.NextPhaseSteps = EndSteps;
		StateInfo.NextState = EStateMachineState::Finished;
		break;

	case EStateMachineState::Finished:
		StateInfo.CurrentPhaseSteps = EndSteps;
		break;

	default:;
	}
}

void UTBStateMachine::OnInitStepFinishCallback(uint8 Id)
{
	InitStepFinished_Internal(Id);
	TryContinueToNextState(Id);
}

void UTBStateMachine::InitStepFinished_Internal(uint8 Id)
{

}

void UTBStateMachine::OnLoopStepFinishCallback(uint8 Id)
{
	LoopStepFinished_Internal(Id);
	TryContinueToNextState(Id);
}

void UTBStateMachine::LoopStepFinished_Internal(uint8 Id)
{

}

void UTBStateMachine::OnFinishStepFinishCallback(uint8 Id)
{
	FinishStepFinished_Internal(Id);
	TryContinueToNextState(Id);
}

void UTBStateMachine::FinishStepFinished_Internal(uint8 Id)
{

}


void UTBStateMachine::BindStepsCallback(TArray<UTBStep*>& StepsArray, FName FunctionToBind)
{
	for (UTBStep*& Step : StepsArray)
	{
		Step->OnStepFinished.AddUFunction(this, FName(FunctionToBind));
	}
}

void UTBStateMachine::HandleCurrentStepsInitialisation()
{
	UpdateStateInfoValues();
	InitAndExecuteSteps();
}

bool UTBStateMachine::TryContinueToNextState(uint8 StepId)
{
	TArray<UTBStep*> CurrentPhaseSteps;
	TArray<UTBStep*> NextPhaseSteps;
	EStateMachineState::Type NextState = EStateMachineState::None;

	int32 StepIdToVerify = StepId - StateInfo.PreviouseStatesStepsNum;
	return TryContinueExecutingSteps(StepIdToVerify);
}

// TODO Discuss: maybe a bool return and checking here the Array bounds validation??
bool UTBStateMachine::TryContinueExecutingSteps(uint8 StepIdxToExecute)
{
	/* We check if we have enough steps in our Current Phase Steps,
	 if not, then we pass over to the next phase.


	 In the case we are on the Loop State && we run out of steps && the Loop State can not end still
	 we run the Current State( Loop ) from the beginning. */
	if( (StateInfo.CurrentState == EStateMachineState::Loop))
	{
		if(!CanLoopStateEnd())
		{
			if(StepIdxToExecute == StateInfo.CurrentPhaseSteps.Num())
			{
				StepIdxToExecute = 0;			
			}

		}else
		{
			ChangeToNextState();
			return true;
		}
		
	}

	if (StateInfo.CurrentPhaseSteps.IsValidIndex(StepIdxToExecute))
	{

		StateInfo.CurrentPhaseSteps[StepIdxToExecute]->InitAndExecute(*this, ++StepsIdCounter);
	
	}else
	{

		ChangeToNextState();
		return true;
	}

	return false;
}

void UTBStateMachine::ChangeToNextState()
{
	if (StateInfo.CurrentState == EStateMachineState::Finished)
	{
		FinishStateMachine();
		return;
	}

	StateInfo.SubmitState();
	HandleCurrentStepsInitialisation();
}

bool UTBStateMachine::CanLoopStateEnd()
{

	if (EndConditions.Num() == 0)
	{
		return true;
	}

	if(!bEndConditionsInit)
	{
		// We init the Conditions Objects here maybe the State machine does not reach here
		// so there is no point in Init this before, wasted time.
		for(UTBEndCondition*& EndCondition : EndConditions)
		{
			EndCondition->InitSMObject(*this);
		}
	
		bEndConditionsInit = true;
	}

	switch ( EndConditionMode )
	{
	case EStateMachineEndConditionMode::None:

		for (UTBEndCondition*& Condition : EndConditions)
		{	
			if ( Condition->GetConditionResult() )
			{	
				return false;
			}
		}
		break;

	case EStateMachineEndConditionMode::All:

		for (UTBEndCondition*& Condition : EndConditions)
		{
			if ( !Condition->GetConditionResult() )
			{
				return false;
			}
		}
		break;

	case EStateMachineEndConditionMode::OnlyOne:

		for (UTBEndCondition*& Condition : EndConditions)
		{

			if ( Condition->GetConditionResult() )
			{
				return true;
			}
		}
		break;

	default:

	// Only One Case
	for(UTBEndCondition*& Condition : EndConditions)
	{
		if ( Condition->GetConditionResult() )
		{
			return true;
		}
	}

		break;
	}

	return false;
}

void UTBStateMachine::FinishStateMachine()
{
	UKismetSystemLibrary::PrintString(this, "Finished State Machine");
}