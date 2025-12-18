


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
#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"
#include "Components/AttributeComponent.h"

ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(GetRootComponent());

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);

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

void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint, const AActor* HitInstigator)
{
	Super::GetHit_Implementation(ImpactPoint, HitInstigator);
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	ActionState = EActionState::EAS_HitReacting;
}

float ASlashCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(Damage);
	return Damage;
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
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ASlashCharacter::Jump);
		EnhancedInput->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ASlashCharacter::EquipItem);
		EnhancedInput->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
	}
}

void ASlashCharacter::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
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

void ASlashCharacter::HitReactionEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitInput();
	InitSlashOverlay();
	Tags.Add("EngagableActor");
}

void ASlashCharacter::PlayEquipMontage(FName SectionName)
{
	PlayMontageSection(EquipMontage, SectionName);
}

bool ASlashCharacter::CanAttack()
{
	return CharacterState != ECharacterState::ECS_UnEquipped && IsUnoccupied();
}

void ASlashCharacter::AttackEnd()
{
	SetActionState(EActionState::EAS_Unoccupied);
}

bool ASlashCharacter::CanArm()
{
	return IsUnoccupied() &&
		CharacterState == ECharacterState::ECS_UnEquipped &&
		EquippedWeapon;
}

bool ASlashCharacter::CanDisarm()
{
	return IsUnoccupied() &&
		CharacterState != ECharacterState::ECS_UnEquipped &&
		EquippedWeapon;
}

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	if (!IsUnoccupied()) return;
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
		EquipWeapon(OverlappingWeapon);
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

void ASlashCharacter::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (GetSlashOverlay() && AttributeComp)
	{
		GetSlashOverlay()->SetHealthProgress(AttributeComp->GetHealthPercentage());
	}
}

void ASlashCharacter::EquipWeapon(AWeapon* OverlappingWeapon)
{
	OverlappingWeapon->Equip(GetMesh(), OverlappingWeapon->GetItemArmAttachSocketName(), this, this);
	OverlappingItem = nullptr;
	EquippedWeapon = OverlappingWeapon;
	CharacterState = OverlappingWeapon->GetItemCharacterStateOnEquipped();
}

void ASlashCharacter::InitInput()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashContext, 0);
		}
	}
}

USlashOverlay* ASlashCharacter::GetSlashOverlay()
{
	if (SlashHUDInst)
	{
		return SlashHUDInst->GetSlashOverlay();
	}
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		SlashHUDInst = Cast<ASlashHUD>(PlayerController->GetHUD());
		if (SlashHUDInst) return SlashHUDInst->GetSlashOverlay();
	}
	return nullptr;
}

void ASlashCharacter::InitSlashOverlay()
{
	if (USlashOverlay* SlashOverlay = GetSlashOverlay())
	{
		SlashOverlay->SetHealthProgress(AttributeComp->GetHealthPercentage());
		SlashOverlay->SetStaminaProgress(1.f);
		SlashOverlay->SetCoinCount(0);
		SlashOverlay->SetSoulCount(0);
	}
}

bool ASlashCharacter::IsUnoccupied() const
{
	return ActionState == EActionState::EAS_Unoccupied;
}

