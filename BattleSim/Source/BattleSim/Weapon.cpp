// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleSim.h"
#include "Weapon.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponContainer = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponContainer"));
	SetRootComponent(WeaponContainer);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(WeaponContainer);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

int32 AWeapon::GetWeaponDamage()
{
	return WeaponDamage;
}

