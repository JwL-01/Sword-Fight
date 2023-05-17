#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SFPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFIGHT_API ASFPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ASFPlayerState();
	// all variables that everyone else needs to know about
	
	// this is not related to the player character (sword guy)
	// we do not want this to be attached to that actor
	UPROPERTY()
	uint16 Kills;
};
