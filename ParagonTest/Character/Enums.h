// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"

UENUM(BlueprintType)	
enum class EAnimMontage : uint8
{
	Combo1 	UMETA(DisplayName = "Combo1"),
	Combo2 	UMETA(DisplayName = "Combo2"),
	Combo3		UMETA(DisplayName = "Combo3"),

	Skill1		UMETA(DisplayName = "Skill1"),
	Skill2		UMETA(DisplayName = "Skill2"),
	Skill3		UMETA(DisplayName = "Skill3"),
};
