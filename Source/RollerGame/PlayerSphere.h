// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "PlayerSphere.generated.h"

UCLASS(config = Game)
class ROLLERGAME_API APlayerSphere : public APawn
{
	GENERATED_BODY()
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Ball;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "True"))
	class USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "True"))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = Ball)
		float Torque;
	UPROPERTY(EditAnywhere, Category = Ball)
		FVector TorqueVec;

	void UpdateTorque(float DeltaTime);
	void FixCam();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "True"))
		float Gravity = 1.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ball, meta = (AllowPrivateAccess = "True"))
		bool AllowGravity = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "True"))
		bool GravityOn = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ball, meta = (AllowPrivateAccess = "True"))
		bool GravityActive = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "True"))
		FVector CurrentVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ball, meta = (AllowPrivateAccess = "True"))
		bool Grounded;

	void TickGravity(float delta);

	void FlipGravity();

	void SetCurrentVelocity();

	void SetVelocity();

public:

	// Sets default values for this pawn's properties
	APlayerSphere();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//UFUNCTION(BlueprintCallable, Category="Rotate")

	FORCEINLINE class UStaticMeshComponent* GetBall() const { return Ball; }
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }

protected:
	void RollForward(float Val);
	void RollRight(float Val);
	void MouseX(float Val);
	void MouseY(float Val);
	//virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	
};