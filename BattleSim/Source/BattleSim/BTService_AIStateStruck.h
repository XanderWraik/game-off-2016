// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTService.h"
#include "AICharacter.h"
#include "AIStates.h"
#include "AIBotController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTService_AIStateStruck.generated.h"

/**
 * 
 */
UCLASS()
class BATTLESIM_API UBTService_AIStateStruck : public UBTService
{
	GENERATED_BODY()
	
private:
	AAICharacter* ThisBotCharacter;
	AAICharacter* EnemyBotCharacter;
	AAIBotController* ThisBotController;
	AAIBotController* EnemyBotController;
	UBlackboardComponent* ThisBotBlackboard;
	UBlackboardComponent* EnemyBlackboard;

	//Helper function
	void SetReferences(UBehaviorTreeComponent& OwnerComp);

	void SetAttackState();
	
public:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	
};
