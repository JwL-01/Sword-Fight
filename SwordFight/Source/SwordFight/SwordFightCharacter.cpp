#include "SwordFightCharacter.h"
#include "SwordWeapon.h"
#include "SFPlayerState.h"

#include "DrawDebugHelpers.h"
#include "SFGameMode.h"
#include "SFGameState.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

//////////////////////////////////////////////////////////////////////////
// ASwordFightCharacter

ASwordFightCharacter::ASwordFightCharacter()
	: bIsAttacking(false)
	, bIsDead(false)
	, MaxHealth(100.0f)
	, CurrentHealth(MaxHealth)
	, DeathAnimationOffset(0.75f)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 350.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Replicate SwordFightCharacter to clients
	bReplicates = true;

	// Spawn Sword as child actor component and attach to weapon socket
	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("BasicSword"));
	Weapon->SetupAttachment(GetMesh(), FName("Weapon_Socket"));
}

void ASwordFightCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasServerFunctionality())
	{
		ASFGameMode* GM = Cast<ASFGameMode>(GetWorld()->GetAuthGameMode());
		GM->AddToPlayersLeftAlive(1);
		
		// TODO should not try to add into array from here
		GetWorld()->GetGameState<ASFGameState>()->SFCharactersArray.Add(this);
	}
}

float ASwordFightCharacter::SetCurrentHealth(float NewCurrentHealth)
{
	return CurrentHealth = NewCurrentHealth;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASwordFightCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASwordFightCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASwordFightCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turn rate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASwordFightCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASwordFightCharacter::LookUpAtRate);
	
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ASwordFightCharacter::Attack);
}

void ASwordFightCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASwordFightCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASwordFightCharacter::MoveForward(float Value)
{
	if (IsValid(Controller) && Value != 0.0f && false == bIsAttacking)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASwordFightCharacter::MoveRight(float Value)
{
	if (IsValid(Controller) && Value != 0.0f && false == bIsAttacking)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ASwordFightCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASwordFightCharacter, CurrentHealth);
	DOREPLIFETIME(ASwordFightCharacter, bIsDead);
}

float ASwordFightCharacter::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToTake = Super::TakeDamage(DamageTaken, DamageEvent, EventInstigator, DamageCauser);
	if (DamageToTake > 0)
	{
		//const float CurrentHealthRemaining = CurrentHealth - DamageTaken;
		UE_LOG(LogTemp,Warning, TEXT(" damage taken: %f "), DamageTaken);
		UpdateCurrentHealth(-DamageTaken);
		//UE_LOG(LogTemp, Warning, TEXT("This Player's Health is: %f"), CurrentHealth);
	}
	if (HasServerFunctionality() && bIsDead)
	{
		EventInstigator->GetPlayerState<ASFPlayerState>()->Kills += 1;
		ASFGameMode* GM = Cast<ASFGameMode>(GetWorld()->GetAuthGameMode());
		GM->AddToPlayersLeftAlive(-1);
		GM->CheckIfAllPlayersAreDead();
		
	}
	return DamageToTake;
}

void ASwordFightCharacter::ServerDealDamage_Implementation(float DamageTaken, AActor* HitActor)
{
	HitActor->TakeDamage(DamageTaken, FDamageEvent(UDamageType::StaticClass()), GetController(), this);
}

void ASwordFightCharacter::UpdateCurrentHealth(const float HealthValueToAdd)
{
	CurrentHealth += HealthValueToAdd;
	UE_LOG(LogTemp,Warning, TEXT("Current Player's Health: %f "), CurrentHealth);
	
	// Checking if this is Server
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
	}
	if (GetCurrentHealth() <= 0)
	{
		MulticastDie();
	}
}

void ASwordFightCharacter::Attack()
{
	if (false == bIsAttacking && false == bIsDead)
	{
		ASwordWeapon* CurrentWeapon = Cast<ASwordWeapon>(Weapon->GetChildActor());
		if (!ensureMsgf(IsValid(CurrentWeapon), TEXT("ASwordFightCharacter::Attack() Tried to attack with not valid weapon")))
		{
			return;
		}
		
		if (IsLocallyControlled())
		{
			ServerAttack();
		}
	
		bIsAttacking = true;
		const float SwordMontageInSeconds = PlayAnimMontage(CurrentWeapon->SwordAttackMontage);
		const UWorld* World = GetWorld();
		if (IsValid(World))
		{
			World->GetTimerManager().SetTimer(AttackTimer, this, &ASwordFightCharacter::EndAttack, SwordMontageInSeconds, false);
		}
	}
}

void ASwordFightCharacter::MulticastAttack_Implementation()
{
	if (false == IsLocallyControlled() && false == bIsDead)
	{
		Attack();
	}
}

void ASwordFightCharacter::ServerAttack_Implementation()
{
	MulticastAttack();
}

void ASwordFightCharacter::EndAttack()
{
	bIsAttacking = false;
}

void ASwordFightCharacter::Die()
{
	bIsDead = true;
	GetCharacterMovement()->DisableMovement();
	SetActorEnableCollision(false);
	
	float DeathMontageInSeconds = PlayAnimMontage(DeathMontage);
	GetWorldTimerManager().SetTimer(DeathAnimationTimer, this, &ASwordFightCharacter::PauseAnimation, DeathMontageInSeconds - DeathAnimationOffset, false);
}

void ASwordFightCharacter::MulticastDie_Implementation()
{
	Die();
}

void ASwordFightCharacter::PauseAnimation()
{
	GetMesh()->bPauseAnims = true;
}

bool ASwordFightCharacter::HasServerFunctionality()
{
	return GetLocalRole() == ROLE_Authority && (GetRemoteRole() == ROLE_AutonomousProxy || GetRemoteRole() == ROLE_SimulatedProxy);
}

void ASwordFightCharacter::PlayerRevive()
{
	bIsDead = false;
	GetCharacterMovement()->MovementMode = MOVE_Walking;
	SetCurrentHealth(GetMaxHealth());
	SetActorEnableCollision(true);
	GetMesh()->bPauseAnims = false;
}

void ASwordFightCharacter::MulticastPlayerRevive_Implementation()
{
	PlayerRevive();
}

