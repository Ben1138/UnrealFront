// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. See LICENSE in the root of this repository or https://www.gnu.org/licenses/
#include "Importer.h"
#include "Misc/Paths.h"
#include "UnrealFrontGame.h"
#include "UnrealFront/UnrealFront.h"
#include "LuaAPI.h"
#include <LibSWBF2/LibSWBF2.h>

// something in the windows API....
#undef UpdateResource


UImporter::UImporter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Container = LibSWBF2::Container::Create();
}

UImporter::~UImporter()
{
	LibSWBF2::Container::Delete(Container);
}

LibSWBF2::Handle UImporter::AddLevel(const FString& path, const TSet<FString>* subLVLFilter)
{
	if (subLVLFilter == nullptr)
	{
		return Container->AddLevel(FSTR_TO_LIBSTR(path), nullptr);
	}

	LibSWBF2::List<LibSWBF2::String> subLVLsToLoad;
	for (const FString& subLVL : *subLVLFilter)
	{
		subLVLsToLoad.Add(FSTR_TO_LIBSTR(subLVL));
	}
	return Container->AddLevel(FSTR_TO_LIBSTR(path), &subLVLsToLoad);
}

LibSWBF2::Handle UImporter::AddSoundBank(const FString& path)
{
	//return Container->AddSoundBank(FSTR_TO_LIBSTR(path));
	return SWBF2HANDLE_INVALID;
}

void UImporter::StartLoading()
{
	Container->StartLoading();
}

void UImporter::FreeAll()
{
	Container->FreeAll();
}

bool UImporter::IsDone() const
{
	return Container->IsDone();
}

TSet<LibSWBF2::Handle> UImporter::GetLoadedLevels() const
{
	TSet<LibSWBF2::Handle> outHandles;
	LibSWBF2::List<LibSWBF2::Handle> handles = Container->GetLoadedLevels();
	for (size_t i = 0; i < handles.Size(); ++i)
	{
		outHandles.Add(handles[i]);
	}
	return outHandles;
}

LibSWBF2::ELoadStatus UImporter::GetLevelStatus(LibSWBF2::Handle handle) const
{
	return Container->GetStatus(handle);
}

float_t UImporter::GetLevelProgress(LibSWBF2::Handle handle) const
{
	return Container->GetLevelProgress(handle);
}

LibSWBF2::Level* UImporter::GetLevel(LibSWBF2::Handle handle) const
{
	return Container->GetLevel(handle);
}

float_t UImporter::GetOverallProgress() const
{
	return Container->GetOverallProgress();
}

FVector2D UImporter::ToUnreal(const LibSWBF2::Vector2& vector)
{
	return { vector.m_X, vector.m_Y };
}

FVector UImporter::ToUnreal(const LibSWBF2::Vector3& vector)
{
	// convert from SWBF2s right-handed-Y-up to Unreals left-handed-Z-up
	// switch Y and Z, flip X and Z
	return { -vector.m_X, -vector.m_Z, vector.m_Y };
}

FQuat UImporter::ToUnreal(const LibSWBF2::Vector4& quaternion)
{
	// convert from SWBF2s right-handed-Y-up to Unreals left-handed-Z-up
	// switch Y and Z, flip Z, mirror across X (via X and W flip)
	return FQuat(-quaternion.m_X, -quaternion.m_Z, quaternion.m_Y, -quaternion.m_W);
}

FColor UImporter::ToUnreal(const LibSWBF2::Color4u8& color)
{
	return { color.m_Red, color.m_Green, color.m_Blue, color.m_Alpha };
}

FLinearColor UImporter::ToLinearColor(const LibSWBF2::Color4f& color)
{
	return { color.m_Red, color.m_Green, color.m_Blue, color.m_Alpha };
}

TArray<FVector> UImporter::ConvertVertexBuffer(uint32 count, LibSWBF2::Vector3* buffer)
{
	TArray<FVector> result;
	for (uint32 i = 0; i < count; ++i)
	{
		// negate X and flip Y and Z to fit Unreal's coordinate system
		result.Add(ToUnreal(buffer[i]));
	}
	return result;
}

TArray<FVector> UImporter::ConvertVertexBuffer(uint32 count, LibSWBF2::Vector3* buffer, float multiplicator)
{
	TArray<FVector> result;
	for (uint32 i = 0; i < count; ++i)
	{
		// negate X and flip Y and Z to fit Unreal's coordinate system
		result.Add(ToUnreal(buffer[i]) * multiplicator);
	}
	return result;
}

//FRuntimeMeshCollisionVertexStream UImporter::ConvertVertexBufferRMC(uint32 count, LibSWBF2::Vector3* buffer, float multiplicator)
//{
//	FRuntimeMeshCollisionVertexStream result;
//	for (uint32 i = 0; i < count; ++i)
//	{
//		// negate X and flip Y and Z to fit Unreal's coordinate system
//		result.Add(ToUnreal(buffer[i]) * multiplicator);
//	}
//	return result;
//}

