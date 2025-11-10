// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGM.h"
#include "MyPawn.h"
#include "MyPC.h"

AMyGM::AMyGM()
{
	//UE_LOG(LogTemp, Warning, TEXT("안녕하세요. %d %f %s"), 1, 2.0f, TEXT("동준이 나와"));
	DefaultPawnClass = AMyPawn::StaticClass(); //CDO 포인터, 기본 객체, 결국은 클래스 이름
	PlayerControllerClass = AMyPC::StaticClass();
}
