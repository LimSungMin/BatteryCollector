// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryPickup.h"
#include "Components/StaticMeshComponent.h"

ABatteryPickup::ABatteryPickup() 
{
	GetMesh()->SetSimulatePhysics(true);

	BatteryPower = 150.f;
}

void ABatteryPickup::WasCollected_Implementation()
{
	// 부모 클래스의 함수 호출
	Super::WasCollected_Implementation();
	// 객체 파괴
	Destroy();
}

float ABatteryPickup::GetPower()
{
	return BatteryPower;
}
