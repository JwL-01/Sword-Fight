#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SFPlayerController.generated.h"

/**
 * stuff for later ;(
 */
UCLASS()
class SWORDFIGHT_API ASFPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASFPlayerController();

	virtual void BeginPlay() override;
	
	// we can put the HUD here
};