// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BatteryCollectorGameMode.generated.h"

UENUM(BlueprintType)
enum class EBatteryPlayState: uint8
{
	EPlaying,
	EGameOver,
	EWon,
	EUnknown
};

UCLASS(minimalapi)
class ABatteryCollectorGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABatteryCollectorGameMode();

	virtual void Tick(float DeltaTime) override;

	// powertowin getter �Լ�
	UFUNCTION(BlueprintPure, Category = "Power")
	float GetPowerToWin() const;

	virtual void BeginPlay() override;

	// ���� �÷��̻��� ��ȯ
	UFUNCTION(BlueprintPure, Category = "Power")
	EBatteryPlayState GetCurrentState() const;

	// �� �÷��� ���� ����	
	void SetCurrentState(EBatteryPlayState NewState);

protected:
	// ĳ���Ͱ� �Ŀ��� ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtect = "true"))
	float DecayRate;

	// ���ӿ��� �̱�� ���� �Ŀ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtect = "true"))
	float PowerToWin;

	// HUD ȭ�鿡�� �� ���� Ŭ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtect = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	// HUD ��ü
	UPROPERTY()
	class UUserWidget* CurrentWidget;
private:
	// ���� ���¸� ������
	EBatteryPlayState CurrentState;
};



