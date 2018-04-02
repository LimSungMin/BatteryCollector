// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnVolume.h"
#include "Pickup.h"
#include "Engine/World.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"


// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn")); 
	RootComponent = WhereToSpawn;

	// ������ ����
	SpawnDelayRangeLow = 1.0f;
	SpawnDelayRangeHigh = 4.5f;
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);
	
	// ���� ����
// 	socket.InitSocket();
// 	if (socket.Connect("127.0.0.1", 8000)) {
// 		// socket.SendLocation(1, 2, 3);
// 	}
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetRandomPointInVolume()
{
	FVector SpawnOrigin = WhereToSpawn->Bounds.Origin;
	FVector SpawnExtent = WhereToSpawn->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}

void ASpawnVolume::SpawnPickup()
{
	if (WhatToSpawn != NULL) {
		// world �� ��ȿ���� Ȯ��
		UWorld* const world = GetWorld();
		if (world) {
			// spawn parameter ����
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			// ���� ��ġ�� ������
			FVector SpawnLocation = GetRandomPointInVolume();

			// ���� �����̼ǰ� �־���
			FRotator SpawnRatation;
			SpawnRatation.Yaw = FMath::FRand() * 360.0f;
			SpawnRatation.Roll = FMath::FRand() * 360.0f;
			SpawnRatation.Pitch = FMath::FRand() * 360.0f;

			// �������� ����
			// auto loc = socket.RequestLocation();
			APickup* const SpawnPickup = world->SpawnActor<APickup>(WhatToSpawn, SpawnLocation, SpawnRatation, SpawnParams);
			// socket.SendLocation(SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);
			

			SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);
		}
	}
}

