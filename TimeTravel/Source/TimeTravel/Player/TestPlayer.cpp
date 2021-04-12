#include "TestPlayer.h"
#include "TimeTravel/TimeTravelGameModeBase.h"
#include "Kismet/GameplayStatics.h"

ATestPlayer::ATestPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATestPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("ReverseTime"), EInputEvent::IE_Pressed, this, &ATestPlayer::BeginReverseTime);

	PlayerInputComponent->BindAction(TEXT("ReverseTime"), EInputEvent::IE_Released, this, &ATestPlayer::EndReverseTime);
}

void ATestPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void ATestPlayer::BeginReverseTime()
{
	auto GameMode = Cast<ATimeTravelGameModeBase>(UGameplayStatics::GetGameMode(this));
	if(GameMode)
		GameMode->TimeSystemInstance.BeginRewind();
}

void ATestPlayer::EndReverseTime()
{
	auto GameMode = Cast<ATimeTravelGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
		GameMode->TimeSystemInstance.EndRewind();
}
