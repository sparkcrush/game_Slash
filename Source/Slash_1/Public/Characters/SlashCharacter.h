// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "SlashCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class Aitem;
class UAnimMotage;
class ASoul;
class ATreasure;

UCLASS()
class SLASH_1_API ASlashCharacter : public ABaseCharacter,public IPickupInterface
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	
	//virtual void Tick(float DeltaTime) override;
	virtual void Tick(float DeltaTime)override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump()override;

	

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void SetOverlappingItem(Aitem* Item) override;
	virtual void AddSouls(ASoul* Soul)override;
	virtual void AddGold(ATreasure* Treasure)override;

protected:
	virtual void BeginPlay() override;
	


	/*
	* call back functions
	*/
	void MoveForward(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void MoveRight(float Value);
	void EKeyPressed();
	void Arm();
	void Disarm();
	virtual void Attack() override;
	void Dodge();

	bool HasEnoughStamina();

	bool IsOccupied();

	/**
	* Combat
	*/
	void EquipWeapon(AWeapon* Weapon);
	virtual void AttackEnd() override;
	virtual void DodgeEnd()override;
	virtual bool CanAttack() override;

	void PlayEquipMontage(const FName& SectionName);
	bool CanDisarm();
	bool CanArm();
	virtual void Die_Implementation()override;

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinnishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();
private:

	bool IsUnoccupied();
	void InitializeSlashOverlay();
	void SetHUDHealth();

	/* Character Components*/
	UPROPERTY(VisibleAnywhere);
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere);
	UCameraComponent* ViewCamera;
	UPROPERTY(VisibleAnywhere, Category = Hair);
	UGroomComponent* Hair;
	UPROPERTY(VisibleAnywhere, Category = Hair);
	UGroomComponent* Eyebrows;
	UPROPERTY(VisibleInstanceOnly);
	Aitem* OverlappingItem;
	

	
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	EActionState ActionState = EActionState::EAS_Unoccupied;
	UPROPERTY()
	class USlashOverlay* SlashOverlay;
public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};
