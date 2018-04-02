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

	// 딜레이 세팅
	SpawnDelayRangeLow = 1.0f;
	SpawnDelayRangeHigh = 4.5f;
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);
	
	// 소켓 연결
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
		// world 가 유효한지 확인
		UWorld* const world = GetWorld();
		if (world) {
			// spawn parameter 설정
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			// 랜덤 위치를 가져옴
			FVector SpawnLocation = GetRandomPointInVolume();

			// 랜덤 로테이션값 넣어줌
			FRotator SpawnRatation;
			SpawnRatation.Yaw = FMath::FRand() * 360.0f;
			SpawnRatation.Roll = FMath::FRand() * 360.0f;
			SpawnRatation.Pitch = FMath::FRand() * 360.0f;

			// 아이템을 생성
			// auto loc = socket.RequestLocation();
			APickup* const SpawnPickup = world->SpawnActor<APickup>(WhatToSpawn, SpawnLocation, SpawnRatation, SpawnParams);
			// socket.SendLocation(SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);
			

			SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);
		}
	}
}

