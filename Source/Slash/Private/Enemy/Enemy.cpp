// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Slash/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	if (GetMesh())
	{
		GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		GetMesh()->SetGenerateOverlapEvents(true);
	}
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::PlayHitMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(HitMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitMontage);
	}
}

void AEnemy::DirectionalHitReaction(const FVector& ImpactPoint)
{
	const FVector ForwardVector = GetActorForwardVector();
	const FVector ToHit = ImpactPoint - GetActorLocation();
	const FVector ToHit_SameHeight = FVector(ToHit.X, ToHit.Y, ForwardVector.Z).GetSafeNormal();
	const double DotProduct_ToHitForward = FVector::DotProduct(ForwardVector, ToHit_SameHeight);
	const FVector CrossProduct_ToHitForward = FVector::CrossProduct(ToHit_SameHeight, ForwardVector);
	double Theta = FMath::RadiansToDegrees(FMath::Acos(DotProduct_ToHitForward));
	if (CrossProduct_ToHitForward.Z < 0)
	{
		Theta *= -1;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Cyan, FString::Printf(TEXT("Theta:%f"), Theta));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ForwardVector * 60.f, 8.f, FColor::Red, 3.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit_SameHeight * 60.f, 8.f, FColor::Green, 3.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct_ToHitForward * 60.f, 8.f, FColor::Blue, 3.f);
	FName SectionName = FName("FromBack");
	if (Theta > -45.f && Theta <= 45.f)
	{
		SectionName = FName("FromFront");
	}
	else if (Theta > -135.f && Theta <= -45.f)
	{
		SectionName = FName("FromRight");
	}
	else if (Theta > 45.f && Theta <= 135.f)
	{
		SectionName = FName("FromLeft");
	}
	PlayHitMontage(SectionName);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);

	DirectionalHitReaction(ImpactPoint);

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
}

