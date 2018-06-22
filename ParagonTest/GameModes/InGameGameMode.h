// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "MyGameMode.h"
#include "NetworkConnectMNG.h"
#include "InGameGameMode.generated.h"

/**
 *
 */

UCLASS()
class PARAGONTEST_API AInGameGameMode : public AMyGameMode
{
	GENERATED_BODY()
public:
	virtual void GetQueueData(std::shared_ptr<RecvBuffer> recv);
	class APingHUD* hud;
	void BeginPlay();
	void EndPlay(const EEndPlayReason::Type EndPlayReason);

	bool RespawnMonster(const FB::Monster* monster);
	bool InsertCharacter(const FB::Character* charater);
	bool RemoveCharacter(const FB::Character* charater);


	class ACharacterBase* myCharacter;
	TMap<int ,class ACharacterBase*> otherPlayers;
	TMap<int, class AMonsterBase*> monsters;
private:
	FTimerHandle pingTimerHandle;
	void CheckPing();
};
