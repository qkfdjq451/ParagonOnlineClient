// Fill out your copyright notice in the Description page of Project Settings.

#include "PingHUD.h"
#include "Engine/Canvas.h"

void APingHUD::DrawHUD()
{
	FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);
	Super::DrawHUD();
	FString print;
	print.Append("Ping : ");
	print.AppendInt(ping);
	print.Append(" ms");
	DrawText(print, FColor::Black,50, 50);	

	
}
