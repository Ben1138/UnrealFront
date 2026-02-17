// Fill out your copyright notice in the Description page of Project Settings.
#include "Main.h"


// Sets default values
AMain::AMain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Hello, World!"));
}

// Called every frame
//void AMain::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

