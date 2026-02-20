// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. See LICENSE in the root of this repository or https://www.gnu.org/licenses/
#include "ProcStaticMeshActor.h"
#include "UnrealFront/UnrealFront.h"

// Sets default values
AProcStaticMeshActor::AProcStaticMeshActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProcMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMeshComp"));
	RootComponent = ProcMeshComp;
}

// Called when the game starts or when spawned
//void AProcStaticMeshActor::BeginPlay()
//{
//	Super::BeginPlay();
//}

// Called every frame
//void AProcStaticMeshActor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}

