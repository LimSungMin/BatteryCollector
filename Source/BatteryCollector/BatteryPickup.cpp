// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryPickup.h"
#include "Components/StaticMeshComponent.h"

ABatteryPickup::ABatteryPickup() 
{
	GetMesh()->SetSimulatePhysics(true);
}

void ABatteryPickup::WasCollected_implementation()
{
	// 부모 클래스의 함수 호출
	Super::WasCollected_implementation();
	// 객체 파괴
	Destroy();
}
