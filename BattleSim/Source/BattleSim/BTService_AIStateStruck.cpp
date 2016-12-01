// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleSim.h"
#include "BTService_AIStateStruck.h"

void UBTService_AIStateStruck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	SetReferences(OwnerComp);

	if (ThisBotBlackboard->GetValueAsEnum("AIState") != (uint8)EAIStates::EAI_Struck)
	{
		return;
	}

	SetAttackState();

	if (ThisBotBlackboard->GetValueAsObject("Enemy") == nullptr)
	{
		ThisBotBlackboard->SetValueAsBool("EnemyDetected", false);
		ThisBotBlackboard->SetValueAsBool("Fighting", false);
		ThisBotBlackboard->SetValueAsEnum("AIState", (uint8)EAIStates::EAI_Patrol);
	}
}

void UBTService_AIStateStruck::SetAttackState()
{
	if (ThisBotBlackboard->GetValueAsBool("HitByWeapon") == false &&
		ThisBotBlackboard->GetValueAsObject("Enemy") != nullptr)
	{
		ThisBotBlackboard->SetValueAsEnum("AIState", (uint8) EAIStates::EAI_Attack);
	}
}

void UBTService_AIStateStruck::SetReferences(UBehaviorTreeComponent& OwnerComp)
{
	ThisBotController = Cast<AAIBotController>(OwnerComp.GetAIOwner());
	ThisBotCharacter = Cast<AAICharacter>(ThisBotController->GetPawn());

	//Get its blackboard
	ThisBotBlackboard = ThisBotController->GetBlackboardComponent();

	if (ThisBotBlackboard->GetValueAsObject("Enemy") != nullptr 
		&& ThisBotBlackboard->GetValueAsObject("Enemy")->IsA(AAICharacter::StaticClass()))
	{
		EnemyBotCharacter = Cast<AAICharacter>(ThisBotBlackboard->GetValueAsObject("Enemy"));
		EnemyBotController = Cast<AAIBotController>(EnemyBotCharacter->GetController());
		EnemyBlackboard = EnemyBotController->GetBlackboardComponent();
	}
}