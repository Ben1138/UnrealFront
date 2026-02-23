// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. See LICENSE in the root of this repository or https://www.gnu.org/licenses/
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RealtimeMeshComponent/Public/RealtimeMeshComponent.h"

#include "ProcStaticMeshActor.generated.h"


UCLASS()
class AProcStaticMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProcStaticMeshActor();

	inline URealtimeMeshComponent* GetRealtimeMeshComponent()
	{
		return MeshComp;
	}

//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(Category = "RealtimeMeshActor", VisibleAnywhere, BlueprintReadOnly,
		meta = (ExposeFunctionCategories = "Mesh,Rendering,Physics,Components|StaticMesh", AllowPrivateAccess = "true"))
	URealtimeMeshComponent* MeshComp;
};
