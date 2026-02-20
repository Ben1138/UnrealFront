// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. See LICENSE in the root of this repository or https://www.gnu.org/licenses/
#include "LuaAPI.h"
#include "UnrealFront/UnrealFront.h"
#include "UnrealFrontGame.h"


static lua_State* L = nullptr;
static int StackSize = 0;

void ULuaAPI::OpenEnvironment()
{
	if (L != nullptr)
	{
		UE_LOG(LogLua, Warning, TEXT("Lua environment already opened!"));
		return;
	}

	L = lua_open();
	StackSize = lua_gettop(L);
	luaopen_base(L);
	StackSize = lua_gettop(L);
	luaopen_math(L);
	StackSize = lua_gettop(L);
	luaopen_string(L);
	StackSize = lua_gettop(L);
	luaopen_table(L);
	StackSize = lua_gettop(L);

	lua_register(L, "ScriptCB_GetPlatform", ScriptCB_GetPlatform);
	lua_register(L, "ScriptCB_GetOnlineService", ScriptCB_GetOnlineService);
	lua_register(L, "ScriptCB_GetLanguage", ScriptCB_GetLanguage);
	lua_register(L, "ScriptCB_GetCONMaxTimeLimit", ScriptCB_GetCONMaxTimeLimit);
	lua_register(L, "ScriptCB_GetCONNumBots", ScriptCB_GetCONNumBots);
	lua_register(L, "ScriptCB_SetNumBots", ScriptCB_SetNumBots);
	lua_register(L, "ScriptCB_IsMissionSetupSaved", ScriptCB_IsMissionSetupSaved);
	lua_register(L, "ScriptCB_LoadMissionSetup", ScriptCB_LoadMissionSetup);
	lua_register(L, "ScriptCB_getlocalizestr", ScriptCB_getlocalizestr);
	lua_register(L, "ScriptCB_tounicode", ScriptCB_tounicode);
	lua_register(L, "ScriptCB_ununicode", ScriptCB_ununicode);
	lua_register(L, "ScriptCB_usprintf", ScriptCB_usprintf);
	lua_register(L, "ScriptCB_DoFile", ScriptCB_DoFile);

	lua_register(L, "SetPS2ModelMemory", SetPS2ModelMemory);
	lua_register(L, "StealArtistHeap", StealArtistHeap);
	lua_register(L, "SetTeamAggressiveness", SetTeamAggressiveness);
	lua_register(L, "SetMemoryPoolSize", SetMemoryPoolSize);
	lua_register(L, "ClearWalkers", ClearWalkers);
	lua_register(L, "AddWalkerType", AddWalkerType);
	lua_register(L, "SetSpawnDelay", SetSpawnDelay);
	lua_register(L, "SetHeroClass", SetHeroClass);
	lua_register(L, "SetTeamAsEnemy", SetTeamAsEnemy);
	lua_register(L, "SetTeamAsFriend", SetTeamAsFriend);
	lua_register(L, "SetTeamName", SetTeamName);
	lua_register(L, "SetUnitCount", SetUnitCount);
	lua_register(L, "AddUnitClass", AddUnitClass);
	lua_register(L, "SetDenseEnvironment", SetDenseEnvironment);
	lua_register(L, "SetMinFlyHeight", SetMinFlyHeight);
	lua_register(L, "SetMaxFlyHeight", SetMaxFlyHeight);
	lua_register(L, "SetMaxPlayerFlyHeight", SetMaxPlayerFlyHeight);
	lua_register(L, "SetAttackingTeam", SetAttackingTeam);
	lua_register(L, "AddCameraShot", AddCameraShot);
	lua_register(L, "SetTeamIcon", SetTeamIcon);
	lua_register(L, "SetBleedRate", SetBleedRate);
	lua_register(L, "GetReinforcementCount", GetReinforcementCount);
	lua_register(L, "SetReinforcementCount", SetReinforcementCount);
	lua_register(L, "AddReinforcements", AddReinforcements);
	lua_register(L, "SetMapNorthAngle", SetMapNorthAngle);
	lua_register(L, "AISnipeSuitabilityDist", AISnipeSuitabilityDist);
	lua_register(L, "EnableSPHeroRules", EnableSPHeroRules);
	lua_register(L, "AddDeathRegion", AddDeathRegion);
	lua_register(L, "SetProperty", SetProperty);
	lua_register(L, "DisableBarriers", DisableBarriers);
	lua_register(L, "PlayAnimation", PlayAnimation);
	lua_register(L, "SetUberMode", SetUberMode);
	lua_register(L, "GetCommandPostBleedValue", GetCommandPostBleedValue);

	lua_register(L, "OpenAudioStream", OpenAudioStream);
	lua_register(L, "AudioStreamAppendSegments", AudioStreamAppendSegments);
	lua_register(L, "SetBleedingVoiceOver", SetBleedingVoiceOver);
	lua_register(L, "SetLowReinforcementsVoiceOver", SetLowReinforcementsVoiceOver);
	lua_register(L, "SetOutOfBoundsVoiceOver", SetOutOfBoundsVoiceOver);
	lua_register(L, "SetAmbientMusic", SetAmbientMusic);
	lua_register(L, "SetVictoryMusic", SetVictoryMusic);
	lua_register(L, "SetDefeatMusic", SetDefeatMusic);
	lua_register(L, "SetSoundEffect", SetSoundEffect);

	lua_register(L, "ReadDataFile", ReadDataFile);
	lua_register(L, "AddDownloadableContent", AddDownloadableContent);

	StackSize = lua_gettop(L);
}

