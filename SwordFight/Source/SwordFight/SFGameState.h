#pragma once

#include "CoreMinimal.h"
#include "SwordFightCharacter.h"
#include "GameFramework/GameState.h"
#include "SFGameState.generated.h"

UCLASS()
class SWORDFIGHT_API ASFGameState : public AGameState
{
	GENERATED_BODY()

public:
	ASFGameState();
	
	void SetCurrentRound(float NewCurrentRound);
	
public:
	/** The Number of rounds at this given moment */
	UPROPERTY(Replicated)
	int16 CurrentRound;

	/** Boolean to check whether the round is over. True = Round Over, False = In round */
	UPROPERTY()
	bool bIsRoundOver;

	/** Array of sword fight characters in the game currently */
	UPROPERTY()
	TArray<ASwordFightCharacter*> SFCharactersArray;

	int16 RoundAmount;
	
	/** Not sure if I will be needing these but good for team fight
	UPROPERTY()
	float TeamOneScore;
	UPROPERTY()
	float TeamTwoScore;
	*/
};