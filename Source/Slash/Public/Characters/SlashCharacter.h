

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "InputActionValue.h"
#include "SlashCharacterTypes.h"
#include "Interface/PickInterface.h"
#include "SlashCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UInputMappingContext;
class UInputAction;
class AItem;
class UAnimMontage;
class AWeapon;
class ASlashHUD;
class USlashOverlay;

UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter, public IPickInterface
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void GetHit_Implementation(const FVector& ImpactPoint, const AActor* HitInstigator) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	virtual void Die() override;
	FORCEINLINE AItem* GetOverlappingItem() const { return OverlappingItem; }
	virtual void SetOverlappingItem(AItem* Item) override { OverlappingItem = Item; }
	virtual void AddSoul(class ASoul* Soul) override;
	virtual void AddGold(class ATreasure* Treasure) override;
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }

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
	UFUNCTION(BlueprintCallable)
	void DodgeEnd();
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
	void Dodge(const FInputActionValue& Value);
	virtual void HandleDamage(float DamageAmount);
private:
	void EquipWeapon(AWeapon* OverlappingWeapon);
	void InitInput();
	USlashOverlay* GetSlashOverlay();
	void InitSlashOverlay();
	bool IsUnoccupied() const;
	bool HasEnoughStaminaToDodge() const;
	void RegenStamina(float DeltaTime);
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

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DodgeAction;
private:
	ECharacterState CharacterState = ECharacterState::ECS_UnEquipped;
	EActionState ActionState = EActionState::EAS_Unoccupied;
	ASlashHUD* SlashHUDInst = nullptr;

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

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

};
