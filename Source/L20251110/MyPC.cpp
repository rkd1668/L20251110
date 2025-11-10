// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPC.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"


AMyPC::AMyPC()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_Default(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Blueprints/Input/IMC_P38.IMC_P38'"));
	if (IMC_Default.Succeeded())
	{
		InputMapping = IMC_Default.Object;
	}
}

void AMyPC::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSystem->AddMappingContext(InputMapping, 0);
			//if (!InputMapping.IsNull())
			//{
			//	InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
			//}
		}
	}
	//ULocalPlayer* LocalPlayer = GetLocalPlayer();
	//UMyPlayerSubsystem* MySubsystem = LocalPlayer->GetSubsystem<UMyPlayerSubsystem>();
}
