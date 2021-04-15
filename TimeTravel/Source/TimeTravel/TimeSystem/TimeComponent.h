#pragma once
#include "Components/ActorComponent.h"
#include "TimeData.h"
#include "TimeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSaveData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBeginRewind);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEndRewind, int, FrameIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCleanup, int, FrameIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLoadData, int, FrameIndex, float, LerpValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTestSave, FTimeData, FrameData);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UTimeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTimeComponent();
	void BeginPlay() override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BeginReverse();
	void EndReverse();
	void ReverseTick(float DeltaTime);

	UPROPERTY(BlueprintAssignable)
	FSaveData SaveData;

	UPROPERTY(BlueprintAssignable)
	FLoadData LoadData;

	UPROPERTY(BlueprintAssignable)
	FBeginRewind BeginRewind;

	UPROPERTY(BlueprintAssignable)
	FEndRewind EndRewind;

	UPROPERTY(BlueprintAssignable)
	FCleanup CleanupArray;

	UPROPERTY(BlueprintAssignable)
	FTestSave TestSave;
	
	UFUNCTION(BlueprintCallable)
	void SaveFloat(FName VariableName, float Value);

	UFUNCTION(BlueprintCallable)
	void SaveVector(FName VariableName, FVector Value);

	//FVector GetSavedVector()


private:
	void PerformCleanup();

	AActor* OwningActor;

	UPROPERTY(VisibleAnywhere)
	UPrimitiveComponent* PhysicsComponent;

	UPROPERTY(EditAnywhere)
	float SecondsToRewind;

	float SecondSinceLastCleanup = 0.f;

	int FramesSincelastCleanup = 0;

	int CurrentRewindFrame = 0;
	float TimeInCurrentRewindFrame = 0.0f;


	TArray<FTimeData> SavedData;
};