// Fill out your copyright notice in the Description page of Project Settings.

#include "LaunchPad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "StealthGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
ALaunchPad::ALaunchPad()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	OverlapComp->SetBoxExtent(FVector(75.0f,75.0f, 50.0f));
	RootComponent = OverlapComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::HandleOverlap);

	LaunchStrenght = 1500.0f;
	LaunchPitchAngle = 35.0f;
}

void ALaunchPad::HandleOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Make rotator with specified Pitch and convert to direction vector * intensity
	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchPitchAngle;
	FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrenght;

	ACharacter* MyCharacter = Cast<ACharacter>(OtherActor);
	if (MyCharacter)
	{
		// Launch Player. Both bool values allows to ignore current player velocity
		MyCharacter->LaunchCharacter(LaunchVelocity, true, true);

		//Spawn FX
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaunchFX, GetActorLocation());
	}
	// Actor is not a player check if simulates physics
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);

		//Spawn FX
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaunchFX, GetActorLocation());
	}
}
