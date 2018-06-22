// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <map>
#include <vector>
#include "Character/CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "../Enums.h"
#include "Shinbi.generated.h"

/**
 * 
 */
UCLASS()
class PARAGONTEST_API AShinbi : public ACharacterBase
{
	GENERATED_BODY()
public:
	AShinbi();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = attack, meta = (AllowPrivateAccess = "tue"))
		class UCapsuleComponent* WeaponCollision;


public:

	void NormalAttack();

	UFUNCTION(BlueprintCallable, Category = "CustomAnim")
		void CollisionOn();

	UFUNCTION(BlueprintCallable, Category = "CustomAnim")
		void CollisionOff();

	UFUNCTION(BlueprintCallable, Category = "CustomAnim")
		void ResetCombo();

	UFUNCTION(BlueprintCallable, Category = "CustomAnim")
		void SaveCombo();

	//공격 관련
	void EndAttack();
private:
	bool bAttacking;
	int combo;
	EAnimMontage attackMontage;

	std::vector<std::pair<EAnimMontage,class AMonsterBase*>> damages;
	//Damage요청 관련
public:
	UFUNCTION(BlueprintCallable, Category = "Damage_Req")
		void Insert_Damage(class AMonsterBase* monster);
private:
	std::map<int,class AMonsterBase*> damaged_Target;
};