TArray<int32> UImporter::ConvertIndexBuffer(uint32 count, uint16* buffer)
{
	TArray<int32> result;
	for (uint32 i = 0; i < count; ++i)
	{
		result.Add((int32)buffer[i]);
	}
	return result;
}

TArray<int32> UImporter::ConvertIndexBuffer(uint32 count, uint32* buffer)
{
	TArray<int32> result;
	for (uint32 i = 0; i < count; ++i)
	{
		result.Add(buffer[i]);
	}
	return result;
}

//FRuntimeMeshCollisionTriangleStream UImporter::ConvertIndexBufferRMC(uint32 count, uint32* buffer)
//{
//	FRuntimeMeshCollisionTriangleStream result;
//	for (uint32 i = 0; i < count; i+=3)
//	{
//		result.Add(buffer[i + 2], buffer[i + 1], buffer[i]);
//	}
//	return result;
//}

TArray<FVector2D> UImporter::ConvertUVBuffer(uint32 count, LibSWBF2::Vector2* buffer)
{
	TArray<FVector2D> result;
	for (uint32 i = 0; i < count; ++i)
	{
		result.Add(ToUnreal(buffer[i]));
	}
	return result;
}

TArray<FColor> UImporter::ConvertColorBuffer(uint32 count, LibSWBF2::Color4u8* buffer)
{
	TArray<FColor> result;
	for (uint32 i = 0; i < count; ++i)
	{
		result.Add(ToUnreal(buffer[i]));
	}
	return result;
}

//ALight* UImporter::ImportLight(const LibSWBF2::Light& light, ULevel* spawnLevel)
//{
//	UWorld* targetWorld = UUnrealFrontGame::GetCurrentWorld();
//	if (targetWorld == nullptr)
//	{
//		UE_LOG(LogUF, Error, TEXT("Given targetWorld is NULL!"));
//		return nullptr;
//	}
//
//	FString lightName = LIBSTR_TO_TCHAR(light.GetName());
//	if (lightName == "Back")
//	{
//		UE_LOG(LogUF, Display, TEXT("asdf"));
//	}
//
//	FActorSpawnParameters params;
//	params.bDeferConstruction = false;
//	params.bNoFail = false;
//	//params.Name = lightName;
//	params.OverrideLevel = spawnLevel;
//	params.ObjectFlags = EObjectFlags::RF_Transient;
//
//	ALight* lightActor = nullptr;
//	FVector position = ToUnreal(light.GetPosition()) * 100.0f;
//
//	LibSWBF2::Vector4 libQuat = light.GetRotation();
//	FRotator rotation = ToUnreal(light.GetRotation()).Rotator();
//	//FRotator rotation = FQuat(libQuat.m_X, libQuat.m_Y, libQuat.m_Z, libQuat.m_W).Rotator();
//
//	// for some reason, Y is flipped for lights!?
//	position.Y = -position.Y;
//	//rotation = FRotator::MakeFromEuler({ -rotation.Roll, -rotation.Pitch, rotation.Yaw });
//	rotation.Pitch = -rotation.Pitch;
//	rotation.Roll = -rotation.Roll;
//	
//	rotation.Clamp();
//
//	FLinearColor color = ToLinearColor(light.GetColor());
//	ELightType lightType = light.GetType();
//	switch (lightType)
//	{
//		case ELightType::Omni:
//		{
//			float range;
//			APointLight* pointLight = targetWorld->SpawnActor<APointLight>(position, rotation, params);
//			pointLight->SetMobility(EComponentMobility::Movable);
//			pointLight->PointLightComponent->SetIntensityUnits(ELightUnits::Lumens);
//			pointLight->PointLightComponent->SetIntensity(100.0f);
//			if (light.GetRange(range))
//			{
//				pointLight->PointLightComponent->SetSourceRadius(range * 100.0f);
//				pointLight->PointLightComponent->SetAttenuationRadius(range * 200.0f);
//			}
//			else
//			{
//				UE_LOG(LogUF, Warning, TEXT("Light '%s' is typed as 'Omni', but has no range!"), LIBSTR_TO_TCHAR(light.GetName()));
//			}
//			lightActor = pointLight;
//			break;
//		}
//		case ELightType::Spot:
//		{
//			float innerAngle, outerAngle;
//			ASpotLight* spotLight = targetWorld->SpawnActor<ASpotLight>(position, rotation, params);
//			spotLight->SetMobility(EComponentMobility::Movable);
//			spotLight->SpotLightComponent->SetIntensityUnits(ELightUnits::Lumens);
//			spotLight->SpotLightComponent->SetIntensity(100.0f);
//			if (light.GetSpotAngles(innerAngle, outerAngle))
//			{
//				spotLight->SpotLightComponent->SetInnerConeAngle(innerAngle);
//				spotLight->SpotLightComponent->SetOuterConeAngle(outerAngle);
//			}
//			else
//			{
//				UE_LOG(LogUF, Warning, TEXT("Light '%s' is typed as 'Spot', but has no spot angles!"), LIBSTR_TO_TCHAR(light.GetName()));
//			}
//			lightActor = spotLight;
//			break;
//		}
//		case ELightType::Dir:
//		{
//			lightActor = targetWorld->SpawnActor<ADirectionalLight>(position, rotation, params);
//			//lightActor->GetTransform().TransformRotation(FQuat::MakeFromEuler(FVector(-rotation.Roll, -rotation.Pitch, rotation.Yaw)));
//			//lightActor->GetTransform().TransformRotation(FQuat::MakeFromEuler(FVector(10.0f, 20.0f, 30.0f)));
//			FVector euler = lightActor->GetTransform().GetRotation().Euler();
//			FRotator rotator = lightActor->GetTransform().Rotator();
//
//			lightActor->GetLightComponent()->SetIntensity(2.0f);
//			lightActor->SetMobility(EComponentMobility::Movable);
//			break;
//		}
//		case ELightType::Unknown:
//			UE_LOG(LogUF, Warning, TEXT("Encountered 'Unknown' Light"));
//			return nullptr;
//		default:
//			UE_LOG(LogUF, Error, TEXT("Unknown Light type: %i"), (int)lightType);
//			return nullptr;
//	}
//
//	lightActor->SetLightColor(color);
//#ifdef WITH_EDITOR
//	lightActor->SetActorLabel(lightName);
//#endif
//	return lightActor;
//}

