// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashAnimInstance.h"
#include "Characters/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	SlashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner());
	if (SlashCharacter)
	{
		MovementComp = Cast<UCharacterMovementComponent>(SlashCharacter->GetMovementComponent());
	}
}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (MovementComp)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(MovementComp->Velocity);
		bIsFalling = MovementComp->IsFalling();
	}
	if (SlashCharacter)
	{
		CharacterState = SlashCharacter->GetCharacterState();
		ActionState = SlashCharacter->GetActionState();
		DeadPose = SlashCharacter->GetDeadPose();
	}
}
