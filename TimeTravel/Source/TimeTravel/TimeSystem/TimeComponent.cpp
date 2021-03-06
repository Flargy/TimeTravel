#include "TimeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimeTravel/TimeTravelGameModeBase.h"
#include "Math/UnrealMathVectorCommon.h"


UTimeComponent::UTimeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

/**
 * Fetches and sets cashed values
 */
void UTimeComponent::BeginPlay()
{
	Super::BeginPlay();
	OwningActor = GetOwner();
	auto GameMode = Cast<ATimeTravelGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->TimeSystemInstance.RegisterTimeComponent(this);
		SecondsToRewind = GameMode->TimeSystemInstance.GetSecondsToRewind();
	}

	PhysicsComponent = Cast<UPrimitiveComponent>(OwningActor->GetRootComponent());
	
}

/**
 * Collects saved data each frame and activates a clean up to delete old data when enough time has passed
 */
void UTimeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// The manual C++ way of storing data, currently used for easy showcase purposes
	FTimeData FrameData;
	FTransform OwnerTransform = OwningActor->GetTransform();
	FrameData.Location = OwnerTransform.GetLocation();
	FrameData.Rotation = OwnerTransform.GetRotation();
	FrameData.Velocity = OwningActor->GetVelocity();
	FrameData.DeltaTime = DeltaTime;
	FrameData.AngularVelocity = PhysicsComponent->GetPhysicsAngularVelocityInRadians();

	SavedData.Add(FrameData);
	// End of C++ version
	SaveData.Broadcast();

	FramesSincelastCleanup++;
	SecondSinceLastCleanup += DeltaTime;

	if (SecondSinceLastCleanup > SecondsToRewind)
	{
		PerformCleanup();
	}

}

/**
 * Disables the tick and physics of the attached actor and sets values for time reversal
 */
void UTimeComponent::BeginReverse()
{
	OwningActor->SetActorTickEnabled(false);
	SetComponentTickEnabled(false);
	PhysicsComponent->SetSimulatePhysics(false);
	
	CurrentRewindFrame = SavedData.Num() - 1;
	TimeInCurrentRewindFrame = 0;
	BeginRewind.Broadcast();
}

/**
 * Reactivates the tick and physics of the actor and applies values from the current frame
 */
void UTimeComponent::EndReverse()
{
	OwningActor->SetActorTickEnabled(true);
	SetComponentTickEnabled(true);
	PhysicsComponent->SetSimulatePhysics(true);
	
	FTimeData CurrentFrameData = SavedData[CurrentRewindFrame];

	PhysicsComponent->SetPhysicsLinearVelocity(CurrentFrameData.Velocity);
	PhysicsComponent->SetPhysicsAngularVelocityInRadians(CurrentFrameData.AngularVelocity);
	OwningActor->SetActorLocation(CurrentFrameData.Location);
	OwningActor->SetActorRotation(CurrentFrameData.Rotation);
	
	EndRewind.Broadcast();
	CurrentRewindFrame++;
	SavedData.RemoveAt(CurrentRewindFrame, SavedData.Num() - CurrentRewindFrame);

}

/**
 * Lerps in between frames and applies values to the attached actor
 */
void UTimeComponent::ReverseTick(float DeltaTime)
{
	TimeInCurrentRewindFrame -= DeltaTime;
	while (TimeInCurrentRewindFrame < 0.0f && CurrentRewindFrame > 0)
	{
		SecondSinceLastCleanup -= SavedData[CurrentRewindFrame].DeltaTime;
		FramesSincelastCleanup--;
		TimeInCurrentRewindFrame += SavedData[CurrentRewindFrame].DeltaTime;
		CurrentRewindFrame--;
	}

	TimeInCurrentRewindFrame = TimeInCurrentRewindFrame < 0.f ? 0.f : TimeInCurrentRewindFrame;

	LerpT = TimeInCurrentRewindFrame / SavedData[CurrentRewindFrame + 1].DeltaTime;


	FTimeData CurrentFrameData = SavedData[CurrentRewindFrame];
	FTimeData PreviousFrameData = SavedData[CurrentRewindFrame + 1];

	FVector NewPosition = FMath::Lerp(CurrentFrameData.Location, PreviousFrameData.Location, LerpT);
	OwningActor->SetActorLocation(NewPosition);

	FQuat NewQuat = FQuat::Slerp(CurrentFrameData.Rotation, PreviousFrameData.Rotation, LerpT);
	OwningActor->SetActorRotation(NewQuat);

	LoadData.Broadcast();
	
}

