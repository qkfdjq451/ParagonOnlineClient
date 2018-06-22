// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include <memory>
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PARAGONTEST_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()

	
public:
	//class USystemUI* GetSystemUI() { return systemUI; };
	class UChattingUI* GetChattingUI() { return chattingUI; };

	AMyGameMode();
	void BeginPlay();
	void EndPlay(const EEndPlayReason::Type EndPlayReason);

	void Tick(float datatime);
	
	virtual void GetQueueData(std::shared_ptr<union RecvBuffer> recv) {}
protected:

	class UMyGameInstance * gi;

	//static TSubclassOf<class USystemUI> systemUIClass;
	//class USystemUI* systemUI;

	//static TSubclassOf<class UChattingUI> chattingUIClass;
	class UChattingUI* chattingUI;
};
