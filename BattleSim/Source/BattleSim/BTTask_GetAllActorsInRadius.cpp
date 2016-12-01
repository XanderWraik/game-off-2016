// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleSim.h"
#include "BTTask_GetAllActorsInRadius.h"


UBTTask_GetAllActorsInRadius::UBTTask_GetAllActorsInRadius()
{
	//Set EQS Query. Hardcoded since can't create a BP of BTTaskNode
	static ConstructorHelpers::FObjectFinder<UEnvQuery> EQS_Query(TEXT("EnvQuery'/Game/Blueprints/Bots/AI/EQS/EnemiesInArea_EQS.EnemiesInArea_EQS'"));

	EnemySeekerQuery = EQS_Query.Object;
}

//Run each tick
EBTNodeResult::Type UBTTask_GetAllActorsInRadius::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ThisBot = Cast<AAIBotController>(OwnerComp.GetAIOwner());
	ThisBotCharacter = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());

	//Used later for comparing once the EQS returns
	ThisBotCharacterTeamName = ThisBotCharacter->GetTeamName();

	//Sanity Check
	if (EnemySeekerQuery != nullptr && ThisBot != nullptr)
	{
		EnemySeekerQueryRequest = FEnvQueryRequest(EnemySeekerQuery, ThisBotCharacter);
	}

	EnemySeekerQueryRequest.Execute(EEnvQueryRunMode::AllMatching,
									this,
									&UBTTask_GetAllActorsInRadius::EnemySeekerQueryFinished);
	
	return EBTNodeResult::Succeeded;
}

void UBTTask_GetAllActorsInRadius::EnemySeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	TArray<AActor*> AllDetectedActors;
	Result->GetAllAsActors(AllDetectedActors);

	UE_LOG(LogTemp, Warning, TEXT("EQS %i"), AllDetectedActors.Num());

	//Simple for each loop. 
	//Check if detected character is friend or foe.
	//If foe, set enemy in BB and fighting to true, so this AICharacter can't fight anywhere else till the engagement is over
	for (AActor* DetectedActor : AllDetectedActors)
	{
		//Check if actor is not null: Sanity Check
		if (DetectedActor != nullptr)
		{
			AAICharacter* Detected_AI_Character = Cast<AAICharacter>(DetectedActor);
			AAIBotController* Detected_AI_BotController = Cast<AAIBotController>(Detected_AI_Character->GetController());
			UBlackboardComponent* Detected_AI_BotBlackboard = Detected_AI_BotController->GetBlackboardComponent();

			if (Detected_AI_Character == nullptr || Detected_AI_BotController == nullptr || Detected_AI_BotBlackboard == nullptr)
			{
				return;
			}

			//A bunch of if else checks coz an EQS returns everything it sees
			if (Detected_AI_Character->GetTeamName() == ThisBotCharacterTeamName)
			{
				UE_LOG(LogTemp, Warning, TEXT("I'm detecting my team"));
				continue;
			}
			else if (Detected_AI_Character->GetName() == ThisBotCharacter->GetName())
			{
				UE_LOG(LogTemp, Warning, TEXT("I'm detecting myself"));
				continue;
			}

			//So, I'm done detecting my teammates, myself and, therefore, all that must be left are the enemies.
			//Run behind any one of them!
			else if(Detected_AI_Character->GetTeamName() != ThisBotCharacterTeamName && 
					Detected_AI_BotBlackboard->GetValueAsEnum("AIState") != (uint8) EAIStates::EAI_Dead &&
					Detected_AI_BotBlackboard->GetValueAsBool("Tagged") == false)
			{
				//Set flags for attack
				ThisBot->GetBlackboardComponent()->SetValueAsBool("EnemyDetected", true);

				UE_LOG(LogTemp, Warning, TEXT("EnemyDetected"));
				//Set enemy to attack in blackboard
				ThisBot->GetBlackboardComponent()->SetValueAsObject("Enemy", DetectedActor);
			}
		}
	}
}

