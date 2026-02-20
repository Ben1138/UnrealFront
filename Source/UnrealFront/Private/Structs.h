// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. See LICENSE in the root of this repository or https://www.gnu.org/licenses/
#pragma once
#include "CoreMinimal.h"

#include "Structs.generated.h"

#define TCHAR_TO_LIB(txt) TCHAR_TO_ANSI(txt)
#define LIB_TO_TCHAR(txt) ANSI_TO_TCHAR(txt)
#define LIBSTR_TO_TCHAR(txt) LIB_TO_TCHAR(txt.Buffer())
#define FSTR_TO_LIBSTR(txt) TCHAR_TO_LIB(*txt)


USTRUCT(BlueprintType)
struct FMaterialImportSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		UMaterial* DefaultMaterial;

	UPROPERTY(BlueprintReadWrite)
		FName AlbedoParameterName;
};

USTRUCT(BlueprintType)
struct FModelImportSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		FVector SpawnPosition;

	UPROPERTY(BlueprintReadWrite)
		FRotator SpawnRotation;
};

USTRUCT(BlueprintType)
struct FLuaKeyValuePair
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		FString Key;

	UPROPERTY(BlueprintReadWrite)
		FString Value;
};

USTRUCT(BlueprintType)
struct FLuaMapEra
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		FString Key;

	UPROPERTY(BlueprintReadWrite)
		FString ShowStr;

	UPROPERTY(BlueprintReadWrite)
		FString Subst;

	UPROPERTY(BlueprintReadWrite)
		FString Team1Name;

	UPROPERTY(BlueprintReadWrite)
		FString Team2Name;

	UPROPERTY(BlueprintReadWrite)
		FString Icon1;

	UPROPERTY(BlueprintReadWrite)
		FString Icon2;
};

USTRUCT(BlueprintType)
struct FLuaMapMode
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		FString Key;

	UPROPERTY(BlueprintReadWrite)
		FString ShowStr;

	UPROPERTY(BlueprintReadWrite)
		FString DescStr;

	UPROPERTY(BlueprintReadWrite)
		FString Subst;

	UPROPERTY(BlueprintReadWrite)
		FString Icon;
};

USTRUCT(BlueprintType)
struct FLuaMapEntry
{
	GENERATED_BODY()

	// e.g. TES%s_%s
	UPROPERTY(BlueprintReadWrite)
		FString MapLuaFile;

	UPROPERTY(BlueprintReadWrite)
		TSet<FString> Modes;

	UPROPERTY(BlueprintReadWrite)
		bool bIsModLevel;
};

USTRUCT(BlueprintType)
struct FMapModeEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		FString MapDisplayName;

	UPROPERTY(BlueprintReadWrite)
		FString ModeDisplayName;

	UPROPERTY(BlueprintReadWrite)
		FString EraDisplayName;

	// e.g. TESc_con
	UPROPERTY(BlueprintReadWrite)
		FString MapScript;

	UPROPERTY(BlueprintReadWrite)
		UTexture2D* IconEra;

	UPROPERTY(BlueprintReadWrite)
		UTexture2D* IconMode;
};

USTRUCT(BlueprintType)
struct FMapEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		FString DisplayName;

	UPROPERTY(BlueprintReadWrite)
		TArray<FMapModeEntry> Modes;

	UPROPERTY(BlueprintReadWrite)
		bool bIsModLevel;
};
