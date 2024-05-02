#pragma once

#include "CoreMinimal.h"
#include "FlockingManager.generated.h"

UCLASS()
class FLOCKING_API UFlockingManager : public UObject
{

public:
	GENERATED_BODY()

	void Init(UWorld* world, UStaticMeshComponent* mesh);
	void Flock();
	FVector GetCenter(int currentb);
	FVector SocialDistancing(int currentb);
	FVector MatchSpeed(int currentb);
	FVector BoundTo(int currentb);
	void setSpeed(int currentb);

private:
	UWorld* World;
	bool initialized;
	TArray<class AAgent*> Agents;
};