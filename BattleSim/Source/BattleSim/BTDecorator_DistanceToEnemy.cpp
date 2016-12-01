// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleSim.h"
#include "AIBotController.h"
#include "AICharacter.h"
#include "AIStates.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTDecorator_DistanceToEnemy.h"

bool UBTDecorator_DistanceToEnemy::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIBotController* BotController = Cast<AAIBotController>(OwnerComp.GetAIOwner());
	EAIStates CurrentAIState = EAIStates::EAI_Idle;

	if (BotController != nullptr)
	{
		AAICharacter* EnemyCharacter = Cast<AAICharacter>(BotController->GetBlackboardComponent()->GetValueAsObject(FName("Enemy")));
		float Distance = BotController->GetPawn()->GetDistanceTo(EnemyCharacter);

		UE_LOG(LogTemp, Warning, TEXT("Distance outside if: %f"), Distance);
		if (Distance >= 60.0 && Distance <= 100)
		{
			UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), Distance);
			

			return false;
		}
	}

	return true;
}


