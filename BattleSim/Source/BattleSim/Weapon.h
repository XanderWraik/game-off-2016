// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class BATTLESIM_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category=WeaponAttributes)
	FName WeaponID;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintPure, Category = CharacterAttributes)
	int32 GetWeaponDamage();

protected:
	//Weapon Damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=WeaponAttributes)
	int32 WeaponDamage = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponAttributes)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponAttributes)
	UBoxComponent* WeaponContainer;
};
