// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "BattleSimGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BATTLESIM_API ABattleSimGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	FTimerHandle BotSpawnTimerHandle;

	int TimerTicker;

	void SpawnAICharacterDeferred(FVector SpawnLocation, FRotator SpawnRotation,  FName TeamName);

public:
	ABattleSimGameModeBase();
	
	UPROPERTY(EditAnywhere, Category=SpawnParams)
	float BotSpawnDelay = 5.0f;

	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = SpawnParams)
	TArray<TSubclassOf<class AAICharacter>> AICharactersToSpawn;

	//Max number of times the timer function fire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=SpawnParams)
	int MaxTimerRunCycles;

	UFUNCTION()//BlueprintImplementableEvent, Category=Spawn)
	void SpawnBot();
};
