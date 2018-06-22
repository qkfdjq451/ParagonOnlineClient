// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "./UI/ChattingUI.h"
#include "./Character/CharacterBase.h"
#include "./Monster/MonsterBase.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"


TSubclassOf<class UChattingUI> UMyGameInstance::chattingUIClass = nullptr;
TSubclassOf<class ACharacterBase> UMyGameInstance::sinbiClass = nullptr;
TSubclassOf<class AMonsterBase> UMyGameInstance::buff_redClass = nullptr;
std::map <int, TSubclassOf<class AMonsterBase>&> UMyGameInstance::monsterClasses= { 
	{1, buff_redClass }
};

UMyGameInstance::UMyGameInstance()
{
	chattingUIClass = 
		ConstructorHelpers::FClassFinder<UChattingUI>		
		(TEXT("WidgetBlueprint'/Game/UI/ChattingUIBP.ChattingUIBP_C'")).Class;

	sinbiClass =
		ConstructorHelpers::FClassFinder<ACharacterBase>
		(TEXT("Blueprint'/Game/Character/Shinbi/BP_Shinbi.BP_Shinbi_C'")).Class;
	
	buff_redClass =
		ConstructorHelpers::FClassFinder<AMonsterBase>
		(TEXT("Blueprint'/Game/Monster/Buff_Red/BP_Buff_Red.BP_Buff_Red_C'")).Class;

	//network=CreateDefaultSubobject<FNetworkConnectMNG>("NetworkManager");
}

void UMyGameInstance::Init()
{
	Super::Init();	
	UE_LOG(LogTemp, Warning, TEXT("Init GI"));
}

void UMyGameInstance::Shutdown()
{	
	UE_LOG(LogTemp, Warning, TEXT("Shutdown GI"));
	Super::Shutdown();	
}

bool UMyGameInstance::ConnectServer()
{		
	return network->Connect();
}

void UMyGameInstance::NetworkRun()
{
	network->Run();
	return;
}

bool UMyGameInstance::NetworkRecvDataEmpty()
{
	return network->IsRecvDataEmpty();
}

bool UMyGameInstance::CreateNetwork()
{
	network = new FNetworkConnectMNG;

	if (network != nullptr)
	{
		return true;
	}
	return false;
}

bool UMyGameInstance::DeleteNetwork()
{
	if (network != nullptr)
	{
		delete network;
		network = nullptr;
		return true;
	}
	return false;
}

bool UMyGameInstance::IsNetworkConnect()
{
	return network->GetConnect();
}


FVector ToFVector(const FB::Vec3* vec)
{
	FVector vector;
	vector.X = vec->x();
	vector.Y = vec->y();
	vector.Z = vec->z();
	return vector;
}

FB::Vec3 ToFBVector(const FVector & vec)
{	
	return FB::Vec3(vec.X, vec.Y, vec.Z);
}
