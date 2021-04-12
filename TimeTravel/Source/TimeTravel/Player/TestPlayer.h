#pragma once
#include "GameFramework/Character.h"
#include "TestPlayer.generated.h"

UCLASS()
class ATestPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ATestPlayer();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	void BeginReverseTime();
	void EndReverseTime();

};