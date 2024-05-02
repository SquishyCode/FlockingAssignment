#include "FlockingManager.h"
#include "Agent.h"

#define AGENT_COUNT 10    

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

            AAgent* agent = World->SpawnActor<AAgent>(location, rotation);
            agent->Init(mesh, i);
            Agents.Add(agent);
        }
    }

    initialized = true;
}

void UFlockingManager::Flock() {
    
    FVector v1, v2, v3;
    
    for (int i = 0; i < AGENT_COUNT; i++) {
        v1 = GetCenter(i);  
        v2 = SocialDistancing(i);  
        v3 = MatchSpeed(i);

        Agents[i]->Velocity = Agents[i]->Velocity + v1 + v2 + v3;
        Agents[i]->GetActorLocation() = Agents[i]->GetActorLocation() + Agents[i]->Velocity;
    };
}

FVector UFlockingManager::GetCenter(int currentb) {

    FVector centerPos;

    for (int i = 0; i < AGENT_COUNT; i++) {
        if (Agents[i] != Agents[currentb]) {
            centerPos = centerPos + Agents[i]->GetActorLocation().X;
        };
    };

    centerPos.X = centerPos.X / AGENT_COUNT - 1;
    centerPos.Y = centerPos.Y / AGENT_COUNT - 1;
    centerPos.Z = centerPos.Z / AGENT_COUNT - 1;

    return (centerPos - Agents[currentb]->GetActorLocation()) / 100;

}

FVector UFlockingManager::SocialDistancing(int currentb) {
    
    FVector c;
    c.X = 0.0f;
    c.Y = 0.0f;
    c.Z = 0.0f;

    for (int i = 0; i < AGENT_COUNT; i++) {
        if (Agents[i] != Agents[currentb]) {
            FVector itA = Agents[i]->GetActorLocation();
            FVector currA = Agents[currentb]->GetActorLocation();
            if (abs((itA.X - currA.X) < 100 &&
                abs((itA.Y - currA.Y)) < 100 &&
                abs((itA.Z - currA.Z)) > 100)) {

                c.X = c.X - (itA.X - currA.X);
                c.Y = c.Y - (itA.Y - currA.Y);
                c.Z = c.Z - (itA.Z - currA.Z);
            };
        };
    };
    
    return c;

}

FVector UFlockingManager::MatchSpeed(int currentb) {

    FVector p;

    for (int i = 0; i < AGENT_COUNT; i++) {
        if (Agents[i] != Agents[currentb]) {
            p = p + Agents[i]->GetActorLocation();
        };
    };

    p.X = p.X / AGENT_COUNT - 1;
    p.Y = p.Y / AGENT_COUNT - 1;
    p.Z = p.Z / AGENT_COUNT - 1;


    return (p - Agents[currentb]->Velocity) / 8;


};
