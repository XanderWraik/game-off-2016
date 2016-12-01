// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleSim.h"
#include "BTService_AIStateChase.h"

void UBTService_AIStateChase::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	SetReferences(OwnerComp);

	if (ThisBotBlackboard->GetValueAsEnum("AIState") != (uint8)EAIStates::EAI_Run)
	{
		return;
	}

	//Check and, if necessary, perform a state change
	SetAttackState();

	if (ThisBotBlackboard->GetValueAsObject("Enemy") == nullptr)
	{
		ThisBotBlackboard->SetValueAsBool("EnemyDetected", false);
		ThisBotBlackboard->SetValueAsBool("Fighting", false);
		ThisBotBlackboard->SetValueAsEnum("AIState", (uint8)EAIStates::EAI_Patrol);
	}
}

void UBTService_AIStateChase::SetAttackState()
{
	if (ThisBotBlackboard->GetValueAsBool("EnemyDetected") == true  &&
		IsEnemyCloseToAttack() == true &&
		ThisBotBlackboard->GetValueAsBool("Fighting") == true)
	{
		ThisBotBlackboard->SetValueAsEnum("AIState", (uint8)EAIStates::EAI_Attack);
	}
}

//Helper Functions
void UBTService_AIStateChase::SetReferences(UBehaviorTreeComponent& OwnerComp)
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

bool UBTService_AIStateChase::IsEnemyCloseToAttack()
{
	if (EnemyBotCharacter == nullptr)
	{
		return false;
	}

	float Distance = ThisBotCharacter->GetDistanceTo(EnemyBotCharacter);
	UE_LOG(LogTemp, Warning, TEXT("Chase Distance Greater: %f"), Distance);

	if (Distance < 90.0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Chase Distance Lesser: %f"), Distance);
		ThisBotBlackboard->SetValueAsBool("Fighting", true);

		return true;
	}

	ThisBotBlackboard->SetValueAsBool("Fighting", false);
	return false;
}
