


#include "Characters/SlashCharacter.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Items/Item.h"
#include "Items/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"

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

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
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

void ASlashCharacter::EquipItem(const FInputActionValue& Value)
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), OverlappingWeapon->GetItemArmAttachSocketName());
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
		CharacterState = OverlappingWeapon->GetItemCharacterStateOnEquipped();
	}
	else
	{
		if (CanDisarm())
		{
			PlayEquipMontage(FName("Disarm"));
			CharacterState = ECharacterState::ECS_UnEquipped;
			ActionState = EActionState::EAS_Equipping;
		}
		else if (CanArm())
		{
			PlayEquipMontage(FName("Arm"));
			check(EquippedWeapon);
			CharacterState = EquippedWeapon->GetItemCharacterStateOnEquipped();
			ActionState = EActionState::EAS_Equipping;
		}
	}

}

void ASlashCharacter::Attack(const FInputActionValue& Value)
{
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

bool ASlashCharacter::CanAttack()
{
	return CharacterState != ECharacterState::ECS_UnEquipped && ActionState == EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_UnEquipped &&
		EquippedWeapon;
}

bool ASlashCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_UnEquipped &&
		EquippedWeapon;
}

void ASlashCharacter::Arm()
{
	if (!EquippedWeapon)
	{
		return;
	}
	EquippedWeapon->AttachMeshToSocket(GetMesh(), EquippedWeapon->GetItemArmAttachSocketName());
}

void ASlashCharacter::Disarm()
{
	if (!EquippedWeapon)
	{
		return;
	}
	EquippedWeapon->AttachMeshToSocket(GetMesh(), EquippedWeapon->GetItemDisarmAttachSocketName());
}

void ASlashCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->ClearActorsToIgnore();
	}
}

void ASlashCharacter::PlayAttackMontage()
{
	if (!GetMesh()) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !AttackMontage) return;
	AnimInstance->Montage_Play(AttackMontage);
	int32 Selection = FMath::RandRange(0, 1);
	FName SectionName = FName();
	switch (Selection)
	{
	case 0:
		SectionName = FName("Attack1");
		break;
	case 1:
		SectionName = FName("Attack2");
		break;
	default:
		break;
	}
	AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
}

void ASlashCharacter::PlayEquipMontage(FName SectionName)
{
	if (!GetMesh()) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !EquipMontage) return;
	AnimInstance->Montage_Play(EquipMontage);
	AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInput->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ASlashCharacter::EquipItem);
		EnhancedInput->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
	}
}


