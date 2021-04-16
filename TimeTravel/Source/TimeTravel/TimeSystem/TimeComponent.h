#pragma once
#include "Components/ActorComponent.h"
#include "TimeData.h"
#include "TimeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSaveData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBeginRewind);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEndRewind, int, FrameIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLoadData, int, FrameIndex, float, LerpValue);

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

	UFUNCTION(BlueprintCallable)
	void SaveVector(FName VariableName, FVector Value);

	UFUNCTION(BlueprintCallable)
	void SaveQuat(FName VariableName, FQuat Value);

	UFUNCTION(BlueprintCallable)
	void SaveFloat(FName VariableName, float Value);

	UFUNCTION(BlueprintCallable)
	void SaveInt(FName VariableName, int Value);

	UFUNCTION(BlueprintCallable)
	void SaveBool(FName VariableName, bool Value);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = 2))
	FVector LoadSavedVector(FName VariableName, bool Interpolated, FVector DefaultReturnValue = FVector::ZeroVector);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Load Saved Quat", AdvancedDisplay = 2))
	FQuat WrappedLoadSavedQuat(FName VariableName, bool Interpolated, FRotator DefaultReturnValue = FRotator::ZeroRotator);

	FQuat LoadSavedQuat(FName VariableName, bool Interpolated, FQuat DefaultReturnValue);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = 2))
	float LoadSavedFloat(FName VariableName, bool Interpolated, float DefaultReturnValue = 0.0f);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = 2))
	int LoadSavedInt(FName VariableName, bool Interpolated, int DefaultReturnValue = 0);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = 1))
	bool LoadSavedBool(FName VariableName, bool DefaultReturnValue = false);



private:
	void PerformCleanup();
	int GetArrayEndIndex();

	AActor* OwningActor;

	UPROPERTY(VisibleAnywhere)
	UPrimitiveComponent* PhysicsComponent;

	UPROPERTY(EditAnywhere)
	float SecondsToRewind;

	float SecondSinceLastCleanup = 0.f;

	int FramesSincelastCleanup = 0;

	int CurrentRewindFrame = 0;
	float TimeInCurrentRewindFrame = 0.0f;
	float LerpT = 0.f;


	TArray<FTimeData> SavedData;
};