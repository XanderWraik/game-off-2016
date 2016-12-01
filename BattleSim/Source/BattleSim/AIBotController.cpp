// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleSim.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AICharacter.h"
#include "Engine.h"
#include "AIBotController.h"

AAIBotController::AAIBotController()
{
	PrimaryActorTick.bCanEverTick = true;

	//Sight config
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI_Perception_Component")));

	if ((GetPerceptionComponent() != nullptr) && (GEngine != nullptr))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "AI Perception be set!");
		//UE_LOG(LogTemp, Warning, TEXT("AI Perception Set"));
	}

	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	GetPerceptionComponent()->SetDominantSense(SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AAIBotController::OnPawnCaught);

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = 60.0f;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	GetPerceptionComponent()->ConfigureSense(*SightConfig);

	BrainComponent = BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

//On Player Detected
void AAIBotController::OnPawnCaught(TArray<AActor*> DetectedBots)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "I see you!");
	//UE_LOG(LogTemp, Warning, TEXT("I see you!"));

	
	for (AActor* DetectedActor : DetectedBots)
	{
		AAICharacter* Detected_AI_Char = Cast<AAICharacter>(DetectedActor);
		if (Detected_AI_Char == nullptr || Detected_AI_Char->IsA(AAICharacter::StaticClass()) == false)
		{
			continue;
		}
		if (Detected_AI_Char->GetTeamName() != Cast<AAICharacter>(this->GetControlledPawn())->GetTeamName() &&
			Detected_AI_Char->GetHealth() >= 0)
		{
			BlackboardComponent->SetValueAsBool(FName("Detected"), true);
			BlackboardComponent->SetValueAsObject(FName("Enemy"), DetectedActor);
		}
	}
}

void AAIBotController::Possess(APawn * Pawn)
{
	Super::Possess(Pawn);

	AAICharacter* AIPatrol = Cast<AAICharacter>(Pawn);

	//UE_LOG(LogTemp, Warning, TEXT("AI Possessed"));
	if (AIPatrol->BehaviorTree != nullptr && BehaviorTreeComponent != nullptr)
	{
		BlackboardComponent->InitializeBlackboard(*(AIPatrol->BehaviorTree->BlackboardAsset));

		//UE_LOG(LogTemp, Warning, TEXT("Blackboard Set"));
		BehaviorTreeComponent->StartTree(*(AIPatrol->BehaviorTree));
		//UE_LOG(LogTemp, Warning, TEXT("BT Started"));

		//Cast<UCharacterMovementComponent>(GetControlledPawn()->GetMovementComponent())->SetAvoidanceEnabled(true);
	}
}

void AAIBotController::Tick(float DeltaSeconds)
{
	//UE_LOG(LogTemp, Warning, TEXT("Ticking"));
	//MoveToActor(UGameplayStatics::GetPlayerController(GetWorld(), 0), 5.0f);
}

FRotator AAIBotController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator();
	}

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

UBlackboardComponent* AAIBotController::GetBlackboardComponent() const
{
	return BlackboardComponent;
}