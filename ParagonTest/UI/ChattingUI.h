// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChattingUI.generated.h"

/**
 * 
 */
UCLASS()
class PARAGONTEST_API UChattingUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Chatting")
		void GetChatFocus(class APlayerController* pc);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Chatting")
		void NewChattingString(const FString& str);

	UFUNCTION(BlueprintCallable, Category = "Chatting")
		void SendMessage(const FString& str);


	UFUNCTION(BlueprintNativeEvent, Category = "Chatting")
		void Test();
};
