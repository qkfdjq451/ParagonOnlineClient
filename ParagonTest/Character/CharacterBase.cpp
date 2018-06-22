// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "Runtime/Engine/Public/Engine.h"
#include "MyGameInstance.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	
	moveForwardValue = 0;
	moveRightValue = 0;
	bMoving = false;
	bMe = false;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::MoveForward(float Value)
{
	moveForwardValue = Value;
	if ((Controller != NULL) && (Value != 0.0f))
	{	
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACharacterBase::MoveRight(float Value)
{
	moveRightValue = Value;
	if ((Controller != NULL) && (Value != 0.0f))
	{		
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ACharacterBase::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ACharacterBase::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ACharacterBase::NormalAttack()
{

}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetMoveTimer();
}



// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ACharacterBase::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacterBase::LookUp);


}

void ACharacterBase::SetMoveTimer()
{
	if ((moveForwardValue != 0 || moveRightValue != 0) && bMoving==false)
	{
		bMoving = true;
		MoveInfoSend();
		GetWorldTimerManager().SetTimer(moveSendtimerHandle, this, &ACharacterBase::MoveInfoSend, 0.2, true);
	}

	if (moveForwardValue == 0 && moveRightValue == 0&& bMoving==true)
	{
		bMoving = false;
		GetWorldTimerManager().PauseTimer(moveSendtimerHandle);
		MoveInfoSend();
	}
}

void ACharacterBase::MoveInfoSend()
{
	auto gi = Cast<UMyGameInstance>(GetGameInstance());
	if (gi)
	{
		auto network = gi->GetNetwork();
		auto fbb=network->FBB_Make_Shared();
		auto MoveB = FB::MoveBuilder(*fbb);
		MoveB.add_code(code);
		auto foward = ToFBVector(GetVelocity());
		auto position = ToFBVector(GetActorLocation());
		MoveB.add_foward(&foward);
		MoveB.add_position(&position);
		MoveB.add_speed(GetVelocity().Size());
		bMoving ? MoveB.add_state(FB::MoveState::MoveState_MOVING) : MoveB.add_state(FB::MoveState::MoveState_STOP);
		fbb->Finish(MoveB.Finish());
		network->SendReq(PS::MOVING, move(fbb));
		{
			FString str = FString::Printf(TEXT("Speed : %f"), GetVelocity().Size());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, str);
		}
	}
}


void ACharacterBase::PlayMontage_Implementation(EAnimMontage montage)
{

}


