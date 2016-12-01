// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleSim.h"
#include "BTService_AIStateAttack.h"

void UBTService_AIStateAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	SetReferences(OwnerComp);

	if (ThisBotBlackboard->GetValueAsEnum("AIState") != (uint8)EAIStates::EAI_Attack)
	{
		return;
	}

	//Perform State Changes, if necessary
	SetPatrolState();
	SetDeadState();
	SetIdleState();
	SetStruckState();
	//SetChaseState();

	if (ThisBotBlackboard->GetValueAsObject("Enemy") == nullptr)
	{
		ThisBotBlackboard->SetValueAsBool("EnemyDetected", false);
		ThisBotBlackboard->SetValueAsBool("Fighting", false);
		ThisBotBlackboard->SetValueAsEnum("AIState", (uint8)EAIStates::EAI_Patrol);
	}
}

void UBTService_AIStateAttack::SetPatrolState()
{
	if (EnemyBotController == nullptr)
	{
		return;
	}
	
	if (EnemyBotController->GetBlackboardComponent()->GetValueAsBool("Dead") == true)
	{
		ThisBotBlackboard->SetValueAsEnum("AIState", (uint8)EAIStates::EAI_Patrol);
	}
	
}

void UBTService_AIStateAttack::SetDeadState()
{
	if (ThisBotBlackboard->GetValueAsBool("Dead") == true)
	{
		ThisBotBlackboard->SetValueAsEnum("AIState", (uint8)EAIStates::EAI_Dead);
	}
}

void UBTService_AIStateAttack::SetStruckState()
{
	if (ThisBotBlackboard->GetValueAsBool("HitByWeapon") == true && 
		ThisBotBlackboard->GetValueAsObject("Enemy") != nullptr)
	{
		ThisBotBlackboard->SetValueAsEnum("AIState", (uint8)EAIStates::EAI_Struck);
	}
}

void UBTService_AIStateAttack::SetChaseState()
{
	if (ThisBotBlackboard->GetValueAsBool("EnemyDetected") == true &&
		IsEnemyCloseToAttack() == false &&
		ThisBotBlackboard->GetValueAsObject("Enemy") != nullptr &&
		ThisBotBlackboard->GetValueAsBool("Fighting") == true)
	{
		ThisBotBlackboard->SetValueAsEnum("AIState", (uint8)EAIStates::EAI_Run);
	}
	
}

void UBTService_AIStateAttack::SetIdleState()
{
	//@TODO
}

//Helper Functions
void UBTService_AIStateAttack::SetReferences(UBehaviorTreeComponent& OwnerComp)
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

bool UBTService_AIStateAttack::IsEnemyCloseToAttack()
{
	if (EnemyBotCharacter == nullptr)
	{
		return false;
	}

	float Distance = ThisBotCharacter->GetDistanceTo(EnemyBotCharacter);
	UE_LOG(LogTemp, Warning, TEXT("Attack Distance Greater: %f"), Distance);

	if (Distance < 90.0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack Distance Lesser: %f"), Distance);

		return true;
	}

	return false;
}
