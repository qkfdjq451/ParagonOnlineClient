// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PARAGONTEST_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void BeginPlay();
	UFUNCTION(BlueprintCallable, Category="Chatting")
		void GetChattingFocus();
	void SendChatting();
private:
	class AMyGameMode* gm;
	
};
