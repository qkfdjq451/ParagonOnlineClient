// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterBase.generated.h"

UCLASS()
class PARAGONTEST_API AMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonsterBase();

	UFUNCTION(BlueprintCallable, Category = "Monster")
		void SetMaxSpeed();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float maxSpeed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	//»ç¿ëÀÚ
	UFUNCTION(BlueprintCallable, Category = "CustomAnim")
		void CollisionOn();

	UFUNCTION(BlueprintCallable, Category = "CustomAnim")
		void CollisionOff();

	UFUNCTION(BlueprintCallable, Category = "CustomAnim")
		void HitStart();

	UFUNCTION(BlueprintCallable, Category = "CustomAnim")
		void HitEnd();
public:
	int GetNumber() { return number; }
	void SetNumber(int _number) { number = _number; }
private:
	int number;
};
