// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_DistanceToEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BATTLESIM_API UBTDecorator_DistanceToEnemy : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
	
	
};
