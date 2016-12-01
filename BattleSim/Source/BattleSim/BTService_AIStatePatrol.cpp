// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleSim.h"
#include "BTService_AIStatePatrol.h"

void UBTService_AIStatePatrol::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	SetReferences(OwnerComp);

	if (ThisBotBlackboard->GetValueAsEnum("AIState") != (uint8)EAIStates::EAI_Patrol)
	{
		return;
	}

	//Check and, if necessary, perform a state change
	SetChaseState();
	SetAttackState();
}

void UBTService_AIStatePatrol::SetAttackState()
{
	if (ThisBotBlackboard->GetValueAsBool("EnemyDetected") == true &&
		IsEnemyCloseToAttack() == true && 
		ThisBotBlackboard->GetValueAsBool("Fighting") == true)
	{
		ThisBotBlackboard->SetValueAsEnum("AIState", (uint8)EAIStates::EAI_Attack);
	}
}

void UBTService_AIStatePatrol::SetChaseState()
{

	if (ThisBotBlackboard->GetValueAsBool("EnemyDetected") == true &&
		IsEnemyCloseToAttack() == false)
	{
		ThisBotBlackboard->SetValueAsEnum("AIState", (uint8)EAIStates::EAI_Run);
	}
}

//Helper Functions
void UBTService_AIStatePatrol::SetReferences(UBehaviorTreeComponent& OwnerComp)
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

bool UBTService_AIStatePatrol::IsEnemyCloseToAttack()
{
	if (EnemyBotCharacter == nullptr)
	{
		return false;
	}

	float Distance = ThisBotCharacter->GetDistanceTo(EnemyBotCharacter);
	UE_LOG(LogTemp, Warning, TEXT("Patrol Distance Greater: %f"), Distance);

	if (Distance < 90.0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Patrol Distance Lesser: %f"), Distance);
		ThisBotBlackboard->SetValueAsBool("Fighting", true);

		return true;
	}

	ThisBotBlackboard->SetValueAsBool("Fighting", false);
	return false;
}

