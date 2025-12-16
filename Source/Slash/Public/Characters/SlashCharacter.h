

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "InputActionValue.h"
#include "SlashCharacterTypes.h"
#include "SlashCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UInputMappingContext;
class UInputAction;
class AItem;
class UAnimMontage;
class AWeapon;

UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void GetHit_Implementation(const FVector& ImpactPoint, const AActor* HitInstigator) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FORCEINLINE AItem* GetOverlappingItem() const { return OverlappingItem; }
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCharacterState(ECharacterState NewState) { CharacterState = NewState; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetActionState(EActionState NewState) { ActionState = NewState; }
	UFUNCTION(BlueprintCallable)
	void Arm();
	UFUNCTION(BlueprintCallable)
	void Disarm();
	UFUNCTION(BlueprintCallable)
	void HitReactionEnd();
protected:
	virtual void BeginPlay() override;
	void PlayEquipMontage(FName SectionName);
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	bool CanArm();
	bool CanDisarm();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EquipItem(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
private:
	void EquipWeapon(AWeapon* OverlappingWeapon);
protected:
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* SlashContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;
private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	UGroomComponent* HairComp;

	UPROPERTY(VisibleAnywhere)
	UGroomComponent* EyebrowComp;
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	ECharacterState CharacterState = ECharacterState::ECS_UnEquipped;

	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

};
