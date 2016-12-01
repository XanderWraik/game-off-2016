// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "AIStates.h"
#include "AIBotController.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDetected, TArray<AActor*>, DetectedBots);

/**
 * 
 */
UCLASS()
class BATTLESIM_API AAIBotController : public AAIController
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, Category = AI)
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, Category = AI)
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, Category = AI)
	class UBlackboardComponent* BlackboardComponent;

	//Store current state of AI just for reference, always refer to state from Blackboard component.
	UPROPERTY(VisibleAnywhere, Category = AI)
	EAIStates AI_State;

public: 
	AAIBotController();

	UFUNCTION()
	void OnPawnCaught(TArray<AActor*> CaughtPawns);

	virtual void Possess(APawn* Pawn) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual FRotator GetControlRotation() const override;

	//Return Blackboard component
	UFUNCTION(BlueprintPure, Category=AI)
	UBlackboardComponent* GetBlackboardComponent() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AICharacter)
	float AISightRadius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI)
	float AILoseSightRadius = 300.0f + 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI)
	float AISightAge = 5.0f;

	/*UPROPERTY(BlueprintAssignable, Category = "EventDispatcher")
	FOnEnemyDetected EnemyDetected;*/
};