void ULuaAPI::CloseEnvironment()
{
	if (L == nullptr)
	{
		// do not log in destructors
		//UE_LOG(LogLua, Warning, TEXT("No Lua environment opened to close!"));
		return;
	}

	lua_close(L);
	L = nullptr;
}

bool ULuaAPI::CheckStatus(const FString& scriptName, int status)
{
	if (status != 0)
	{
		FString errorCode;
		switch (status)
		{
			case LUA_ERRRUN:
				errorCode = "RUN";
				break;
			case LUA_ERRFILE:
				errorCode = "FILE";
				break;
			case LUA_ERRSYNTAX:
				errorCode = "SYNTAX";
				break;
			case LUA_ERRMEM:
				errorCode = "MEMORY";
				break;
			case LUA_ERRERR:
				errorCode = "FATAL";
				break;
			default:
				errorCode = "UNKNOWN";
				break;
		}

		int num = lua_gettop(L);
		if (num > 0)
		{
			const char* msg = lua_tostring(L, -1);
			UE_LOG(LogLua, Error, TEXT("[LUA] %s ERROR (%s): %s"), *errorCode, *scriptName, ANSI_TO_TCHAR(msg));
		}
		else
		{
			UE_LOG(LogLua, Error, TEXT("[LUA] %s ERROR in: %s"), *errorCode, *scriptName);
		}

		//CloseEnvironment();
		return false;
	}

	UE_LOG(LogLua, Display, TEXT("[LUA] (%s): Execution successful!"), *scriptName);
	return true;
}

bool ULuaAPI::ExecuteLUA(const LibSWBF2::Script& luaScript)
{
	if (L == nullptr)
	{
		UE_LOG(LogLua, Warning, TEXT("No Lua environment opened!"));
		return false;
	}

	FString scriptName = ANSI_TO_TCHAR(luaScript.GetName().Buffer());
	const uint8_t* scriptData = nullptr;
	size_t scriptSize = 0;

	if (!luaScript.GetData(scriptData, scriptSize))
	{
		UE_LOG(LogLua, Error, TEXT("Could not grab script data of script %s!"), *scriptName);
		return false;
	}

	int result = luaL_loadbuffer(L, (const char*)scriptData, scriptSize, "testChunkDoBuffer");
	if (result == 0)
	{
		result = lua_pcall(L, 0, LUA_MULTRET, 0);
	}

	return CheckStatus(scriptName, result);
}

bool ULuaAPI::ExecuteLUAFile(FString luaFilePath)
{
	if (L == nullptr)
	{
		UE_LOG(LogLua, Warning, TEXT("No Lua environment opened!"));
		return false;
	}

	int result = luaL_loadfile(L, TCHAR_TO_ANSI(*luaFilePath));
	if (result == 0)
	{
		result = lua_pcall(L, 0, LUA_MULTRET, 0);
	}

	return CheckStatus(FPaths::GetPathLeaf(luaFilePath), result);
}

int ULuaAPI::ScriptCB_GetPlatform(lua_State* lua)
{
	lua_pushstring(lua, "PC");
	return 1;
}

int ULuaAPI::ScriptCB_GetOnlineService(lua_State* lua)
{
	lua_pushstring(lua, "GameSpy");
	return 1;
}

int ULuaAPI::ScriptCB_GetLanguage(lua_State* lua)
{
	lua_pushstring(lua, "english");
	lua_pushnumber(lua, 0); // TODO: verfify
	return 2;
}