AProcStaticMeshActor* UImporter::ImportModel(const LibSWBF2::Model& model, const FModelImportSettings& modelSettings, const FMaterialImportSettings& materialSettings, ULevel* spawnLevel, FString nameOverride)
{
	UWorld* targetWorld = UUnrealFrontGame::GetCurrentWorld();
	if (targetWorld == nullptr)
	{
		UE_LOG(LogUF, Error, TEXT("Given targetWorld is NULL!"));
		return nullptr;
	}
	if (materialSettings.DefaultMaterial == nullptr)
	{
		UE_LOG(LogUF, Warning, TEXT("No default Material specified!"));
		return nullptr;
	}

	//AProcStaticMeshActor** cached = ModelCache.Find(LIBSTR_TO_TCHAR(model.GetName()));
	FString modelName = LIBSTR_TO_TCHAR(model.GetName());
	FString instanceName = !nameOverride.IsEmpty() ? nameOverride : modelName;
	
	FActorSpawnParameters params;
	params.bDeferConstruction = true;
	params.bNoFail = true;
	//params.Name = *instanceName;
	params.OverrideLevel = spawnLevel;
	params.ObjectFlags = EObjectFlags::RF_Transient;

	// copy cached actor
	//params.Template = cached != nullptr ? *cached : nullptr;

	AProcStaticMeshActor* actor = targetWorld->SpawnActor<AProcStaticMeshActor>(modelSettings.SpawnPosition, modelSettings.SpawnRotation, params);
	//actor->SetMobility(EComponentMobility::Stationary);
	//actor->SetRuntimeMeshMobility(ERuntimeMeshMobility::Stationary);

#ifdef WITH_EDITOR
	actor->SetActorLabel(instanceName);
#endif

	// TODO: proper mesh caching / multi instancing
	//if (cached != nullptr)
	//{
	//	// just return cached copy
	//	return actor;
	//}

	UProceduralMeshComponent* comp = actor->GetProcMeshComp();
	//URuntimeMeshProviderStatic* provider = NewObject<URuntimeMeshProviderStatic>();
	//comp->Initialize(provider);

	const LibSWBF2::List<LibSWBF2::Segment>& segments = model.GetSegments();
	for (size_t i = 0; i < segments.Size(); ++i)
	{
		LibSWBF2::ETopology topology = segments[i].GetTopology();

		TArray<FVector> vertices;
		TArray<int32> triangles;
		TArray<FVector> normals;
		TArray<FVector2D> uv0;
		TArray<FVector2D> uv1;
		TArray<FVector2D> uv2;
		TArray<FVector2D> uv3;
		TArray<FColor> vertexColors;
		TArray<FProcMeshTangent> tangents;

		{
			uint32 count;
			LibSWBF2::Vector3* buffer;
			segments[i].GetVertexBuffer(count, buffer);
			vertices = ConvertVertexBuffer(count, buffer, 100.0f);
		}

		{
			uint32 count;
			uint16* buffer;
			segments[i].GetIndexBuffer(count, buffer);
			TArray<int32> indices = ConvertIndexBuffer(count, buffer);

			if (topology == LibSWBF2::ETopology::TriangleList)
			{
				// Unreal expects Triangles Lists, so no conversion needed
				// TODO: or is there? we shall see...
				triangles = indices;
				UE_LOG(LogUF, Warning, TEXT("Segment %i of %s is TriangleList"), i, *instanceName);
			}
			else if (topology == LibSWBF2::ETopology::TriangleStrip)
			{
				// convert from Triangle Strip to Triangle List (what Unreal expects)
				int32 triCount = 0;
				int32 numIndices = indices.Num();
				for (int32 j = 0; j < numIndices; ++j)
				{
					if (triCount == 3)
					{
						j -= 2;
						triCount = 0;
					}

					triangles.Add(indices[j]);
					triCount++;
				}

				//after conversion to Triangle List, indices are listed CW CCW CW CCW, so let's flip them
				bool flip = false;
				int32 numTriangles = triangles.Num();
				for (int32 j = 0; j < numTriangles; j += 3)
				{
					if (flip)
					{
						Swap(triangles[j], triangles[j + 2]);
					}
					flip = !flip;
				}
			}
			else
			{
				UE_LOG(LogUF, Warning, TEXT("Skipping unsupported segment topology: %s"), LIBSTR_TO_TCHAR(LibSWBF2::TopologyToString(topology)));
				continue;
			}
		}

		{
			uint32 count;
			LibSWBF2::Vector3* buffer;
			segments[i].GetNormalBuffer(count, buffer);
			normals = ConvertVertexBuffer(count, buffer);
		}

		{
			uint32 count;
			LibSWBF2::Vector2* buffer;
			segments[i].GetUVBuffer(count, buffer);
			uv0 = ConvertUVBuffer(count, buffer);
		}

		comp->CreateMeshSection(i, vertices, triangles, normals, uv0, uv1, uv2, uv3, vertexColors, tangents, false);

		//UMaterialInstanceDynamic* material = comp->CreateDynamicMaterialInstance(i, materialSettings.DefaultMaterial);
		//provider->SetupMaterialSlot(i, "SWBF2MaterialSlot", material);

		String textureName;
		if (segments[i].GetMaterial().GetTextureName(0, textureName) && !textureName.IsEmpty())
		{
			FString texName = LIBSTR_TO_TCHAR(textureName);
			const LibSWBF2::Texture* lvlTexture = Container->FindTexture(FSTR_TO_LIBSTR(texName));
			if (lvlTexture == nullptr)
			{
				UE_LOG(LogUF, Warning, TEXT("Could not find global texture: %s"), *texName);
			}
			else
			{
				UTexture2D* ueTexture = ImportTexture(*lvlTexture);
				if (ueTexture != nullptr)
				{
					//material->SetTextureParameterValue(materialSettings.AlbedoParameterName, ueTexture);
				}
			}
		}
		else
		{
			UE_LOG(LogUF, Warning, TEXT("No Textures defined to load in model: %s - %s"), *instanceName, *modelName);
		}
	}

	//TArray<FRuntimeMeshCollisionBox> boxes;
	//TArray<FRuntimeMeshCollisionCapsule> capsules;
	//TArray<FRuntimeMeshCollisionSphere> spheres;
	//LibSWBF2::List<LibSWBF2::CollisionPrimitive> primitives = model.GetCollisionPrimitives();
	//for (size_t i = 0; i < primitives.Size(); ++i)
	//{
	//	switch (primitives[i].GetPrimitiveType())
	//	{
	//		case ECollisionPrimitiveType::Cube:
	//		{
	//			FRuntimeMeshCollisionBox& box = boxes.Emplace_GetRef();
	//			float_t x, y, z;
	//			primitives[i].GetCubeDims(x, y, z);
	//			box.Extents = ToUnreal(LibSWBF2::Vector3{ x * 100.0f, y * 100.0f, z * 100.0f });
	//			box.Center = ToUnreal(primitives[i].GetPosition()) * 100.0f;
	//			box.Rotation = ToUnreal(primitives[i].GetRotation()).Rotator();

	//			if (modelName == "com_item_healthrecharge")
	//			{
	//				UE_LOG(LogUF, Warning, TEXT("Found Cube: %f, %f, %f"), box.Center.X, box.Center.Y, box.Center.Z);
	//				UE_LOG(LogUF, Warning, TEXT("  Extends: %f, %f, %f"), box.Extents.X, box.Extents.Y, box.Extents.Z);
	//			}

	//			break;
	//		}
	//		case ECollisionPrimitiveType::Cylinder:
	//		{
	//			FRuntimeMeshCollisionCapsule& capsule = capsules.Emplace_GetRef();
	//			float_t radius, height;
	//			primitives[i].GetCylinderDims(radius, height);
	//			capsule.Radius = radius * 100.0f;
	//			capsule.Length = height * 100.0f;
	//			capsule.Center = ToUnreal(primitives[i].GetPosition()) * 100.0f;
	//			capsule.Rotation = ToUnreal(primitives[i].GetRotation()).Rotator();

	//			if (modelName == "com_item_healthrecharge")
	//			{
	//				UE_LOG(LogUF, Warning, TEXT("Found Cylinder: %f, %f, %f"), capsule.Center.X, capsule.Center.Y, capsule.Center.Z);
	//				UE_LOG(LogUF, Warning, TEXT("  Radius: %f"), capsule.Radius);
	//				UE_LOG(LogUF, Warning, TEXT("  Length: %f"), capsule.Length);
	//			}

	//			break;
	//		}
	//		case ECollisionPrimitiveType::Sphere:
	//		{
	//			FRuntimeMeshCollisionSphere& sphere = spheres.Emplace_GetRef();
	//			float_t radius;
	//			primitives[i].GetSphereRadius(radius);
	//			sphere.Radius = radius * 100.0f;
	//			sphere.Center = ToUnreal(primitives[i].GetPosition()) * 100.0f;

	//			if (modelName == "com_item_healthrecharge")
	//			{
	//				UE_LOG(LogUF, Warning, TEXT("Found Sphere: %f, %f, %f"), sphere.Center.X, sphere.Center.Y, sphere.Center.Z);
	//				UE_LOG(LogUF, Warning, TEXT("  Radius: %f"), sphere.Radius);
	//			}

	//			break;
	//		}
	//		default:
	//			UE_LOG(LogUF, Warning, TEXT("Unknown primitive collision type: %i"), (int)primitives[i].GetPrimitiveType());
	//			break;
	//	}
	//}

	//// TODO: primitive collisions don't seem to work!?
	//FRuntimeMeshCollisionSettings collSettings;
	//collSettings.Boxes = boxes;
	//collSettings.Capsules = capsules;
	//collSettings.Spheres = spheres;
	//collSettings.CookingMode = ERuntimeMeshCollisionCookingMode::CollisionPerformance;
	//provider->SetCollisionSettings(collSettings);
	//provider->MarkCollisionDirty();

	//const CollisionMesh& collMesh = model.GetCollisionMesh();
	//FRuntimeMeshCollisionVertexStream vertices;
	//{
	//	uint32 count;
	//	LibSWBF2::Vector3* buffer;
	//	collMesh.GetVertexBuffer(count, buffer);
	//	vertices = ConvertVertexBufferRMC(count, buffer, 100.0f);
	//}
	//FRuntimeMeshCollisionTriangleStream triangles;
	//{
	//	uint32 count;
	//	uint32* buffer;
	//	collMesh.GetIndexBuffer(LibSWBF2::ETopology::TriangleList, count, buffer);
	//	triangles = ConvertIndexBufferRMC(count, buffer);
	//}
	//FRuntimeMeshCollisionData collData;
	//collData.Vertices = vertices;
	//collData.Triangles = triangles;

	//if (collData.Vertices.Num() > 0)
	//{
	//	// RMC will warn when trying to provide empty buffers
	//	provider->SetCollisionMesh(collData);
	//	provider->MarkCollisionDirty();
	//}
	//provider->CollisionUpdateCompleted();

	//comp->UpdateBounds();
	ModelCache.Emplace(LIBSTR_TO_TCHAR(model.GetName()), actor);
	return actor;
}

