// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/UserDefinedEnum.h"
#include "AIStates.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAIStates : uint8
{
	EAI_Idle						UMETA(DisplayName = "Idle"),
	EAI_Patrol						UMETA(DisplayName = "Patrol Area"),
	EAI_Run							UMETA(DisplayName = "Running/Chase Enemy"),
	EAI_Flee						UMETA(DisplayName = "Flee enemy"),
	EAI_Attack						UMETA(DisplayName = "Attack Enemy"),
	EAI_Struck						UMETA(DisplayName = "Struck By Weapon"),
	EAI_Dead						UMETA(DisplayName = "Bot Dead"),
};

UCLASS()
class BATTLESIM_API UAIStates : public UUserDefinedEnum
{
	GENERATED_BODY()
	
	
	
	
};
