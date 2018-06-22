// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameGameMode.h"
#include "NetworkConnectMNG.h"
#include "../MyGameInstance.h"
#include "../UI/ChattingUI.h"
#include "../UI/PingHUD.h"

#include "../Character/CharacterBase.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/GameFramework/Pawn.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "AIController.h"
#include "./Monster/MonsterBase.h"

#include <memory>


void AInGameGameMode::BeginPlay()
{
	Super::BeginPlay();
	hud = Cast<APingHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	GetWorldTimerManager().SetTimer(pingTimerHandle,this, &AInGameGameMode::CheckPing,2.5f,true);
	auto network = gi->GetNetwork();
	network->SendReq(PS::REQ_SELECT_CHARACTER);
	
}

void AInGameGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().PauseTimer(pingTimerHandle);
}

bool AInGameGameMode::RespawnMonster(const FB::Monster * monster)
{
	auto result = monsters.Find(monster->number());
	if (result == nullptr)
	{
		auto nick = monster->nick();
		auto pos = monster->position();
		FVector location;
		if (pos)
		{
			location.X = pos->x();
			location.Y = pos->y();
			location.Z = pos->z();
		}
		FTransform transform;
		transform.SetLocation(location);
		auto sp = FActorSpawnParameters();
		sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		auto node = UMyGameInstance::monsterClasses.find(monster->code());
		if (node!= UMyGameInstance::monsterClasses.end())
		{
			auto monsterClass = node->second;
			auto monsterBP = Cast<AMonsterBase>(GetWorld()->SpawnActor(monsterClass, &transform, sp));
			if (monsterBP)
			{
				monsterBP->SpawnDefaultController();
				monsterBP->SetNumber(monster->number());
				//monsterBP->SetCode(monster->code());
				//monsterBP->SetNickName(UTF8_TO_TCHAR(nick->c_str()));
				monsters.Add(monster->number(), monsterBP);
			 
			}
			else
			{
				/*{
					FString str = FString::Printf(TEXT("MonsterSpawnFail"));
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, str);
				}*/
			}
		}
		


	}

	return true;
}

