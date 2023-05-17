#include "SFPlayerController.h"
#include "SFGameState.h"
#include "SFPlayerState.h"

ASFPlayerController::ASFPlayerController()
{
	
}

void ASFPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//GetWorld()->GetGameState<ASFGameState>()->SFCharactersArray.Add(Cast<ASwordFightCharacter>(GetPawn()));
}

