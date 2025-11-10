// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPC.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class L20251110_API AMyPC : public APlayerController
{
	GENERATED_BODY()
public:
	AMyPC();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
	//TSoftObjectPtr<UInputMappingContext> InputMapping;
	TObjectPtr<UInputMappingContext> InputMapping;
};
