// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryPickup.h"
#include "Components/StaticMeshComponent.h"

ABatteryPickup::ABatteryPickup() 
{
	GetMesh()->SetSimulatePhysics(true);
}

void ABatteryPickup::WasCollected_implementation()
{
	// �θ� Ŭ������ �Լ� ȣ��
	Super::WasCollected_implementation();
	// ��ü �ı�
	Destroy();
}