AProcStaticMeshActor* UImporter::ImportTerrain(const LibSWBF2::Terrain& terrain, bool bEnsureImportOnce, const FMaterialImportSettings& materialSettings, ULevel* spawnLevel)
{
//	UWorld* targetWorld = UUnrealFrontGame::GetCurrentWorld();
//	if (targetWorld == nullptr)
//	{
//		UE_LOG(LogUF, Error, TEXT("Given targetWorld is NULL!"));
//		return nullptr;
//	}
//	if (materialSettings.DefaultMaterial == nullptr)
//	{
//		UE_LOG(LogUF, Warning, TEXT("No default Material specified!"));
//		return nullptr;
//	}
//
//	// some layers (worlds) might specify the same terrain again,
//	// so let's make sure to just load that terrain just once
//	FString terrainName = "terrain_";
//	terrainName += LIBSTR_TO_TCHAR(terrain.GetName());
//
//	FActorSpawnParameters params;
//	params.bDeferConstruction = true;
//	params.bNoFail = true;
//	//params.Name = *terrainName;
//	params.OverrideLevel = spawnLevel;
//	params.ObjectFlags = EObjectFlags::RF_Transient;
//	AProcStaticMeshActor* actor = targetWorld->SpawnActor<AProcStaticMeshActor>(params);
//	actor->SetMobility(EComponentMobility::Stationary);
//	actor->SetRuntimeMeshMobility(ERuntimeMeshMobility::Stationary);
//	URuntimeMeshComponent* comp = actor->GetRuntimeMeshComponent();
//
//#ifdef WITH_EDITOR
//	actor->SetActorLabel(terrainName);
//#endif
//
//	//URuntimeMeshProviderStatic* provider = NewObject<URuntimeMeshProviderStatic>();
//	//comp->Initialize(provider);
//
//	TArray<FVector> vertices;
//	TArray<int32> triangles;
//	TArray<FVector> normals;
//	TArray<FVector2D> uv0;
//	TArray<FColor> vertexColors;
//	TArray<FProcMeshTangent> tangents;
//	FRuntimeMeshCollisionVertexStream collVertices;
//	FRuntimeMeshCollisionTriangleStream collTriangles;
//
//	{
//		uint32 count;
//		LibSWBF2::Vector3* buffer;
//		terrain.GetVertexBuffer(count, buffer);
//		vertices = ConvertVertexBuffer(count, buffer, 100.0f);
//		collVertices = ConvertVertexBufferRMC(count, buffer, 100.0f);
//	}
//
//	{
//		uint32 count;
//		uint32* buffer;
//		terrain.GetIndexBuffer(LibSWBF2::ETopology::TriangleList, count, buffer);
//		triangles = ConvertIndexBuffer(count, buffer);
//		collTriangles = ConvertIndexBufferRMC(count, buffer);
//		//triangles = {0, 9, 1, 1, 9, 10, 1, 10, 2, 2, 10, 11};
//	}
//
//	{
//		uint32 count;
//		LibSWBF2::Vector3* buffer;
//		terrain.GetNormalBuffer(count, buffer);
//		normals = ConvertVertexBuffer(count, buffer);
//	}
//
//	{
//		uint32 count;
//		Color* buffer;
//		terrain.GetColorBuffer(count, buffer);
//		vertexColors = ConvertColorBuffer(count, buffer);
//	}
//
//	{
//		uint32 count;
//		LibSWBF2::Vector2* buffer;
//		terrain.GetUVBuffer(count, buffer);
//		uv0 = ConvertUVBuffer(count, buffer);
//	}
//
//	UMaterialInstanceDynamic* material = comp->CreateDynamicMaterialInstance(0, materialSettings.DefaultMaterial);
//	provider->SetupMaterialSlot(0, "SWBF2TerrainMaterial", material);
//	provider->CreateSectionFromComponents(0, 0, 0, vertices, triangles, normals, uv0, vertexColors, tangents, ERuntimeMeshUpdateFrequency::Infrequent, false);
//
//	FRuntimeMeshCollisionData collData;
//	collData.Vertices = collVertices;
//	collData.Triangles = collTriangles;
//	provider->SetCollisionMesh(collData);
//
//	const LibSWBF2::List<LibSWBF2::String>& layerTextures = terrain.GetLayerTextures();
//	for (size_t i = 0; i < layerTextures.Size(); ++i)
//	{
//		// for now, just grab the first texture we get and use it everywhere
//		if (!layerTextures[i].IsEmpty())
//		{
//			const LibSWBF2::Texture* lvlTexture = Container->FindTexture(layerTextures[i]);
//			if (lvlTexture != nullptr)
//			{
//				UTexture2D* texture = ImportTexture(*lvlTexture);
//				if (texture != nullptr)
//				{
//					material->SetTextureParameterValue(materialSettings.AlbedoParameterName, texture);
//				}
//			}
//			break;
//		}
//	}
//
//	comp->UpdateBounds();
//	return actor;
		return nullptr;
}

