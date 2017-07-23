// Fill out your copyright notice in the Description page of Project Settings.

#include "RollerGame.h"
#include "PlayerSphere.h"


// Sets default values
APlayerSphere::APlayerSphere()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/PlayerAssets/PlayerBallMesh.PlayerBallMesh"));
	
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerBall"));
	Ball->SetStaticMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Ball->SetSimulatePhysics(true);
	Ball->SetAngularDamping(1.f);
	Ball->SetLinearDamping(1.f);
	Ball->BodyInstance.MassScale = 6.0f;
	Ball->BodyInstance.MaxAngularVelocity = 2000.0f;	
	Ball->SetNotifyRigidBodyCollision(true);
	RootComponent = Ball;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CamArm"));
	SpringArm->AttachTo(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bAbsoluteRotation = true;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->RelativeRotation = FRotator(0.0f, 0.0f, 90.0f);
	SpringArm->RelativeLocation = FVector::ForwardVector*2;
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 3.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	

	Torque = 50000000.0f;
	
}

// Called when the game starts or when spawned
void APlayerSphere::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerSphere::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	UpdateTorque(DeltaTime);
	FixCam();
	if(AllowGravity||GravityActive)
		TickGravity(DeltaTime);
	SetCurrentVelocity();
	if (Ball->GetComponentVelocity().Z != 0)
	{
		SetVelocity();
	}

}

// Called to bind functionality to input
void APlayerSphere::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("RollForward",this, &APlayerSphere::RollForward);
	InputComponent->BindAxis("RollRight", this, &APlayerSphere::RollRight);
	InputComponent->BindAxis("MouseMovementX", this, &APlayerSphere::MouseX);
	InputComponent->BindAxis("MouseMovementY", this, &APlayerSphere::MouseY);
	InputComponent->BindAction("Gravity",IE_Pressed, this, &APlayerSphere::FlipGravity);
	
}

void APlayerSphere::UpdateTorque(float DeltaTime)
{
	if (!TorqueVec.Equals(FVector(0.0f, 0.0f, 0.0f)))
	{
		FRotator Temp = SpringArm->GetComponentRotation();
		Temp.Pitch = 0;
		FVector result = Temp.RotateVector(TorqueVec);
		Ball->AddTorque(result);
	}
}

void APlayerSphere::RollForward(float Val)
{
	TorqueVec.Y = Torque*Val;
	FVector dir;
	//Ball->AddTorque(dir.ForwardVector*Val*Torque);
}

void APlayerSphere::RollRight(float Val)
{
	TorqueVec.X = Torque*Val;
	FVector dir;
	//Ball->AddTorque(dir.RightVector*Val*Torque);
}

void APlayerSphere::MouseX(float Val)
{
	SpringArm->AddLocalRotation(FQuat(0, 0, Val / 500, 1));
}

void APlayerSphere::MouseY(float Val)
{
	SpringArm->AddLocalRotation(FQuat(0, Val/500, 0, 1));
}
void APlayerSphere::FixCam()
{
	FRotator FixedRotate;
	FixedRotate.Yaw = SpringArm->GetComponentRotation().Yaw;
	FixedRotate.Pitch = SpringArm->GetComponentRotation().Pitch;

	if (FixedRotate.Pitch < -30)
	{
		FixedRotate.Pitch = -30;
	}
	if (FixedRotate.Pitch > 30)
	{
		FixedRotate.Pitch = 30;
	}
	FixedRotate.Roll = 0;

	SpringArm->SetWorldRotation(FixedRotate);
}

void APlayerSphere::TickGravity(float Delta)
{
	if (GravityOn)
	{
		Ball->AddForce(FVector::UpVector*Ball->GetBodyInstance()->GetBodyMass() * 1700);
		Gravity -= 0.3*Delta;
		if (Gravity <= 0)
		{
			Gravity = 0;
			GravityOn = false;
			AllowGravity = false;
		}
	}
	else
	{
		Gravity += 0.1*Delta;
		if (Gravity > 1.0f)
		{
			Gravity = 1;
			AllowGravity = true;
		}
	}

}

void APlayerSphere::FlipGravity()
{
	if (AllowGravity)
	{
		GravityOn = !GravityOn;
	}
}

void APlayerSphere::SetCurrentVelocity()
{
	if (Ball->GetComponentVelocity().Z == 0)
	{
		CurrentVelocity = Ball->GetComponentVelocity();
	}
	CurrentVelocity.Z = Ball->GetComponentVelocity().Z;
}

void APlayerSphere::SetVelocity()
{
	Ball->SetPhysicsLinearVelocity(CurrentVelocity, false);
}