// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTService.h"
#include "AICharacter.h"
#include "AIStates.h"
#include "AIBotController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTService_AIStateIdle.generated.h"

/**
 * AI_State Idle
 * Actions to perform in Idle state
 */
UCLASS()
class BATTLESIM_API UBTService_AIStateIdle : public UBTService
{
	GENERATED_BODY()
	
private:
	AAICharacter* ThisBotCharacter;
	AAICharacter* EnemyBotCharacter;
	AAIBotController* ThisBotController;
	AAIBotController* EnemyBotController;
	UBlackboardComponent* ThisBotBlackboard; 
	UBlackboardComponent* EnemyBlackboard;

	void SetPatrolState();
	void SetChaseState();
	void SetAttackState();

	//Helper Functions
	bool IsEnemyCloseToAttack();
	void SetReferences(UBehaviorTreeComponent& OwnerComp);

public:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
