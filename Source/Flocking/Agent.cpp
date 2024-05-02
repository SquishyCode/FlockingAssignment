#include "Agent.h"
#include "FlockingGameModeBase.h"
#include "Kismet/GameplayStatics.h"

AAgent::AAgent() {
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AgentMesh"));
	RootComponent = Mesh;
	Velocity = FVector(0.f);
}

void AAgent::BeginPlay() {
	Super::BeginPlay();
}

void AAgent::Init(UStaticMeshComponent* mesh, int id) {
	UE_LOG(LogTemp, Warning, TEXT("Agent initialized."));
	Mesh->SetStaticMesh(mesh->GetStaticMesh());
}

void AAgent::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	AFlockingGameModeBase* gmb = Cast<AFlockingGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	Velocity.Z = gmb->Speed;

	FVector loc = GetActorLocation();
	SetActorLocation(loc + Velocity);
}