// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"


UCLASS()
class PARAGONTEST_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

//BP에서 사용
public:
	// Sets default values for this character's properties
	ACharacterBase();

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UFUNCTION(BlueprintNativeEvent, Category = "CustomAnim")
		void PlayMontage(EAnimMontage montage);


protected:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int code;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FString nick;



//C++ 에서만 사용
public:
	void SetCode(int _code) { code = _code; }
	void SetNickName(const FString& _nick) { nick = _nick; }
	int GetCode() { return code; }
	FString GetNick() { return nick; }

//Override 함수
protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void Turn(float Value);
	void LookUp(float Value);

	void NormalAttack();

public:	


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	FTimerHandle moveSendtimerHandle;
	void SetMoveTimer();
	void MoveInfoSend();

	bool GetMe() { return bMe; }
	void SetMe(bool val) { bMe = val; }
	//이동관련
private:
	bool bMoving;
	float moveForwardValue;
	float moveRightValue;

private:
	bool bMe;

};
