#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SwordFightCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS(config=Game)
class ASwordFightCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASwordFightCharacter();

	UFUNCTION()
	virtual void BeginPlay() override;
	
	/** Getter for Max Health. */
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	
	float SetCurrentHealth(float NewCurrentHealth);

	/** Getter for Current Health. */
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	
	/** Setter for Current Health. Clamps the value between 0 and MaxHealth and calls OnHealthUpdate. Should only be called on the server. */
	UFUNCTION()
	void UpdateCurrentHealth(float HealthValueToAdd);

	/** Event for taking damage. Overriden from APawn. */
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/** Event for dealing damage to the actor that was hit. */
	UFUNCTION(Server, Reliable)
	void ServerDealDamage(float DamageTaken, AActor* HitActor);
	
	/** Property Replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const override;
	
	/** Returns CameraBoom sub object **/
	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera sub object **/
	UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Enables movement and resets animation to idle */
	void PlayerRevive();

	/** Multicast for the player revive down to the clients */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayerRevive();

protected:
	
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Normal Attack */
	void Attack();

	/** Multicast for the attack of the character */
	UFUNCTION(NetMulticast, Reliable, Category = "Gameplay")
	void MulticastAttack();

	/** Server RPC for the attack of the character */
	UFUNCTION(Server, Reliable, Category = "Gameplay")
	void ServerAttack();

	/** Finish the attack after an attack was called */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void EndAttack();

	/** Disables the movement for the player and plays Death Animation */
	void Die();

	/** Pauses the animation of the sword fight character */
	void PauseAnimation();

	/** Checks if the local role is authority and remote role is autonomous or simulated */
	bool HasServerFunctionality();

private:
	/** Sends the multicast of player death down to the clients */
	UFUNCTION(NetMulticast, Reliable, Category = "Gameplay")
	void MulticastDie();

public:
	/** Boolean value to check if the player is attacking. True = Attacking. False = Not attacking */
	bool bIsAttacking;

	/** The Weapon that the character is currently holding */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UChildActorComponent* Weapon;
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float BaseLookUpRate;
	
	/** A boolean value to check if the player is dead. True = Dead. False = Not Dead */
	UPROPERTY(Replicated)
	bool bIsDead;
	
protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;
	
	/** A timer handle used for providing the attack rate delay in-between swings. */
	FTimerHandle AttackTimer;

	/** Used to pause the animation of a dead character */
	FTimerHandle DeathAnimationTimer;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DeathMontage;

	/** The player's maximum Health. This is the highest their health can be, and the value that their health starts at when spawned. */
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	/** The player's CurrentHealth. When reduced to 0, they are considered dead. */
	UPROPERTY(Replicated)
	float CurrentHealth;

	/** The death animation requires an offset to pause the animation after death */
	float DeathAnimationOffset;
};