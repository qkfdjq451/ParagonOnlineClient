// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameMode.h"
#include "../MyGameInstance.h"
#include "../UI/SystemUI.h"
#include "../UI/ChattingUI.h"

//
//TSubclassOf<class USystemUI> AMyGameMode::systemUIClass = nullptr;
//TSubclassOf<class UChattingUI> AMyGameMode::chattingUIClass = nullptr;

AMyGameMode::AMyGameMode()
{
	////
	//systemUIClass = ConstructorHelpers::FClassFinder<USystemUI>(TEXT("WidgetBlueprint'/Game/UI/SystemUIBP.SystemUIBP_C'")).Class;
	//chattingUIClass = ConstructorHelpers::FClassFinder<UChattingUI>(TEXT("WidgetBlueprint'/Game/UI/ChattingUIBP.ChattingUIBP_C'")).Class;
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();
	gi = Cast<UMyGameInstance>(GetGameInstance());
	
	if (gi->CreateNetwork())
	{
		if (gi->ConnectServer())
		{
			gi->NetworkRun();
		}
	}	
/*
	if (systemUIClass != nullptr)
	{
		systemUI = CreateWidget<USystemUI>(GetWorld(), systemUIClass);
		if (systemUI != nullptr)
		{
			systemUI->AddToViewport();
		}
	}
	*/

	if (UMyGameInstance::chattingUIClass != nullptr)
	{
		chattingUI = CreateWidget<UChattingUI>(GetWorld(), UMyGameInstance::chattingUIClass);
		if (chattingUI != nullptr)
		{
			chattingUI->AddToViewport();
		}
	}
}

void AMyGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	switch (EndPlayReason)
	{
	case EEndPlayReason::Type::Quit:
	case EEndPlayReason::Type::EndPlayInEditor:
		gi->DeleteNetwork();
		break;
	default:
		break;
	}



}

void AMyGameMode::Tick(float datatime)
{
	Super::Tick(datatime);
	
	if (gi != nullptr)
	{
		auto network = gi->GetNetwork();
		if (gi->IsNetworkConnect())
		{
			if (!gi->NetworkRecvDataEmpty())
			{				
				GetQueueData(network->GetRecvData());
			}
		}
		else
		{

		}
	}
}