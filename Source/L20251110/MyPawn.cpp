// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyRocket.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(46.f, 70.f, 15.f));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT("/Script/Engine.StaticMesh'/Game/Meshes/SM_P38_Body.SM_P38_Body'"));

	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}

	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);

	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);

	Left->SetRelativeLocation(FVector(37.1f, 21.0f, 1.0f));
	Right->SetRelativeLocation(FVector(37.1f, -21.0f, 1.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Propeller(TEXT("/Script/Engine.StaticMesh'/Game/Meshes/SM_P38_Propeller.SM_P38_Propeller'"));
	if (SM_Propeller.Succeeded())
	{
		Right->SetStaticMesh(SM_Propeller.Object);
		Left->SetStaticMesh(SM_Propeller.Object);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Box);
	SpringArm->TargetArmLength = 150.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->SocketOffset = FVector(0, 0, 50.0f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Box);
	Arrow->SetRelativeLocation(FVector(150.0f, 0, 0));

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	Movement->MaxSpeed = 0;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_FireAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/IA_Fire.IA_Fire'"));
	if (IA_FireAsset.Succeeded())
	{
		IA_Fire = IA_FireAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_MovementAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/IA_Movement.IA_Movement'"));
	if (IA_MovementAsset.Succeeded())
	{
		IA_Movement = IA_MovementAsset.Object;
	}
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//AddMovementInput(GetActorForwardVector());

	Left->AddRelativeRotation(FRotator(0, 0, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * 7200.0f));
	Right->AddRelativeRotation(FRotator(0, 0, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * 7200.0f));
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* UIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (UIC)
	{
		UIC->BindAction(IA_Fire, ETriggerEvent::Completed, this, &AMyPawn::EnhancedFire);
		UIC->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &AMyPawn::ProcessMovement);
	}
	//PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AMyPawn::Fire);
	////PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, AMyPawn::Fire);

	//PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyPawn::Pitch);
	//PlayerInputComponent->BindAxis(TEXT("Roll"), this, &AMyPawn::Roll);
}

void AMyPawn::EnhancedFire(const FInputActionValue& Value)
{
	Fire();
}

void AMyPawn::ProcessMovement(const FInputActionValue& Value)
{
	FVector2D WantedRotation = Value.Get<FVector2D>();
	//WantedRotation = WantedRotation * 60.0 * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	//AddActorLocalRotation(FRotator(WantedRotation.X,
	//	0,
	//	WantedRotation.Y));

	Pitch(WantedRotation.Y);
	Roll(WantedRotation.X);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *WantedRotation.ToString());
}

void AMyPawn::Fire()
{
	GetWorld()->SpawnActor<AMyRocket>(AMyRocket::StaticClass(),
		Arrow->K2_GetComponentToWorld());
}

void AMyPawn::Pitch(float Value)
{
	AddActorLocalRotation(FRotator(Value * 60.0 * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 0, 0));
}

void AMyPawn::Roll(float Value)
{
	AddActorLocalRotation(FRotator(0, 0, Value * 60.0 * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())));
}
