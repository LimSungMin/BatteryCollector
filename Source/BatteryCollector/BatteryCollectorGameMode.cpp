// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "SpawnVolume.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PawnMovementComponent.h"

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
		// 일정 파워이상 모이면 게임상태를 EWon 으로 설정
		if (MyCharacter->GetCurrentPower() > PowerToWin)
		{
			SetCurrentState(EBatteryPlayState::EWon);
		}
		// 캐릭터의 파워가 0 이상일 때
		else if (MyCharacter->GetCurrentPower() > 0)
		{
			// decay rate 만큼 파워를 업데이트
			MyCharacter->UpdatePower(-DeltaTime * DecayRate * (MyCharacter->GetInitialPower()));
		}
		else 
		{
			SetCurrentState(EBatteryPlayState::EGameOver);
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

	// 모든 spawn volume actor를 찾아냄
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);
		if (SpawnVolumeActor)
		{
			SpawnVolumeActors.AddUnique(SpawnVolumeActor);
		}
	}

	SetCurrentState(EBatteryPlayState::EPlaying);

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

EBatteryPlayState ABatteryCollectorGameMode::GetCurrentState() const
{
	return CurrentState;
}

void ABatteryCollectorGameMode::SetCurrentState(EBatteryPlayState NewState)
{
	// 현재 상태 변경
	CurrentState = NewState;
	// 핸들러에게 현재 상태 전달
	HandleNewState(CurrentState);
}

void ABatteryCollectorGameMode::HandleNewState(EBatteryPlayState NewState)
{
	switch (NewState)
	{
	case EBatteryPlayState::EPlaying:
		{
			// Spawn volume 활성화
			for (auto Volume : SpawnVolumeActors)
			{
				Volume->SetSpawningActive(true);
			}
		}
		break;
	case EBatteryPlayState::EGameOver:
	{
		// Spawn volume 비활성화
		for (auto Volume : SpawnVolumeActors)
		{
			Volume->SetSpawningActive(true);
		}
		// 사용자 입력 비활성화
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			PlayerController->SetCinematicMode(true, false, false, true, true);
		}
		// 캐릭터 래그돌화
		ACharacter* MyChracter = UGameplayStatics::GetPlayerCharacter(this, 0);
		if (MyChracter)
		{
			MyChracter->GetMesh()->SetSimulatePhysics(true);
			MyChracter->GetMovementComponent()->MovementState.bCanJump = false;
		}
	}
		break;
	case EBatteryPlayState::EWon:
	{
		// Spawn volume 비활성화
		for (auto Volume : SpawnVolumeActors)
		{
			Volume->SetSpawningActive(false);
		}
	}
		break;
	case EBatteryPlayState::EUnknown:
		break;
	default:
		break;
	}
}
