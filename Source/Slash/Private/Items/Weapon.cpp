// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapon.h"
#include "Characters/SlashCharacter.h"

void AWeapon::Equip(USceneComponent* AttachComponent, FName AttachSocketName)
{
	if (ItemMesh && AttachComponent)
	{
		AttachMeshToSocket(AttachComponent, AttachSocketName);
		ItemState = EItemState::EIS_Equipped;
	}
}

void AWeapon::AttachMeshToSocket(USceneComponent* AttachComponent, FName AttachSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(AttachComponent, TransformRules, AttachSocketName);
}

void AWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
