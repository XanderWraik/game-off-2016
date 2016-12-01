// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleSim.h"
#include "AICharacter.h"


// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCapsuleComponent()->bGenerateOverlapEvents = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnWeaponOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AAICharacter::OnWeaponOverlapEnd);
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Spawn Weapon
	if (CharacterWeapon != nullptr)
	{
		FActorSpawnParameters SpawnParams;

		SpawnerWeapon = GetWorld()->SpawnActor<AWeapon>(CharacterWeapon, SpawnParams);

		FAttachmentTransformRules AttachementRules(EAttachmentRule::SnapToTarget, true);
		SpawnerWeapon->AttachToComponent(GetMesh(), AttachementRules, WeaponSocket);
	}

	ThisBotController = Cast<AAIBotController>(GetController());
}

// Called every frame
void AAICharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//Point Bot in direction of enemy
	AAIBotController* BotController = Cast<AAIBotController>(GetController());
	if (BotController != nullptr && 
		BotController->GetBlackboardComponent()->GetValueAsObject(FName("Enemy")) != nullptr &&
		BotController->GetBlackboardComponent()->GetValueAsBool("EnemyDetected") == true)
	{
		FVector MyLoc = GetActorLocation();
		FVector TargetLoc = Cast<AAICharacter>(BotController->GetBlackboardComponent()->GetValueAsObject(FName("Enemy")))->GetActorLocation();
		FVector Dir = (TargetLoc - MyLoc);
		Dir.Normalize();
		SetActorRotation(FMath::Lerp(GetActorRotation(), Dir.Rotation(), 0.05f));
	}
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

int32 AAICharacter::GetHealth()
{
	return Health;
}

FName AAICharacter::GetTeamName()
{
	return this->TeamName;
}

void AAICharacter::SetTeamName(FName TeamName)
{
	this->TeamName = TeamName;
}

void AAICharacter::ReduceAIBotHealth(int32 WeaponDamage)
{
	UE_LOG(LogTemp, Warning, TEXT("Health: %d"), Health);
	if (Health > 0)
	{
		Health = Health - WeaponDamage;
	}
	else
	{
		BotDead();
	}
}

void AAICharacter::BotDead()
{
	if (ThisBotController != nullptr)
	{
		ThisBotController->GetBlackboardComponent()->SetValueAsBool("Dead", true);

		if (ThisBotController->GetBrainComponent() != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Bot dead"));
			this->Destroy();
			SpawnerWeapon->Destroy();
		}
	}
}

void AAICharacter::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Sword In"));
	if (OtherComp != nullptr && OtherActor != nullptr && OtherActor->IsA(AWeapon::StaticClass()))
	{
		AWeapon* EnemyWeapon = Cast<AWeapon>(OtherActor);

		//UE_LOG(LogTemp, Warning, TEXT("Enemy Weapon: %s, Character Weapon: %s"), *EnemyWeapon->GetName(), *SpawnerWeapon->GetName());
		if (EnemyWeapon == SpawnerWeapon)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Same Weapon Interacting. Return."));
			return;
		}
		else
		{
			ReduceAIBotHealth(EnemyWeapon->GetWeaponDamage());
			if (ThisBotController != nullptr)
			{
				ThisBotController->GetBlackboardComponent()->SetValueAsBool("HitByWeapon", true);
			}
		}
	}
}

void AAICharacter::OnWeaponOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Sword Out"));
	if (OtherComp != nullptr && OtherActor != nullptr)
	{
		AWeapon* EnemyWeapon = Cast<AWeapon>(OtherActor);
		if (ThisBotController != nullptr)
		{
			ThisBotController->GetBlackboardComponent()->SetValueAsBool("HitByWeapon", false);
		}
	}
}