// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterBase.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"

// Sets default values
AMonsterBase::AMonsterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	maxSpeed = 400;
}

void AMonsterBase::SetMaxSpeed()
{
	maxSpeed = 400;
	GetCharacterMovement()->MaxWalkSpeed = maxSpeed;
}

// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonsterBase::CollisionOn()
{
}

void AMonsterBase::CollisionOff()
{
}

void AMonsterBase::HitStart()
{
	GetCharacterMovement()->MaxWalkSpeed = 0;
}

void AMonsterBase::HitEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = maxSpeed;
}

