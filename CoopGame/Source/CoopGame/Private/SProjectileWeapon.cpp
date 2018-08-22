// Fill out your copyright notice in the Description page of Project Settings.

#include "SProjectileWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


void ASProjectileWeapon::Fire()
{
	AActor* Owner = GetOwner();

	if (Owner)
	{
		FVector StartPoint;
		FRotator StartRotation;

		Owner->GetActorEyesViewPoint(StartPoint, StartRotation);

		FVector TraceDirection = StartRotation.Vector();

		FVector MuzzleLocation = WeaponMesh->GetSocketLocation(SocketName);

		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (Projectial)
		{
			GetWorld()->SpawnActor<AActor>(Projectial, MuzzleLocation, StartRotation, SpawnParam);
		}

	
		DrawDebugLine(GetWorld(), StartPoint, (StartPoint + TraceDirection * 2000), FColor(25, 217, 226), false, 2.f, 0, 1.f);

	}
}
