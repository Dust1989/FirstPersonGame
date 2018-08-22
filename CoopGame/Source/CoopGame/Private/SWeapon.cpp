// Fill out your copyright notice in the Description page of Project Settings.

#include "SWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/Engine.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(
	TEXT("COOP.DebugWeapons"), 
	DebugWeaponDrawing, 
	TEXT("Draw Debug Lines for Weapons"), 
	ECVF_Cheat);

// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	SocketName = "MuzzleSocket";
	TraceTargetName = "BeamEnd";
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASWeapon::Fire()
{
	AActor* Owner = GetOwner();

	FVector StartPoint;
	FRotator StartRotation;

	Owner->GetActorEyesViewPoint(StartPoint, StartRotation);

	FVector TraceDirection = StartRotation.Vector();

	FVector EndPoint = StartPoint + (TraceDirection * 10000.f);

	TraceEndPoint = EndPoint;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(Owner);
	QueryParams.bTraceComplex = true;

	if (FireEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(FireEffect, WeaponMesh, SocketName);
	}

	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartPoint, EndPoint, ECC_Visibility, QueryParams))
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor == nullptr) return;
		UGameplayStatics::ApplyPointDamage(HitActor, 20.f, TraceDirection, Hit, Owner->GetInstigatorController(), this, DamageType);

		TraceEndPoint = Hit.ImpactPoint;

		if (HitEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		}
	}

	if (TraceEffect)
	{
		FVector MuzzlePoint = WeaponMesh->GetSocketLocation(SocketName);
		UParticleSystemComponent* TraceEffectComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceEffect, MuzzlePoint);
		TraceEffectComp->SetVectorParameter(TraceTargetName, TraceEndPoint);

		UE_LOG(LogTemp, Warning, TEXT("trace....."));
	}

	if (DebugWeaponDrawing > 0)
	{
		DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor(25, 217, 226), false, 2.f, 0, 1.f);
	}
}

