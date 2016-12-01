// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "AIBotController.h"
#include "Coin.h"
#include "AIStates.h"
#include "Weapon.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AICharacter.generated.h"

UCLASS()
class BATTLESIM_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BehaviorTree;

	//Getters for Basic CharacterAttributes
	UFUNCTION(BlueprintPure, Category = CharacterAttributes)
	int32 GetHealth();

	UFUNCTION(BlueprintPure, Category = CharacterAttributes)
	FName GetTeamName();

	UFUNCTION(BlueprintCallable, Category = CharacterAttributes)
	void SetTeamName(FName TeamName);

	UFUNCTION(BlueprintCallable, Category=CharacterAttributeModification)
	void ReduceAIBotHealth(int32 WeaponDamage);

	UPROPERTY(EditAnywhere, Category=Weapon)
	TSubclassOf<AWeapon> CharacterWeapon;

	AWeapon* SpawnerWeapon;

	UPROPERTY(EditAnywhere, Category=Socket)
	FName WeaponSocket;

	UPROPERTY()
	AAICharacter* EnemyCharacter;

	UPROPERTY()
	UBlackboardComponent* EnemyBlackboardComponent;	

protected:
	//Character Max Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= CharacterAttributes)
	int32 Health = 1000;

	//The team the bot belongs to in the simulation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterAttributes)
	FName TeamName;

	UPROPERTY()
	AAIBotController* ThisBotController;

	UFUNCTION()
	void OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnWeaponOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void BotDead();
};