bool AInGameGameMode::InsertCharacter(const FB::Character * charater)
{
	if (charater->code() == myCharacter->GetCode())
		return false;
	auto result = otherPlayers.Find(charater->code());
	if (result != nullptr)
		return false;
	auto nick = charater->nick();
	auto pos = charater->position();
	FVector location;
	if (pos)
	{
		location.X = pos->x();
		location.Y = pos->y();
		location.Z = pos->z();
	}
	FTransform transform;
	transform.SetLocation(location);
	auto sp = FActorSpawnParameters();
	sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	auto characterBP = Cast<ACharacterBase>(GetWorld()->SpawnActor(UMyGameInstance::sinbiClass, &transform, sp));
	if (characterBP != nullptr)
	{
		characterBP->SpawnDefaultController();
		characterBP->SetCode(charater->code());
		characterBP->SetNickName(UTF8_TO_TCHAR(nick->c_str()));
		otherPlayers.Add(charater->code(), characterBP);
	}

	else
	{
		FString str = FString::Printf(TEXT("Spawn False"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, str);
	}
	return true;
}

bool AInGameGameMode::RemoveCharacter(const FB::Character * character)
{
	auto code = character->code();
	if (code == myCharacter->GetCode())
		return false;
	auto result = otherPlayers.Find(code);
	if (result != nullptr)
	{
		otherPlayers.Remove(code);
		(*result)->Destroy();
	}
	return true;
}

void AInGameGameMode::GetQueueData(std::shared_ptr<union RecvBuffer> recv)
{
	using namespace std;
	
	auto symbol = recv->packet.symbol;
	switch (symbol)
	{
	case PS::CHATTING_ALL:
	{
		auto chat = FB::GetChat(&recv->packet.buffer);		
		FString msg;

		msg.Append(UTF8_TO_TCHAR(chat->nick()->c_str()));
		msg.Append(" : ");
		msg.Append(UTF8_TO_TCHAR(chat->message()->c_str()));
		chattingUI->NewChattingString(msg);
		break;
	}
	case PS::CON_SELECT_CHARACTER:
	{
		auto character = FB::GetCharacter(&recv->packet.buffer);
		auto pos = character->position();
		FVector location;
		if (pos)
		{
			location.X = pos->x();
			location.Y = pos->y();
			location.Z = pos->z();
		}
		FTransform transform;
		transform.SetLocation(location);
		auto sp = FActorSpawnParameters();
		
		sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		myCharacter = Cast<ACharacterBase>(GetWorld()->SpawnActor(UMyGameInstance::sinbiClass, &transform, sp));
		if (myCharacter != nullptr)
		{
			myCharacter->SetMe(true);
			myCharacter->SetCode(character->code());
			myCharacter->SetNickName(UTF8_TO_TCHAR(character->nick()->c_str()));
			GetWorld()->GetFirstPlayerController()->Possess(myCharacter);
		}
		
		auto fbb = gi->GetNetwork()->FBB_Make_Shared();
		auto mapinfo = FB::MapBuilder(*fbb);
		mapinfo.add_mapcode(character->mapcode());
		auto mapoffset = mapinfo.Finish();
		fbb->Finish(mapoffset);

		gi->GetNetwork()->SendReq(PS::REQ_ENTER_MAP, fbb);
	}
		break;
	case PS::CON_ENTER_MAP:
	{
		//TODO : Level 변경 루틴 추가
	}
	break;
	
	case PS::ENTER_NEW_CHARACTER_VECTOR:
	{
		auto characters = FB::GetCharacterVec(&recv->packet.buffer);
		auto cv = characters->charvector();

		for (auto character : *cv)
		{
			InsertCharacter(character);
		}	
	}
	break;
	case PS::ENTER_NEW_CHARACTER:
	{
		auto charater = FB::GetCharacter(&recv->packet.buffer);
		InsertCharacter(charater);
	}
		break;

	case PS::LEAVE_CHARACTER:
	{
		auto character = FB::GetCharacter(&recv->packet.buffer);
		RemoveCharacter(character);
	}
	break;

	case PS::PING_TEST:
	{
		Ping p;
		memcpy(&p, &recv->packet.buffer,sizeof(Ping));
		hud->ping = (std::chrono::system_clock::now().time_since_epoch().count() - p.time)/2000;
		break;
	}

	case PS::MOVING_VECTOR:
	{
		auto moveVec = FB::GetMoveVec(&recv->packet.buffer);
		auto vec = moveVec->movevector();
		if (vec == nullptr) return;
		for (auto character : *vec)
		{
			if (character->code() == myCharacter->GetCode())
			{
				auto serverpos = ToFVector(character->position());
				auto Clientpos = myCharacter->GetActorLocation();

				serverpos.Z = 0;
				Clientpos.Z = 0;

				auto distance =FVector::Distance(serverpos, Clientpos);
				if (distance > 250)
				{
					serverpos.Z = myCharacter->GetActorLocation().Z;
					myCharacter->SetActorLocation(serverpos);
				}
			}

			else
			{
				auto player = otherPlayers.Find(character->code());
				if (player)
				{
					if (character->state() != FB::MoveState::MoveState_MOVING)
					{						
						auto ai = Cast<AAIController>((*player)->GetController());
						if (ai)
						{
							FVector dest = ToFVector(character->position());
							ai->MoveToLocation(dest);
						/*	FString str = FString::Printf(TEXT("X : %f,Y : %f,Z : %f"), dest.X, dest.Y, dest.Z);
							GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, str);*/
						}
					}
					else
					{
						FVector position = ToFVector(character->position());
						FVector foward = ToFVector(character->foward());;
						FVector dest = position + foward * 0.5*character->speed();
						auto ai = Cast<AAIController>((*player)->GetController());
						if (ai)
						{
							ai->MoveToLocation(dest);
					/*		FString str = FString::Printf(TEXT("X : %f,Y : %f,Z : %f"), dest.X, dest.Y, dest.Z);
							GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, str);*/
						}
					}
					{	
						FString str = FString::Printf(TEXT("Speed : %f"), (*player)->GetVelocity().Size());
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, str);
					}
				}
			
			}		
		}
	}
	break;

	case PS::ATTACK:
	{
		auto attack = FB::GetAttack(&recv->packet.buffer);
		auto character = otherPlayers.Find(attack->code());
		if (character)
		{
			(*character);
			auto state = attack->state();
			(*character)->SetActorRotation(FRotator(0, attack->yaw(), 0));
			switch (state)
			{
			case FB::AttackState_Combo1:
				(*character)->PlayMontage(EAnimMontage::Combo1);
				break;
			case FB::AttackState_Combo2:
				(*character)->PlayMontage(EAnimMontage::Combo2);
				break;
			case FB::AttackState_Combo3:
				(*character)->PlayMontage(EAnimMontage::Combo3);
				break;
			case FB::AttackState_Skill1:
				break;
			case FB::AttackState_Skill2:
				break;
			case FB::AttackState_Skill3:
				break;		
			}
		}
	}
	case PS::RESPAWN_MONSTER_VEC:
	{
		auto monsterVec = FB::GetMonsterVec(&recv->packet.buffer);
		auto mv = monsterVec->monvector();
		if (mv)
		{
			for (auto monster : *mv)
			{
				RespawnMonster(monster);
			}
		}
	}

	case PS::MONSTER_MOVING_VECTOR:
	{
		auto moveVec = FB::GetMoveVec(&recv->packet.buffer);
		auto vec = moveVec->movevector();
		if (!vec) return;
		for (auto monster : *vec)
		{
			auto mon = monsters.Find(monster->code());
			if (mon)
			{
				if (monster->state() != FB::MoveState::MoveState_MOVING)
				{
					auto ai = Cast<AAIController>((*mon)->GetController());
					if (ai)
					{
						FVector dest = ToFVector(monster->position());
						ai->MoveToLocation(dest);
						/*	FString str = FString::Printf(TEXT("X : %f,Y : %f,Z : %f"), dest.X, dest.Y, dest.Z);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, str);*/
					}
				}
				else
				{
					FVector position = ToFVector(monster->position());
					FVector foward = ToFVector(monster->foward());;
					FVector dest = position + foward * 0.5*monster->speed();
					auto ai = Cast<AAIController>((*mon)->GetController());
					if (ai)
					{
						ai->MoveToLocation(dest);
						/*		FString str = FString::Printf(TEXT("X : %f,Y : %f,Z : %f"), dest.X, dest.Y, dest.Z);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, str);*/
					}
				}				
			}
		}
	}
	break;

	break;
	default:
		break;
	}
}


void AInGameGameMode::CheckPing()
{
	using namespace std;
	Ping p;
	p.time = chrono::system_clock::now().time_since_epoch().count();	
	gi->GetNetwork()->SendReq(PS::PING_TEST, &p, sizeof(p));
}