UTexture2D* UImporter::ImportTexture(const LibSWBF2::Texture& lvlTexture)
{
	{
		UTexture2D** cached = TextureCache.Find(LIBSTR_TO_TCHAR(lvlTexture.GetName()));
		if (cached != nullptr) return *cached;
	}

	uint16 width, height;
	const uint8* data;
	if (!lvlTexture.GetImageData(LibSWBF2::ETextureFormat::B8_G8_R8_A8, 0, width, height, data))
	{
		UE_LOG(LogUF, Error, TEXT("Could not get Image Data!"));
		return nullptr;
	}

	UTexture2D* texture = UTexture2D::CreateTransient(width, height, EPixelFormat::PF_B8G8R8A8);
	//FTexture2DMipMap& mip = texture->PlatformData->Mips[0];
	//void* mipData = mip.BulkData.Lock(LOCK_READ_WRITE);
	//FMemory::Memcpy(mipData, data, width * height * 4);
	//mip.BulkData.Unlock();
	//texture->UpdateResource();
	//TextureCache.Emplace(LIBSTR_TO_TCHAR(lvlTexture.GetName()), texture);
	return texture;
}

//bool UImporter::ImportTo(ULevel* spawnLevel, const FMaterialImportSettings& materialSettings)
//{
//	UWorld* targetWorld = UUnrealFrontGame::GetCurrentWorld();
//	if (targetWorld == nullptr)
//	{
//		UE_LOG(LogUF, Error, TEXT("Given targetWorld is NULL!"));
//		return false;
//	}
//
//	LibSWBF2::Level* worldLevel = Container->TryGetWorldLevel();
//	if (worldLevel == nullptr)
//	{
//		UE_LOG(LogUF, Warning, TEXT("No world LVL found to import!"));
//		return false;
//	}
//
//	LoadedTerrains.Empty();
//	const LibSWBF2::List<LibSWBF2::World>& worlds = worldLevel->GetWorlds();
//	for (size_t i = 0; i < worlds.Size(); ++i)
//	{
//		ImportWorld(worlds[i], materialSettings, spawnLevel);
//	}
//
//	TMap<FString, ALight*> importedLights;
//	const LibSWBF2::List<LibSWBF2::Light> lights = worldLevel->GetLights();
//	for (size_t i = 0; i < lights.Size(); ++i)
//	{
//		importedLights.Add(LIBSTR_TO_TCHAR(lights[i].GetName()), ImportLight(lights[i], spawnLevel));
//	}
//
//	const GlobalLightingConfig* globalLighting = worldLevel->GetGlobalLighting();
//	if (globalLighting != nullptr)
//	{
//		// TODO: better solution than searching...
//		TArray<AActor*> found;
//		UGameplayStatics::GetAllActorsOfClass(targetWorld, ASkyLight::StaticClass(), found);
//		if (found.Num() == 0)
//		{
//			UE_LOG(LogUF, Error, TEXT("Could not find SkyLight Actor!"));
//		}
//		else
//		{
//			if (found.Num() > 1)
//			{
//				UE_LOG(LogUF, Warning, TEXT("Found more than 1 SkyLight Actor: %i! Just use the first one..."), found.Num());
//			}
//
//			ASkyLight* skyLight = Cast<ASkyLight>(found[0]);
//
//			LibSWBF2::Vector3 topColor, bottomColor;
//			FLinearColor color1, color2;
//			if (globalLighting->GetTopColor(topColor))
//			{
//				color1 = ToLinearColor(topColor);
//				//UE_LOG(LogUF, Display, TEXT("Setting Sky Color to: (%f, %f, %f)"), color.R, color.G, color.B);
//				//skyLight->GetLightComponent()->SetLightColor(color);
//			}
//			else
//			{
//				UE_LOG(LogUF, Warning, TEXT("Get global lighting top color failed!"));
//			}
//			if (globalLighting->GetBottomColor(bottomColor))
//			{
//				//UE_LOG(LogUF, Display, TEXT("Setting Sky Lower Hemisphere Color to: (%f, %f, %f)"), bottomColor.m_X, bottomColor.m_Y, bottomColor.m_Z);
//				//skyLight->GetLightComponent()->SetLowerHemisphereColor(ToLinearColor(bottomColor));
//				color2 = ToLinearColor(bottomColor);
//			}
//			else
//			{
//				UE_LOG(LogUF, Warning, TEXT("Get global lighting bottom color failed!"));
//			}
//
//			//FLinearColor finalColor = (color1 + color2) / 2.0f;
//			FLinearColor finalColor = color1 / 255.0f;
//			//skyLight->GetLightComponent()->LightColor = FColor((uint8_t)color1.R, (uint8_t)color1.G, (uint8_t)color1.B);
//			skyLight->GetLightComponent()->SetLightColor(finalColor);
//		}
//
//		// We're currently discarding all directional lights that are not specified as suns!
//		// TODO: proper directional light handling...
//		TSet<FString> allowedDirLights;
//		String sun1, sun2;
//		if (globalLighting->GetLight1(sun1))
//		{
//			allowedDirLights.Add(LIBSTR_TO_TCHAR(sun1));
//		}
//		if (globalLighting->GetLight2(sun2))
//		{
//			allowedDirLights.Add(LIBSTR_TO_TCHAR(sun2));
//		}
//
//		for (const TPair<FString, ALight*> light : importedLights)
//		{
//			ADirectionalLight* dirLight = Cast<ADirectionalLight>(light.Value);
//			if (dirLight != nullptr && allowedDirLights.Find(light.Key) == nullptr)
//			{
//				dirLight->Destroy();
//			}
//		}
//	}
//
//	return true;
//}

