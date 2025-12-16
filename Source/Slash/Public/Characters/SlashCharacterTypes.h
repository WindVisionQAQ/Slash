#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_UnEquipped UMETA(DisplayName = "UnEquipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_Equipping UMETA(DisplayName = "Equipping"),
	EAS_HitReacting UMETA(DisplayName = "HitReacting"),
};

UENUM(BlueprintType)
enum EDeadPose
{
	EDP_Death1 UMETA(DisplayName = "DeadPose1"),
	EDP_Death2 UMETA(DisplayName = "DeadPose2"),
	EDP_Death3 UMETA(DisplayName = "DeadPose3"),
	EDP_Death4 UMETA(DisplayName = "DeadPose4"),
	EDP_MAX UMETA(DisplayName = "DefaultMax"),
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged"),
	EES_NoState UMETA(DisplayName = "NoState"),
};