#pragma region CellectionOfSaveAndLoadFunctions

void UTimeComponent::SaveVector(FName VariableName, FVector Value)
{
	SavedData[GetArrayEndIndex()].SaveVector(VariableName, Value);
}

void UTimeComponent::SaveQuat(FName VariableName, FQuat Value)
{
	SavedData[GetArrayEndIndex()].SaveQuat(VariableName, Value);
}

void UTimeComponent::SaveFloat(FName VariableName, float Value)
{
	SavedData[GetArrayEndIndex()].SaveFLoat(VariableName, Value);
}

void UTimeComponent::SaveInt(FName VariableName, int Value)
{
	SavedData[GetArrayEndIndex()].SaveInt(VariableName, Value);
}

void UTimeComponent::SaveBool(FName VariableName, bool Value)
{
	SavedData[GetArrayEndIndex()].SaveBool(VariableName, Value);
}

FVector UTimeComponent::LoadSavedVector(FName VariableName, bool Interpolated, FVector DefaultReturnValue)
{
	FVector ToReturn = SavedData[CurrentRewindFrame].GetSavedVector(VariableName, DefaultReturnValue);

	if (Interpolated)
	{
		ToReturn = FMath::Lerp(ToReturn, SavedData[CurrentRewindFrame + 1].GetSavedVector(VariableName, DefaultReturnValue), LerpT);
	}

	return ToReturn;
}

FQuat UTimeComponent::WrappedLoadSavedQuat(FName VariableName, bool Interpolated, FRotator DefaultReturnValue /*= FRotator::ZeroRotator*/)
{
	FQuat QuatFromRotator = DefaultReturnValue.Quaternion();

	return LoadSavedQuat(VariableName, Interpolated, QuatFromRotator);
}

FQuat UTimeComponent::LoadSavedQuat(FName VariableName, bool Interpolated, FQuat DefaultReturnValue)
{
	FQuat ToReturn = SavedData[CurrentRewindFrame].GetSavedQuat(VariableName, DefaultReturnValue);

	if (Interpolated)
	{
		ToReturn = FQuat::Slerp(ToReturn, SavedData[CurrentRewindFrame + 1].GetSavedQuat(VariableName, DefaultReturnValue), LerpT);
	}

	return ToReturn;
}

float UTimeComponent::LoadSavedFloat(FName VariableName, bool Interpolated, float DefaultReturnValue)
{
	float ToReturn = SavedData[CurrentRewindFrame].GetSavedFloat(VariableName, DefaultReturnValue);

	if (Interpolated)
	{
		ToReturn = FMath::Lerp(ToReturn, SavedData[CurrentRewindFrame + 1].GetSavedFloat(VariableName, DefaultReturnValue), LerpT);
	}

	return ToReturn;
}

int UTimeComponent::LoadSavedInt(FName VariableName, bool Interpolated, int DefaultReturnValue)
{
	int ToReturn = SavedData[CurrentRewindFrame].GetSavedInt(VariableName, DefaultReturnValue);

	if (Interpolated)
	{
		ToReturn = FMath::Lerp(ToReturn, SavedData[CurrentRewindFrame + 1].GetSavedInt(VariableName, DefaultReturnValue), LerpT);
	}

	return ToReturn;
}

bool UTimeComponent::LoadSavedBool(FName VariableName, bool DefaultReturnValue)
{
	return SavedData[CurrentRewindFrame].GetSavedBool(VariableName, DefaultReturnValue);
}

#pragma endregion

/**
 * Removes saved frame data from more than SecondsToRewind (base value 5) ago
 */
void UTimeComponent::PerformCleanup()
{
	SavedData.RemoveAt(0, SavedData.Num() - FramesSincelastCleanup, true);
	
	FramesSincelastCleanup = 0;
	SecondSinceLastCleanup = 0.f;

}

/**
 * Returns the last element in SavedData 
 */
int UTimeComponent::GetArrayEndIndex()
{
	return SavedData.Num() - 1;
}

