// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "NetworkConnectMNG.h"
#include <memory>
#include <map>
#include "MyGameInstance.generated.h"

/**
 * 
 */


UCLASS()
class PARAGONTEST_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UMyGameInstance();

	virtual void Init() override;

	virtual void Shutdown() override;
	
	UFUNCTION(BlueprintCallable)
		bool ConnectServer();

	UFUNCTION(BlueprintCallable)
		void NetworkRun();

	UFUNCTION(BlueprintCallable)
		bool NetworkRecvDataEmpty();


	bool CreateNetwork();
	bool DeleteNetwork();

	class FNetworkConnectMNG* GetNetwork() { return network; };
	bool IsNetworkConnect();
private:	
	class FNetworkConnectMNG* network;
	bool Running = false;


	////////////////////////////////////////////////////////////////////////////////
	//BluePrint °´Ã¼µé

public:
	static TSubclassOf<class UChattingUI> chattingUIClass;
	static TSubclassOf<class ACharacterBase> sinbiClass;
	static std::map <int, TSubclassOf<class AMonsterBase>&> monsterClasses;
	static TSubclassOf<class AMonsterBase> buff_redClass;
};


FVector ToFVector(const FB::Vec3* vec);

FB::Vec3 ToFBVector(const FVector& vec);