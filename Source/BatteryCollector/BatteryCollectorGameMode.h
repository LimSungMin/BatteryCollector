// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BatteryCollectorGameMode.generated.h"

UCLASS(minimalapi)
class ABatteryCollectorGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABatteryCollectorGameMode();

	virtual void Tick(float DeltaTime) override;

protected:
	// ĳ���Ͱ� �Ŀ��� ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power")
	float DecayRate;
};



