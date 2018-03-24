// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "cSocket.h"
#include "SpawnVolume.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API ASpawnVolume : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class APickup> WhatToSpawn;

	FTimerHandle SpawnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelayRangeLow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelayRangeHigh;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UBoxComponent* GetWhereToSpawn() const { return WhereToSpawn; }

	// 불륨속 랜덤 좌표를 반환
	UFUNCTION(BlueprintPure, Category = "Spawning")
	FVector GetRandomPointInVolume();

private:
	// 아이템 스폰 위치를 지정할 박스 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* WhereToSpawn;

	// 새 pickup 생성을 관리
	void SpawnPickup();

	// 생성 딜레이 시간
	float SpawnDelay;

	// 소켓
	cSocket socket;
	
};
