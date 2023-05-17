#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SFGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoundEndResetPlayer);

/**
 * 
 */

UCLASS()
class SWORDFIGHT_API ASFGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ASFGameMode();

	UFUNCTION()
	virtual void BeginPlay() override;

	/** Repeating the round timer */
	UFUNCTION()
	void PrintOutRoundTimer();

	/** Restart the players in the game to their spawn points */
	UFUNCTION()
	void ResetRound();

	/** Give a delay before restarting the round */
	UFUNCTION()
	void DelayResetRound();

	/** Chooses a random spawn point in the game. Using the target points */
	UFUNCTION()
	AActor* GetRandomSpawnPoint(TArray<AActor*> Array) const;

	/** Set the round timer with a new time */
	UFUNCTION(Category = "Timer")
	virtual void SetRoundTimer(float NewRoundTimer);

	/** Decrease the number of players alive by one */
	UFUNCTION()
	void AddToPlayersLeftAlive(int16 AddAmountToPlayersLeftAlive);

	void CheckIfAllPlayersAreDead();

public:
	/** The timer for the round in the game */
	UPROPERTY(EditAnywhere)
	int32 RoundTimer;

	UPROPERTY()
	FTimerHandle RoundTimerHandle;

	UPROPERTY()
	TArray<AActor*> TargetPointArray;

	UPROPERTY(EditAnywhere)
	int32 MaxRoundTimer;

	/** Keeps track of the number of players alive to know when the round ends */
	UPROPERTY()
	int16 PlayersLeftAlive;

	/** Event that is fired when the round ends */
	UPROPERTY()
	FOnRoundEndResetPlayer OnRoundEndResetPlayerEvent;
	
};
