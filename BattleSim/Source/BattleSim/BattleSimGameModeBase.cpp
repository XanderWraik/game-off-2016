// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleSim.h"
#include "AICharacter.h"
#include "SpawnPoint.h"
#include "BattleSimGameModeBase.h"

ABattleSimGameModeBase::ABattleSimGameModeBase()
{
	if (GetWorld() != nullptr)
	{
		GetWorld()->GetTimerManager().SetTimer(BotSpawnTimerHandle, this, &ABattleSimGameModeBase::SpawnBot, BotSpawnDelay, true, 0);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Timer Set"));
}

void ABattleSimGameModeBase::SpawnAICharacterDeferred(FVector SpawnLocation, FRotator SpawnRotation, FName TeamName)
{
	if (AICharactersToSpawn[0] == nullptr)
	{
		return;
	}
	int index = FMath::RandRange(0, (AICharactersToSpawn.Num() - 1));
	AAICharacter* AISpawnedCharacter = GetWorld()->SpawnActorDeferred<AAICharacter>(AICharactersToSpawn[index], SpawnLocation, SpawnRotation);

	if (AISpawnedCharacter != nullptr)
	{
		AISpawnedCharacter->SetTeamName(TeamName);
		UGameplayStatics::FinishSpawningActor(AISpawnedCharacter, FTransform(SpawnRotation, SpawnLocation));
		//UE_LOG(LogTemp, Warning, TEXT("Exec complete Set %p"), AISpawnedCharacter);
	}
}

void ABattleSimGameModeBase::SpawnBot()
{
	if (TimerTicker >= MaxTimerRunCycles)
	{
		GetWorld()->GetTimerManager().ClearTimer(BotSpawnTimerHandle);
		return;
	}

	if (GetWorld() != nullptr)
	{
		TimerTicker++;
		UE_LOG(LogTemp, Warning, TEXT("%d. Timer Running"), TimerTicker);

		TArray<AActor*> AllSpawnPoints;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), AllSpawnPoints);

		//Foreach variant
		/*for (AActor* SpawnPoint : AllSpawnPoints)
		{
			if (SpawnPoint != nullptr)
			{
				//regular Spawn
				/*AAICharacter* AISpawnedCharacter = GetWorld()->SpawnActor<AAICharacter>(AICharacterToSpawn, SpawnPoint->GetActorLocation(), FRotator());

				UE_LOG(LogTemp, Warning, TEXT("Exec complete Set %p"), AISpawnedCharacter);

				//Deferred Spawn. Needed for setting properties before creation
				AAICharacter* AISpawnedCharacter = GetWorld()->SpawnActorDeferred<AAICharacter>(AICharacterToSpawn, SpawnPoint->GetActorLocation(), FRotator(0.0, 0.0, 0.0));

				if (AISpawnedCharacter != nullptr)
				{
					AISpawnedCharacter->SetTeamName("Team2");
					UGameplayStatics::FinishSpawningActor(AISpawnedCharacter, FTransform(FRotator(), SpawnPoint->GetActorLocation()));
					UE_LOG(LogTemp, Warning, TEXT("Exec complete Set %p"), AISpawnedCharacter);
				}
			}
		}*/
		
		//Use a simple odd even scheme to populate the world and spawn actors
		for (int i = 0; i < AllSpawnPoints.Num(); i++)
		{
			if (i % 2 == 0)
			{
				SpawnAICharacterDeferred(AllSpawnPoints[i]->GetActorLocation(), AllSpawnPoints[i]->GetActorRotation(), "Team1");

			}
			else
			{
				SpawnAICharacterDeferred(AllSpawnPoints[i]->GetActorLocation(), AllSpawnPoints[i]->GetActorRotation(), "Team2");
			}
		}
	}
}
