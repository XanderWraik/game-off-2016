// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleSim.h"
#include "BTService_AIStateIdle.h"

void UBTService_AIStateIdle::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	SetReferences(OwnerComp);

	if (ThisBotBlackboard->GetValueAsEnum("AIState") != (uint8)EAIStates::EAI_Idle)
	{
		return;
	}

	//Check and, if necessary, perform a state change
	SetPatrolState();
	SetChaseState();
	SetAttackState();
}

void UBTService_AIStateIdle::SetPatrolState()
{
	if (ThisBotBlackboard->GetValueAsBool("SpawnTimeComplete") == true && 
		ThisBotBlackboard->GetValueAsBool("Detected") == false && 
		ThisBotBlackboard->GetValueAsObject("Enemy") == nullptr)
	{
		ThisBotBlackboard->SetValueAsEnum("AIState", (uint8) EAIStates::EAI_Patrol);
	}
}

void UBTService_AIStateIdle::SetChaseState()
{
	if (ThisBotBlackboard->GetValueAsBool("EnemyDetected") == true && 
		IsEnemyCloseToAttack() &&
		ThisBotBlackboard->GetValueAsBool("Fighting") == true)
	{
		ThisBotBlackboard->SetValueAsEnum("AIState", (uint8)EAIStates::EAI_Run);
	}
}

void UBTService_AIStateIdle::SetAttackState()
{
	if (ThisBotBlackboard->GetValueAsBool("EnemyDetected") == true &&
		IsEnemyCloseToAttack() == true &&
		ThisBotBlackboard->GetValueAsBool("Fighting") == true)
	{
		ThisBotBlackboard->SetValueAsEnum("AIState", (uint8)EAIStates::EAI_Attack);
	}
}

//Helper Functions
void UBTService_AIStateIdle::SetReferences(UBehaviorTreeComponent& OwnerComp)
{
	ThisBotController = Cast<AAIBotController>(OwnerComp.GetAIOwner());
	ThisBotCharacter = Cast<AAICharacter>(ThisBotController->GetPawn());

	//Get its blackboard
	ThisBotBlackboard = ThisBotController->GetBlackboardComponent();

	if (ThisBotBlackboard->GetValueAsObject("Enemy") != nullptr && 
		ThisBotBlackboard->GetValueAsObject("Enemy")->IsA(AAICharacter::StaticClass()))
	{
		EnemyBotCharacter = Cast<AAICharacter>(ThisBotBlackboard->GetValueAsObject("Enemy"));
		EnemyBotController = Cast<AAIBotController>(EnemyBotCharacter->GetController());
		EnemyBlackboard = EnemyBotController->GetBlackboardComponent();
	}
}

bool UBTService_AIStateIdle::IsEnemyCloseToAttack()
{
	if (EnemyBotCharacter == nullptr)
	{
		return false;
	}

	float Distance = ThisBotCharacter->GetDistanceTo(EnemyBotCharacter);
	UE_LOG(LogTemp, Warning, TEXT("Idle Distance Greater: %f"), Distance);

	if (Distance < 90.0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Idle Distance Lesser: %f"), Distance);
		ThisBotBlackboard->SetValueAsBool("Fighting", true);

		return true;
	}

	ThisBotBlackboard->SetValueAsBool("Fighting", false);
	return false;
}
