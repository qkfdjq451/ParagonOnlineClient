// Fill out your copyright notice in the Description page of Project Settings.

#include "ChattingUI.h"
#include "GameFramework/PlayerController.h"
#include "MyGameInstance.h"

void UChattingUI::GetChatFocus_Implementation(APlayerController*pc)
{

}

void UChattingUI::NewChattingString_Implementation(const FString & str)
{
}

void UChattingUI::Test_Implementation()
{
}

void UChattingUI::SendMessage(const FString& str)
{
	auto gi = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	if (gi != nullptr)
	{
		auto network = gi->GetNetwork();		
		auto fbb = network->FBB_Make_Shared();
		
		auto chatb = FB::ChatBuilder(*fbb);
		chatb.add_message(fbb->CreateString(std::string(TCHAR_TO_UTF8(*str))));
		auto chat = chatb.Finish();
		fbb->Finish(chat);
		
		UE_LOG(LogTemp, Warning, TEXT("Size: %d"),fbb->GetSize());
		network->SendReq(PS::CHATTING_ALL, move(fbb));
	}
}
