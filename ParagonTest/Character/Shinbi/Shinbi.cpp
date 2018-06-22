// Fill out your copyright notice in the Description page of Project Settings.

#include "Shinbi.h"
#include "ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MyGameInstance.h"
#include "PlayerController/MyPlayerController.h"
#include "../NetworkMNG/NetworkConnectMNG.h"
#include "./Monster/MonsterBase.h"

#include "MyGameInstance.h"

AShinbi::AShinbi()
{
	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	WeaponCollision->SetRelativeLocationAndRotation(FVector(5.0f, -5.0f, 50.0f), FRotator(-10.0f, 10.0f, 90.0f));
	bAttacking = false;
	combo = 0;
	attackMontage = EAnimMontage::Combo1;
	
	WeaponCollision->SetActive(false);
}

// Called to bind functionality to input
void AShinbi::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("NormalAttack", IE_Pressed, this, &AShinbi::NormalAttack);
}

void AShinbi::NormalAttack()
{
	if (bAttacking == false)
	{
		bAttacking = true;

		if (combo == 0)
			attackMontage = EAnimMontage::Combo1;
		else if (combo == 1)
			attackMontage = EAnimMontage::Combo2;
		else if (combo == 2)
			attackMontage = EAnimMontage::Combo3;
		PlayMontage(attackMontage);

		if (GetMe())
		{
			auto gi = Cast<UMyGameInstance>(GetGameInstance());
			auto network = gi->GetNetwork();
			auto fbb = network->FBB_Make_Shared();

			auto attackb = FB::AttackBuilder(*fbb);
			attackb.add_code(code);	
			auto rotation = GetActorRotation();			
			attackb.add_yaw(rotation.Yaw);
			FB::AttackState state= FB::AttackState::AttackState_MIN;
			switch (attackMontage)
			{
			case EAnimMontage::Combo1:
				state = FB::AttackState::AttackState_Combo1;
				break;
			case EAnimMontage::Combo2:
				state = FB::AttackState::AttackState_Combo2;
				break;
			case EAnimMontage::Combo3:
				state = FB::AttackState::AttackState_Combo3;
				break;		
			}
			attackb.add_state(state);
			fbb->Finish(attackb.Finish());
			network->SendReq(PS::ATTACK, fbb);
		}
	}
}

void AShinbi::CollisionOn()
{
	WeaponCollision->SetActive(true);
}

void AShinbi::CollisionOff()
{
	WeaponCollision->SetActive(false);
}

void AShinbi::ResetCombo()
{
	combo = 0;
	EndAttack();
}

void AShinbi::SaveCombo()
{
	combo++;
	EndAttack();
}

void AShinbi::EndAttack()
{
	bAttacking = false;
	if (GetMe())
	{
		if (damages.size() != 0)
		{
			auto gi = Cast<UMyGameInstance>(GetGameInstance());
			if (!gi) return;
			auto network = gi->GetNetwork();
			auto fbb = network->FBB_Make_Shared();
			std::vector<flatbuffers::Offset<FB::Damage>> damageVec;
			for (auto damage : damages)
			{
				auto damageb = FB::DamageBuilder(*fbb);
				damageb.add_attacker_code(GetCode());
				damageb.add_attacker_type(FB::PlayerType::PlayerType_Player);
				damageb.add_damaged_code(damage.second->GetNumber());
				damageb.add_damaged_type(FB::PlayerType::PlayerType_Monster);
				FB::AttackState type = FB::AttackState::AttackState_MIN;
				switch (attackMontage){
				case EAnimMontage::Combo1:
					type = FB::AttackState::AttackState_Combo1;
					break;
				case EAnimMontage::Combo2:
					type = FB::AttackState::AttackState_Combo2;
					break;
				case EAnimMontage::Combo3:
					type = FB::AttackState::AttackState_Combo3;
					break;
				case EAnimMontage::Skill1:
					type = FB::AttackState::AttackState_Skill1;
					break;
				case EAnimMontage::Skill2:
					type = FB::AttackState::AttackState_Skill2;
					break;
				case EAnimMontage::Skill3:
					type = FB::AttackState::AttackState_Skill3;
					break;
				}
				damageb.add_attackType(type);
				damageVec.push_back(damageb.Finish());
			}//for (auto damage : damages)			
			fbb->Finish(FB::CreateDamageVec(*fbb, fbb->CreateVector(damageVec)));
			network->SendReq(PS::REQ_DAMAGE_VECTOR, move(fbb));
		}
	}
}

void AShinbi::Insert_Damage(AMonsterBase * monster)
{
	if (GetMe())
		damages.push_back(std::make_pair(attackMontage,monster));
}
