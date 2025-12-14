// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapon.h"
#include "Characters/SlashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interface/HitInterface.h"
#include "NiagaraComponent.h"

AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::Equip(USceneComponent* AttachComponent, FName AttachSocketName, AActor* InOwner, APawn* InInstigator, bool bPlaySound)
{
	if (ItemMesh && AttachComponent)
	{
		ItemState = EItemState::EIS_Equipped;
		AttachMeshToSocket(AttachComponent, AttachSocketName);
		SetOwner(InOwner);
		SetInstigator(InInstigator);
		DisableSphereCollision();
		if (bPlaySound)	PlayEquipSound();
		DeactivateEmbers();
	}
}

void AWeapon::AttachMeshToSocket(USceneComponent* AttachComponent, FName AttachSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(AttachComponent, TransformRules, AttachSocketName);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (WeaponBox)
	{
		WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponBoxOverlap);
	}
}

void AWeapon::PlayEquipSound()
{
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	}
}

void AWeapon::DisableSphereCollision()
{
	if (SphereComp)
	{
		SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::DeactivateEmbers()
{
	if (EmberComponent)	EmberComponent->Deactivate();
}

void AWeapon::ExecuteGetHit(const FHitResult& BoxHitResult)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHitResult.GetActor());
	if (HitInterface)
	{
		HitInterface->Execute_GetHit(BoxHitResult.GetActor(), BoxHitResult.ImpactPoint);
	}
}

bool AWeapon::IsIgnoreWeaponOverlapOrHit(AActor* OtherActor)
{
	if (!OtherActor || !GetOwner()) return true;
	return GetOwner()->ActorHasTag("Enemy") && OtherActor->ActorHasTag("Enemy");
}

void AWeapon::OnWeaponBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsIgnoreWeaponOverlapOrHit(OtherActor)) return;
	FHitResult BoxHit;
	BoxTrace(BoxHit);
	if (BoxHit.GetActor())
	{
		if (IsIgnoreWeaponOverlapOrHit(BoxHit.GetActor())) return;
		if (GetInstigator())
		{
			UGameplayStatics::ApplyDamage(BoxHit.GetActor(), Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
		}
		ExecuteGetHit(BoxHit);
		CreateField(BoxHit.ImpactPoint);
	}
}

void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	if (!BoxTraceStart || !BoxTraceEnd)
	{
		return;
	}
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	IgnoreActors.Append(ActorsToIgnoreDuringBoxTrace);
	UKismetSystemLibrary::BoxTraceSingle(this, Start, End, BoxTraceExtent, BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1, false, IgnoreActors,
		bDebugBoxTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		BoxHit, true
	);
	ActorsToIgnoreDuringBoxTrace.AddUnique(BoxHit.GetActor());
}
