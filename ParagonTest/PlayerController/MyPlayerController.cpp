// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Engine/World.h"
#include "../UI/ChattingUI.h"
#include "../GameModes/MyGameMode.h"


void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	gm=Cast<AMyGameMode>(GetWorld()->GetAuthGameMode());
}

void AMyPlayerController::GetChattingFocus()
{
	gm->GetChattingUI()->GetChatFocus(this);
}

void AMyPlayerController::SendChatting()
{
	
}
