// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "AICharacter.h"
#include "AIBotController.h"
#include "AIStates.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetAllActorsInRadius.generated.h"

/**
 * 
 */
UCLASS()
class BATTLESIM_API UBTTask_GetAllActorsInRadius : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	AAIBotController* ThisBot;
	AAICharacter* ThisBotCharacter;
	FName ThisBotCharacterTeamName;

public:
	UBTTask_GetAllActorsInRadius();

	UPROPERTY(BlueprintReadWrite, Category = EQS)
	UEnvQuery* EnemySeekerQuery; // set the query in editor

	FEnvQueryRequest EnemySeekerQueryRequest;

	// The function that gets called when querry finished
	void EnemySeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
