// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PingHUD.generated.h"

/**
 * 
 */
UCLASS()
class PARAGONTEST_API APingHUD : public AHUD
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
		int ping=0;
private:
	
	UPROPERTY()
		UFont* HUDFont;

	virtual void DrawHUD() override;
	
	
	
};
