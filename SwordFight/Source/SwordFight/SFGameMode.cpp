#include "SFGameMode.h"
#include "SFGameState.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

ASFGameMode::ASFGameMode()
	:RoundTimer(60)
	,MaxRoundTimer(60)
	,PlayersLeftAlive(0)
{
}

void ASFGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	OnRoundEndResetPlayerEvent.AddDynamic(this, &ASFGameMode::DelayResetRound);
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), TargetPointArray);

	//TODO Find a better way to print out round timer and no more integer
	GetWorldTimerManager().SetTimer(RoundTimerHandle, this, &ASFGameMode::PrintOutRoundTimer, 1.0f, true);
}

AActor* ASFGameMode::GetRandomSpawnPoint(TArray<AActor*> Array) const
{
	int32 const ChooseRandom = FMath::RandRange(0, Array.Num() - 1);
	//UE_LOG(LogTemp, Warning, TEXT("This is the random in target point: %i"), ChooseRandom);
	return Array[ChooseRandom];
}

void ASFGameMode::ResetRound()
{
	AddToPlayersLeftAlive(GetNumPlayers() - PlayersLeftAlive);

	for(ASwordFightCharacter* CurrentCharacter : GetGameState<ASFGameState>()->SFCharactersArray)
	{
		const AActor* TargetPointActor = GetRandomSpawnPoint(TargetPointArray);
		CurrentCharacter->MulticastPlayerRevive();
		CurrentCharacter->SetActorLocation(TargetPointActor->GetActorLocation());
		CurrentCharacter->SetActorRotation(TargetPointActor->GetActorRotation());
	}
	SetRoundTimer(MaxRoundTimer);
}

void ASFGameMode::DelayResetRound()
{
	FTimerHandle TemporaryResetTimer;
	GetWorldTimerManager().SetTimer(TemporaryResetTimer, this, &ASFGameMode::ResetRound, 5, false);
}

void ASFGameMode::PrintOutRoundTimer()
{
	const FString RoundTimerMessage = FString::Printf(TEXT("Round Timer: %i"), RoundTimer);
	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::White, RoundTimerMessage);
	
	if (--RoundTimer <= 0)
	{
		GetWorldTimerManager().ClearTimer(RoundTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("ROUND IS OVER"));
		// do a delegate event for choosing the player with the most health to win
	}
}

void ASFGameMode::SetRoundTimer(float NewRoundTimer)
{
	RoundTimer = NewRoundTimer;
}

void ASFGameMode::AddToPlayersLeftAlive(int16 AddAmountToPlayersLeftAlive)
{
	PlayersLeftAlive += AddAmountToPlayersLeftAlive;
	UE_LOG(LogTemp,Warning, TEXT("Number of Players Left Alive: %i "), PlayersLeftAlive);
}

void ASFGameMode::CheckIfAllPlayersAreDead()
{
	ASFGameState* GS = GetGameState<ASFGameState>();
	if (!ensureMsgf(IsValid(GS), TEXT("hey ur game state is not working")))
	{
		return;
	}
	
	if (PlayersLeftAlive <= 1)
	{
		UE_LOG(LogTemp,Warning, TEXT("Round is over"));
		GS->SetCurrentRound(GS->CurrentRound + GS->RoundAmount);
		UE_LOG(LogTemp,Warning, TEXT("Current round: %i "), GS->CurrentRound);
		OnRoundEndResetPlayerEvent.Broadcast();
	}
}