bool UImporter::ExecuteLUA(const FString& scriptName)
{
	const LibSWBF2::Script* script = Container->FindScript(FSTR_TO_LIBSTR(scriptName));
	if (script != nullptr)
	{
		return ULuaAPI::ExecuteLUA(*script);
	}

	UE_LOG(LogUF, Warning, TEXT("Could not find script '%s'!"), *scriptName);
	return false;
}

bool UImporter::ExecuteLUA(LibSWBF2::Handle lvlHandle, const FString& scriptName)
{
	LibSWBF2::Level* level = Container->GetLevel(lvlHandle);
	if (level == nullptr)
	{
		UE_LOG(LogUF, Warning, TEXT("Could not find level for handle '%i'!"), lvlHandle);
		return false;
	}

	const LibSWBF2::Script* script = level->GetScript(FSTR_TO_LIBSTR(scriptName));
	if (script != nullptr)
	{
		return ULuaAPI::ExecuteLUA(*script);
	}

	UE_LOG(LogUF, Warning, TEXT("Could not find script '%s'!"), *scriptName);
	return false;
}

//void UImporter::ImportWorld(const World& lvlWorld, const FMaterialImportSettings& materialSettings, ULevel* spawnLevel)
//{
//	const LibSWBF2::List<LibSWBF2::Instance> instances = lvlWorld.GetInstances();
//	for (size_t i = 0; i < instances.Size(); ++i)
//	{
//		FString instanceType = LIBSTR_TO_TCHAR(instances[i].GetType());
//		FString instanceName = LIBSTR_TO_TCHAR(instances[i].GetName());
//
//		FVector position = ToUnreal(instances[i].GetPosition()) * 100.0f;
//		FRotator rotation = ToUnreal(instances[i].GetRotation()).Rotator();
//
//		String geometryName;
//		if (!instances[i].GetProperty("GeometryName", geometryName))
//		{
//			//UE_LOG(LogUF, Warning, TEXT("Instance '%s', '%s' does not have any Geometry!"), *instanceName, *instanceType);
//			continue;
//		}
//
//		const Model* model = Container->FindModel(geometryName);
//		if (model == nullptr)
//		{
//			UE_LOG(LogUF, Warning, TEXT("Could not resolve geometry '%s' for: '%s', '%s'"), LIBSTR_TO_TCHAR(geometryName), *instanceName, *instanceType);
//			continue;
//		}
//
//		FModelImportSettings modelSettings;
//		modelSettings.SpawnPosition = position;
//		modelSettings.SpawnRotation = rotation;
//		AProcStaticMeshActor* actor = ImportModel(*model, modelSettings, materialSettings, spawnLevel, instanceName);
//		if (actor == nullptr)
//		{
//			UE_LOG(LogUF, Warning, TEXT("Importing Model '%s' failed!"), *instanceType);
//		}
//	}
//
//	// NOTE: There are maps without any terrain. In some cases, there might be a terrain name specified, but there's no terrain data.
//	const Terrain* terrain = lvlWorld.GetTerrain();
//	if (terrain != nullptr && !LoadedTerrains.Find(LIBSTR_TO_TCHAR(terrain->GetName())))
//	{
//		ImportTerrain(*terrain, true, materialSettings, spawnLevel);
//		LoadedTerrains.Add(LIBSTR_TO_TCHAR(terrain->GetName()));
//	}
//}