int ULuaAPI::ScriptCB_GetCONMaxTimeLimit(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::ScriptCB_GetCONNumBots(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::ScriptCB_SetNumBots(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::ScriptCB_IsMissionSetupSaved(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::ScriptCB_LoadMissionSetup(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::ScriptCB_getlocalizestr(lua_State* lua)
{
	FString localizePath = lua_tostring(lua, 1);
	UUnrealFrontGame::GetLocalizedString(localizePath);
	lua_pushstring(L, (const char*)TCHAR_TO_WCHAR(*localizePath));
	return 0;
}

int ULuaAPI::ScriptCB_tounicode(lua_State* lua)
{
	FString ansiString = lua_tostring(lua, 1);
	lua_pushstring(L, (const char*)TCHAR_TO_WCHAR(*ansiString));
	return 0;
}

int ULuaAPI::ScriptCB_ununicode(lua_State* lua)
{
	FString ansiString = WCHAR_TO_TCHAR(lua_tostring(lua, 1));
	lua_pushstring(L, (const char*)TCHAR_TO_ANSI(*ansiString));
	return 0;
}

// TODO: Also handle %i, %f, ...
// Maybe just use libc's 'vsprintf'?
int ULuaAPI::ScriptCB_usprintf(lua_State* lua)
{
	StackSize = lua_gettop(L);
	FString formatString = lua_tostring(lua, 1);
	TArray<FString> params;
	for (int i = 2; i <= StackSize; ++i)
	{
		params.Add(lua_tostring(lua, i));
	}

	// FString::Printf only takes string literals... duh!
	// so let's simply replace %s occourences ourselfs
	FString result;
	int32 pos = 0;
	for (
		int32 foundIdx = -1, i = 0;
		(foundIdx = formatString.Find("%s", ESearchCase::IgnoreCase, ESearchDir::FromStart, pos) >= 0 && i < params.Num());
		i++)
	{
		result = formatString.Left(foundIdx);
		result += params[i];
		formatString.RemoveAt(0, foundIdx + 2);
	}

	lua_pushstring(L, (const char*)TCHAR_TO_WCHAR(*result));
	return 0;
}

int ULuaAPI::ScriptCB_DoFile(lua_State* lua)
{
	FString scriptName = lua_tostring(lua, 1);
	UE_LOG(LogLua, Display, TEXT("Import script: %s"), *scriptName);
	UUnrealFrontGame::ExecuteLUA(scriptName);
	return 0;
}

int ULuaAPI::SetPS2ModelMemory(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::StealArtistHeap(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetTeamAggressiveness(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetMemoryPoolSize(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::ClearWalkers(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::AddWalkerType(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetSpawnDelay(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetHeroClass(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetTeamAsEnemy(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetTeamAsFriend(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetTeamName(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetUnitCount(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::AddUnitClass(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetDenseEnvironment(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetMinFlyHeight(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetMaxFlyHeight(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetMaxPlayerFlyHeight(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetAttackingTeam(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::AddCameraShot(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetTeamIcon(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetBleedRate(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::GetReinforcementCount(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetReinforcementCount(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::AddReinforcements(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::OpenAudioStream(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::AudioStreamAppendSegments(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetBleedingVoiceOver(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetLowReinforcementsVoiceOver(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetOutOfBoundsVoiceOver(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetAmbientMusic(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetVictoryMusic(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetDefeatMusic(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetSoundEffect(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetMapNorthAngle(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::AISnipeSuitabilityDist(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::EnableSPHeroRules(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::AddDeathRegion(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetProperty(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::DisableBarriers(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::PlayAnimation(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::SetUberMode(lua_State* lua)
{
	StackSize = lua_gettop(L);
	return 0;
}

int ULuaAPI::GetCommandPostBleedValue(lua_State* lua)
{
	StackSize = lua_gettop(L);

	// LUA return value
	//lua_pushnumber(L, 0.0f);
	// TODO: not working?

	return 0;
}

int ULuaAPI::ReadDataFile(lua_State* lua)
{
	StackSize = lua_gettop(L);

	FString path = lua_tostring(L, 1);
	TSet<FString> args;

	UE_LOG(LogLua, Display, TEXT("Called ReadDataFile with %d arguments, path '%s'"), StackSize, *path);
	//FString log = FString::Printf(TEXT("Called ReadDataFile with %d arguments:\n\t%s\n"), StackSize, *path);
	for (int i = 2; i <= StackSize; ++i)
	{
		FString arg = lua_tostring(L, i);
		args.Add(arg);
		//log += "\t" + arg + "\n";
	}

	UUnrealFrontGame::AddLVL(path, args.Num() == 0 ? nullptr : &args);
	//UE_LOG(LogLua, Display, TEXT("%s"), *log);

	return 0;
}

int ULuaAPI::AddDownloadableContent(lua_State* lua)
{
	FString		threeLetterName		= lua_tostring(L, 1);
	FString		scriptName			= lua_tostring(L, 2);
	lua_Number	levelMemoryModifier	= lua_tonumber(L, 3);

	UUnrealFrontGame::AddDownloadableContent(threeLetterName, scriptName);

	return 0;
}

FString ULuaAPI::GetTableKeyValueAsString(const FString& key)
{
	lua_pushstring(L, TCHAR_TO_ANSI(*key));
	lua_gettable(L, -2);
	FString result = lua_tostring(L, -1);
	lua_pop(L, 1);
	return result;
}

bool ULuaAPI::GetTableKeyValueAsBool(const FString& key)
{
	lua_pushstring(L, TCHAR_TO_ANSI(*key));
	lua_gettable(L, -2);
	bool result = (bool)lua_toboolean(L, -1);
	lua_pop(L, 1);
	return result;
}

TArray<FLuaKeyValuePair> ULuaAPI::IterateTable(lua_State* lua, int index)
{
	TArray<FLuaKeyValuePair> table;

	// Push another reference to the table on top of the stack (so we know
	// where it is, and this function can work for negative, positive and
	// pseudo indices
	lua_pushvalue(lua, index);
	// stack now contains: -1 => table
	lua_pushnil(lua);
	// stack now contains: -1 => nil; -2 => table
	while (lua_next(lua, -2))
	{
		// stack now contains: -1 => value; -2 => key; -3 => table
		// copy the key so that lua_tostring does not modify the original
		lua_pushvalue(lua, -2);
		// stack now contains: -1 => key; -2 => value; -3 => key; -4 => table
		FLuaKeyValuePair& pair = table.Emplace_GetRef();
		pair.Key = ANSI_TO_TCHAR(lua_tostring(lua, -1));
		pair.Value = ANSI_TO_TCHAR(lua_tostring(lua, -2));
		// pop value + copy of key, leaving original key
		lua_pop(lua, 2);
		// stack now contains: -1 => key; -2 => table
	}
	// stack now contains: -1 => table (when lua_next returns 0 it pops the key
	// but does not push anything.)
	// Pop table
	lua_pop(lua, 1);
	// Stack is now the same as it was on entry to this function
	return table;
}

TArray<FLuaMapEra> ULuaAPI::GetEras()
{
	TArray<FLuaMapEra> eras;

	int lol = lua_gettop(L);
	lua_getglobal(L, "gMapEras");
	if (lua_istable(L, -1))
	{
		int num = luaL_getn(L, -1);
		for (int i = 1; i <= num; ++i)
		{
			lua_pushnumber(L, i);
			lua_gettable(L, -2);
			if (lua_istable(L, -1))
			{
				FLuaMapEra& era = eras.Emplace_GetRef();

				era.Key = GetTableKeyValueAsString("key");
				era.ShowStr = GetTableKeyValueAsString("showstr");
				era.Subst = GetTableKeyValueAsString("subst");
				era.Team1Name = GetTableKeyValueAsString("Team1Name");
				era.Team2Name = GetTableKeyValueAsString("Team2Name");
				era.Icon1 = GetTableKeyValueAsString("icon1");
				era.Icon2 = GetTableKeyValueAsString("icon2");
			}
			else
			{
				UE_LOG(LogLua, Warning, TEXT("Expected nested tables in 'gMapEras'!"));
			}

			lua_pop(L, 1);
		}

		lua_pop(L, 1);
	}
	else
	{
		UE_LOG(LogLua, Warning, TEXT("Could not find global table 'gMapEras'!"));
	}
	lol = lua_gettop(L);
	return eras;
}

TArray<FLuaMapMode> ULuaAPI::GetModes()
{
	TArray<FLuaMapMode> modes;

	int lol = lua_gettop(L);
	lua_getglobal(L, "gMapModes");
	if (lua_istable(L, -1))
	{
		int num = luaL_getn(L, -1);
		for (int i = 1; i <= num; ++i)
		{
			lua_pushnumber(L, i);
			lua_gettable(L, -2);
			if (lua_istable(L, -1))
			{
				FLuaMapMode& mode = modes.Emplace_GetRef();
				
				mode.Key = GetTableKeyValueAsString("key");
				mode.ShowStr = GetTableKeyValueAsString("showstr");
				mode.DescStr = GetTableKeyValueAsString("descstr");
				mode.Subst = GetTableKeyValueAsString("subst");
				mode.Icon = GetTableKeyValueAsString("icon");
			}
			else
			{
				UE_LOG(LogLua, Warning, TEXT("Expected nested tables in 'gMapModes'!"));
			}

			lua_pop(L, 1);
		}

		lua_pop(L, 1);
	}
	else
	{
		UE_LOG(LogLua, Warning, TEXT("Could not find global table 'gMapModes'!"));
	}
	lol = lua_gettop(L);
	return modes;
}

TArray<FLuaMapEntry> ULuaAPI::GetMapList(const FString& mapTableName)
{
	TArray<FLuaMapEntry> list;

	int lol = lua_gettop(L);
	lua_getglobal(L, TCHAR_TO_ANSI(*mapTableName));
	if (lua_istable(L, -1))
	{
		int num = luaL_getn(L, -1);
		for (int i = 1; i <= num; ++i)
		{
			FLuaMapEntry entry;

			lua_pushnumber(L, i);
			lua_gettable(L, -2);
			if (lua_istable(L, -1))
			{
				TArray<FLuaKeyValuePair> table = IterateTable(L, -1);

				// first iteration: get all eras
				TSet<FString> eras;
				for (FLuaKeyValuePair& pair : table)
				{
					if (pair.Key.StartsWith("era_") && pair.Value == "1")
					{
						eras.Add(pair.Key.RightChop(4));
					}
				}

				// second iteration: get all modes and filter by eras
				for (FLuaKeyValuePair& pair : table)
				{
					FString era = pair.Key.Right(1);
					if (pair.Key.StartsWith("mode_") && pair.Value == "1" && eras.Contains(era))
					{
						entry.Modes.Add(pair.Key);
					}
				}
				entry.MapLuaFile = GetTableKeyValueAsString("mapluafile");
				entry.bIsModLevel = GetTableKeyValueAsBool("isModLevel");

				list.Add(entry);
			}
			else
			{
				UE_LOG(LogLua, Warning, TEXT("Expected nested tables in '%s'!"), *mapTableName);
			}

			lua_pop(L, 1);
		}

		lua_pop(L, 1);
	}
	else
	{
		UE_LOG(LogLua, Warning, TEXT("Could not find global table '%s'!"), *mapTableName);
	}
	lol = lua_gettop(L);
	return list;
}

TArray<FLuaMapEntry> ULuaAPI::GetSPMapList()
{
	return GetMapList("sp_missionselect_listbox_contents");
}

TArray<FLuaMapEntry> ULuaAPI::GetMPMapList()
{
	return GetMapList("mp_missionselect_listbox_contents");
}

void ULuaAPI::Call_ScriptInit()
{
	lua_getglobal(L, "ScriptInit");
	if (!lua_isfunction(L, -1))
	{
		UE_LOG(LogLua, Error, TEXT("Could not find global function 'ScriptInit()'!"));
		return;
	}

	UE_LOG(LogLua, Display, TEXT("Call global ScriptInit()"));
	int result = lua_pcall(L, 0, 0, 0);
	if (!CheckStatus("", result))
	{
		lua_pop(L, 1);
	}
	lua_pop(L, 1);
}

void ULuaAPI::Call_ScriptPostLoad()
{
	lua_getglobal(L, "ScriptPostLoad");
	if (!lua_isfunction(L, -1))
	{
		UE_LOG(LogLua, Error, TEXT("Could not find global function 'ScriptPostLoad()'!"));
		return;
	}

	UE_LOG(LogLua, Display, TEXT("Call global ScriptPostLoad()"));
	int result = lua_pcall(L, 0, 0, 0);
	if (!CheckStatus("", result))
	{
		lua_pop(L, 1);
	}
	lua_pop(L, 1);
}

//void ULuaAPI::Call_missionlist_ExpandMaplist(bool bForMP)
//{
//	lua_getglobal(L, "missionlist_ExpandMaplist");
//	if (!lua_isfunction(L, -1))
//	{
//		UE_LOG(LogLua, Error, TEXT("Could not find global function 'missionlist_ExpandMaplist()'!"));
//		return;
//	}
//
//	lua_pushboolean(L, bForMP);
//	int result = lua_pcall(L, 1, 0, 0);
//	if (!CheckStatus("", result))
//	{
//		lua_pop(L, 1);
//	}
//	lua_pop(L, 1);
//}
//
//void ULuaAPI::PrintExpandedMapList()
//{
//	Call_missionlist_ExpandMaplist(false);
//
//	TArray<FMapEntry> array = GetMapList("missionselect_listbox_contents");
//}

