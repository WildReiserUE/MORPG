// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class AItemBase;
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Item,
	Weapon,
	Armor,
	Arrow
};

UENUM(BlueprintType)
enum class EArmorType : uint8
{
	Light,
	Heavy,
	Magic
};

UENUM(BlueprintType)
enum class EArmorPart : uint8
{
	Helmet,
	UpperBody,
	LowerBody,
	FullBody,
	Gloves,
	Boots,
	Shield,
	Underwear
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	RightHand,
	LeftHand,
	DoubleHand,
	Bow
};

UENUM(BlueprintType)
enum class EWeaponClass : uint8
{
	Knife,
	Sword,
	Blunt,
	DualSword,
	Polearm,
	Bow,
	Shield
};

UENUM(BlueprintType)
enum class EWeaponAttackSpeed : uint8
{
	VerySlow,
	Slow,
	Normal,
	Fast,
	VeryFast
};

UENUM(BlueprintType)
enum class EItemGrade : uint8
{
	NoGrade,
	DGrade,
	CGrade,
	BGrade,
	AGrade,
	SGrade,
	S84Grade
};

USTRUCT(BlueprintType)
struct FArrowInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EItemGrade ArrowGrade;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystem* HitFX = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* HitSound = nullptr;
};

USTRUCT(BlueprintType)
struct FItemInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int ItemID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName ItemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMesh* ItemMesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* ItemIcon = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* PickupSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystem* PickupFX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystem* DropFX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bIsStackable = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int Count = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int Cost = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int Weight = 0;
};

USTRUCT(BlueprintType)
struct FWeaponInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EWeaponType WeaponType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EWeaponClass WeaponClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition="WeaponClass == EWeaponClass::Bow", EditConditionHides))
	TSubclassOf<AItemBase> WeaponArrow = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EItemGrade WeaponGrade;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EWeaponAttackSpeed AttackSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int Damage = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UDamageType> DamageType;
};

USTRUCT(BlueprintType)
struct FArmorInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EArmorType ArmorType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EArmorPart ArmorPart;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EItemGrade ArmorGrade;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float PhysicalDefence = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MagicalDefence = 0.0f;	
};
UCLASS()
class MORPG_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();
	virtual void Tick(float DeltaTime) override;
	void SpawnParticleFx(UParticleSystem* NewParticle);

	UFUNCTION()
	void RenderOn(UPrimitiveComponent* pComponent);

	UFUNCTION()
	void RenderOff(UPrimitiveComponent* pComponent);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void ChangeSettings();
#endif
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "StaticMesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ItemMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemSettings")
	EItemType ItemType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemSettings")
	FItemInfo ItemInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemSettings", meta = (EditCondition="ItemType == EItemType::Weapon", EditConditionHides))
	FWeaponInfo WeaponInfo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemSettings", meta = (EditCondition="ItemType == EItemType::Armor", EditConditionHides))
	FArmorInfo ArmorInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemSettings", meta = (EditCondition="ItemType == EItemType::Arrow", EditConditionHides))
	FArrowInfo ArrowInfo;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};