#include "FlockingManager.h"
#include "Agent.h"

#define AGENT_COUNT 20    

void UFlockingManager::Init(UWorld* world, UStaticMeshComponent* mesh) {
    UE_LOG(LogTemp, Warning, TEXT("MANAGER INIT"));

    World = world;

    float incr = (PI * 2.f) / AGENT_COUNT;
    for (int i = 0; i < AGENT_COUNT; i++) {
        if (World != nullptr) {
            FRotator rotation = FRotator();

            FVector location = FVector();
            location.X = FMath::Sin(incr * i) * 150.f;
            location.Z = FMath::Cos(incr * i) * 150.f;
            location.Y = FMath::Cos(incr * i) * 30.f;

            AAgent* agent = World->SpawnActor<AAgent>(location, rotation);
            agent->Init(mesh, i);
            Agents.Add(agent);
        }
    }

    initialized = true;
}

void UFlockingManager::Flock() {
    
    FVector v1, v2, v3, v4;
    
    for (int i = 0; i < AGENT_COUNT; i++) {
        v1 = GetCenter(i);  
        v2 = SocialDistancing(i);  
        v3 = MatchSpeed(i);
        v4 = BoundTo(i);

        FVector totalForce = (v1 + v2 + v3 + v4) * 0.01f;
        totalForce.Normalize();

        Agents[i]->Velocity += totalForce;
        setSpeed(i);
        Agents[i]->SetActorLocation(Agents[i]->GetActorLocation() + Agents[i]->Velocity);
    };
}

FVector UFlockingManager::GetCenter(int currentb) {
    FVector centerPos = FVector::ZeroVector;

    for (int i = 0; i < AGENT_COUNT; i++) {
        if (i != currentb) {
            centerPos.X += Agents[i]->GetActorLocation().X;
            centerPos.Y += Agents[i]->GetActorLocation().Y;
            centerPos.Z += Agents[i]->GetActorLocation().Z;
        }
    }

    centerPos /= AGENT_COUNT - 1;

    return (centerPos - Agents[currentb]->GetActorLocation()) / 100;
}

FVector UFlockingManager::SocialDistancing(int currentb) {
    FVector c = FVector::ZeroVector;

    for (int i = 0; i < AGENT_COUNT; i++) {
        if (i != currentb) {
            FVector itA = Agents[i]->GetActorLocation();
            FVector currA = Agents[currentb]->GetActorLocation();
            float distance = FVector::Distance(itA, currA);
            if (distance < 100) {
                c += (currA - itA) * 0.01f; 
            }
        }
    }

    return c;
}

FVector UFlockingManager::MatchSpeed(int currentb) {
    FVector avgVelocity = FVector::ZeroVector;

    for (int i = 0; i < AGENT_COUNT; i++) {
        if (i != currentb) {
            avgVelocity += Agents[i]->Velocity;
        }
    }

    avgVelocity /= AGENT_COUNT - 1;

    return (avgVelocity - Agents[currentb]->Velocity) * 0.01f; 
}

FVector UFlockingManager::BoundTo(int currentb) {
    int Xmin = -1000, Xmax = 1000, Ymin = -1000, Ymax = 1000, Zmin = -1000, Zmax = 1000;
    float boundStrength = 0.1f;

    FVector v = FVector::ZeroVector;
    FVector currentLocation = Agents[currentb]->GetActorLocation();
    FVector& velocity = Agents[currentb]->Velocity;

    // Check X-axis boundaries
    if (currentLocation.X < Xmin || currentLocation.X > Xmax) {
        velocity.X *= -1; // Reverse X velocity
    }

    // Check Y-axis boundaries
    if (currentLocation.Y < Ymin || currentLocation.Y > Ymax) {
        velocity.Y *= -1; // Reverse Y velocity
    }

    // Check Z-axis boundaries
    if (currentLocation.Z < Zmin || currentLocation.Z > Zmax) {
        velocity.Z *= -1; // Reverse Z velocity;
        velocity.Z += -10;
    }

    return v;
}


void UFlockingManager::setSpeed(int currentb) {
    int vlim = 50;
    FVector& v = Agents[currentb]->Velocity;

    if (v.Size() > vlim) {
        v = v.GetSafeNormal() * vlim;
    }
}
