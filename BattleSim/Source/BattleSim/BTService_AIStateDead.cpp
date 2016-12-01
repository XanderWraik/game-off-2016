// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleSim.h"
#include "BTService_AIStateDead.h"


void UBTService_AIStateDead::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	SetReferences(OwnerComp);

	if (ThisBotBlackboard->GetValueAsEnum("AIState") != (uint8)EAIStates::EAI_Dead)
	{
		return;
	}

	//seems to be an issue with clearing balck board values.
	TArray<AActor*> DummyActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), DummyActor);

	//Reset Enemy Bot to Patrol
	if (EnemyBlackboard != nullptr)
	{
		EnemyBlackboard->ClearValue("Enemy");
		EnemyBlackboard->SetValueAsBool("EnemyDetected", false);
		EnemyBlackboard->SetValueAsBool("Fighting", false);

		EnemyBlackboard->SetValueAsEnum("AIState", (uint8)EAIStates::EAI_Patrol);
	}

	
	ThisBotBlackboard->ClearValue("Enemy");
	ThisBotBlackboard->SetValueAsBool("EnemyDetected", false);
	ThisBotBlackboard->SetValueAsBool("Fighting", false);
	ThisBotCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ThisBotController->GetBrainComponent()->StopLogic("Bot Dead");
	return;
}

void UBTService_AIStateDead::SetReferences(UBehaviorTreeComponent& OwnerComp)
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
