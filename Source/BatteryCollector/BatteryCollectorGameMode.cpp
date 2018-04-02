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

	// �⺻ decay rate
	DecayRate = 0.01f;

	// AGameModeBase �� ��ӹ޴� ���Ӹ�忡���� 
	// �Ʒ� �ڵ尡 �־�� ĳ���� ������ tick �� Ȱ��ȭ �ȴ�
	PrimaryActorTick.bCanEverTick = true;
}

void ABatteryCollectorGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ������ ĳ���͸� ������
	auto MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter)
	{
		// ĳ������ �Ŀ��� 0 �̻��� ��
		if (MyCharacter->GetCurrentPower() > 0)
		{
			// decay rate ��ŭ �Ŀ��� ������Ʈ
			MyCharacter->UpdatePower(-DeltaTime * DecayRate * (MyCharacter->GetInitialPower()));
		}
	}
}

void ABatteryCollectorGameMode::BeginPlay()
{
	Super::BeginPlay();

	// power to win �� initial power �������� ����
	auto MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter)
	{
		PowerToWin = (MyCharacter->GetInitialPower()) * 1.25f;
	}

	// ���� ����
	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}