bool UImporter::GetLocalizedString(const FString& language, const FString& localizePath, FString& outLocalized)
{
	uint16* wideChars;
	uint32 numChars;
	if (!Container->GetLocalizedWideString(FSTR_TO_LIBSTR(language), FSTR_TO_LIBSTR(localizePath), wideChars, numChars))
	{
		//UE_LOG(LogUF, Warning, TEXT("Could not find localized '%s' in language '%s' in LVL '%s'!"), *localizePath, *language, *LVLPath);
		outLocalized = localizePath;
		return false;
	}

	outLocalized = WCHAR_TO_TCHAR(wideChars);
	return true;
}

USoundWave* UImporter::ImportSound(const FString& soundName)
{
	LibSWBF2::FNVHash soundHash = UUnrealFrontGame::FNVHash(soundName);
	{
		USoundWave** cached = SoundCache.Find(soundHash);
		if (cached != nullptr) return *cached;
	}

	const LibSWBF2::Sound* sound = Container->FindSound(soundHash);
	if (sound == nullptr)
	{
		UE_LOG(LogUF, Warning, TEXT("Could not find sound '%s'!"), *soundName);
		return nullptr;
	}

	return ImportSound(*sound);
}

UTexture2D* UImporter::ImportTexture(const FString& textureName)
{
	{
		UTexture2D** cached = TextureCache.Find(textureName);
		if (cached != nullptr) return *cached;
	}

	const LibSWBF2::Texture* texture = Container->FindTexture(FSTR_TO_LIBSTR(textureName));
	if (texture == nullptr)
	{
		UE_LOG(LogUF, Warning, TEXT("Could not find texture '%s'!"), *textureName);
		return nullptr;
	}

	return ImportTexture(*texture);
}

