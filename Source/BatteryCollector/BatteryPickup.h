// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "BatteryPickup.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYCOLLECTOR_API ABatteryPickup : public APickup
{
	GENERATED_BODY()
public:
	// 기본값 설정
	ABatteryPickup();
	
	void WasCollected_Implementation() override;

	// battery power getter 함수
	float GetPower();

protected:
	// 캐릭터에게 줄 power 계수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Power", Meta=(BlueprintProtected = "true"))
	float BatteryPower;
};
