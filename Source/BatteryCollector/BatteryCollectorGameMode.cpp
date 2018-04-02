// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

ABatteryCollectorGameMode::ABatteryCollectorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// 기본 decay rate
	DecayRate = 0.01f;

	// AGameModeBase 를 상속받는 게임모드에서는 
	// 아래 코드가 있어야 캐릭터 액터의 tick 이 활성화 된다
	PrimaryActorTick.bCanEverTick = true;
}

void ABatteryCollectorGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 스폰된 캐릭터를 가져옴
	auto MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter)
	{
		// 캐릭터의 파워가 0 이상일 때
		if (MyCharacter->GetCurrentPower() > 0)
		{
			// decay rate 만큼 파워를 업데이트
			MyCharacter->UpdatePower(-DeltaTime * DecayRate * (MyCharacter->GetInitialPower()));
		}
	}
}

float ABatteryCollectorGameMode::GetPowerToWin() const
{
	return PowerToWin;
}

void ABatteryCollectorGameMode::BeginPlay()
{
	Super::BeginPlay();

	// power to win 을 initial power 기준으로 세팅
	auto MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter)
	{
		PowerToWin = (MyCharacter->GetInitialPower()) * 1.25f;
	}

	// 위젯 세팅
	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}