USoundWave* UImporter::ImportSound(const LibSWBF2::Sound& sound)
{
	LibSWBF2::FNVHash soundHash = sound.GetHashedName();
	uint32 sampleRate;
	uint32 sampleCount;
	uint8 blockAlign;
	const uint8* data = nullptr;

	if (!sound.GetData(sampleRate, sampleCount, blockAlign, data))
	{
		UE_LOG(LogUF, Error, TEXT("Could not get data of sound '%i'!"), soundHash);
		return nullptr;
	}

	uint64 dataSize = sampleCount * blockAlign;
	USoundWave* wave = NewObject<USoundWave>();

	// Compressed data is now out of date.
	wave->InvalidateCompressedData();
	wave->NumChannels = 1;

	// this is unnecessary? might be the buffer for editor preview...
	//wave->RawData.Lock(EBulkDataLockFlags::LOCK_READ_WRITE);
	//void* dest = wave->RawData.Realloc(dataSize);
	//FMemory::Memcpy(dest, data, dataSize);
	//wave->RawData.Unlock();

	wave->Duration = (float)sampleCount / (float)sampleRate;
	wave->SetSampleRate(sampleRate);
	wave->TotalSamples = sampleCount;

	// we could just point directly do "data", but Unreal will
	// attempt to free that memory and will crash. so let's copy
	wave->RawPCMData = (uint8*)FMemory::Malloc(dataSize);
	FMemory::Memcpy(wave->RawPCMData, data, dataSize);
	wave->RawPCMDataSize = dataSize;

	wave->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
	SoundCache.Emplace(sound.GetHashedName(), wave);
	return wave;
}

//ULoadscreenWidget::ULoadscreenWidget(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//
//}
