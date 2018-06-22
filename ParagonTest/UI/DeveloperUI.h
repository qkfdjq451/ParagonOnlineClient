// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeveloperUI.generated.h"

/**
 * 
 */
UCLASS()
class PARAGONTEST_API UDeveloperUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "DeveloperUI")
		void ExportNavimesh();

	
};
