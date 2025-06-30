


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFrameWork/FloatingPawnMovement.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(Capsule);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMeshComponent"));
	BirdMesh->SetupAttachment(RootComponent);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	MovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComp"));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(Capsule);
	SpringArmComp->TargetArmLength = 300.f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
}

void ABird::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BirdContext, 0);
		}
	}
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

// 	if (PlayerInputComponent)
// 	{
// 		PlayerInputComponent->BindAxis("MoveForward", this, &ABird::MoveForward);
// 		PlayerInputComponent->BindAxis("Turn", this, &ABird::Turn);
// 		PlayerInputComponent->BindAxis("LookUp", this, &ABird::LookUp);
// 	}

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);
	}

}

void ABird::MoveForward(float Value)
{
	if (GetController() && Value != 0.f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ABird::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ABird::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ABird::Move(const FInputActionValue& Value)
{
	float MoveValue = Value.Get<float>();
	if (GetController() && MoveValue != 0.f)
	{
		AddMovementInput(GetActorForwardVector(), MoveValue);
	}
}

void ABird::Look(const FInputActionValue& Value)
{
	FVector2D VectorValue = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(VectorValue.X);
		AddControllerPitchInput(VectorValue.Y);
	}
}

