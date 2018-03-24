// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// 아이템이 사용 가능한지
	bool bIsActive;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 아이템의 메시를 반환해줌
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return PickupMesh; }

	UFUNCTION(BlueprintPure, Category = "Pickup")
	bool IsActive();
	
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SetActive(bool NewPickupState);
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;

};
