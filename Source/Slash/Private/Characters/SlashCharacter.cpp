


#include "Characters/SlashCharacter.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(GetRootComponent());

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	HairComp = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	HairComp->SetupAttachment(GetMesh());
	HairComp->AttachmentName = TEXT("head");

	EyebrowComp = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrow"));
	EyebrowComp->SetupAttachment(GetMesh());
	EyebrowComp->AttachmentName = TEXT("head");
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashContext, 0);
		}
	}
}

void ASlashCharacter::MoveForward(float Value)
{
	const FRotator ControlRotation = GetControlRotation();
	FRotator ControlRotation_YawOnly(0.f, ControlRotation.Yaw, 0.f);
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ControlRotation_YawOnly);
	AddMovementInput(ForwardVector, Value);
}

void ASlashCharacter::MoveRight(float Value)
{
	const FRotator ControlRotation = GetControlRotation();
	FRotator ControlRotation_YawOnly(0.f, ControlRotation.Yaw, 0.f);
	FVector ForwardVector = UKismetMathLibrary::GetRightVector(ControlRotation_YawOnly);
	AddMovementInput(ForwardVector, Value);
}

void ASlashCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MoveValue = Value.Get<FVector2D>();
	const FRotator ControlRotation = GetControlRotation();
	FRotator ControlRotation_YawOnly(0.f, ControlRotation.Yaw, 0.f);
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ControlRotation_YawOnly);
	AddMovementInput(ForwardVector, MoveValue.Y);
	FVector RightVector = UKismetMathLibrary::GetRightVector(ControlRotation_YawOnly);
	AddMovementInput(RightVector, MoveValue.X);
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookValue.X);
		AddControllerPitchInput(LookValue.Y);
	}
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

// 	if (PlayerInputComponent)
// 	{
// 		PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASlashCharacter::MoveForward);
// 		PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASlashCharacter::MoveRight);
// 		PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ASlashCharacter::Turn);
// 	}

	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
	}
}


