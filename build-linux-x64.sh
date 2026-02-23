#!/usr/bin/env bash
SCRIPT_DIR="$(cd "$(dirname "$BASH_SOURCE[0]")" && pwd)"
echo "Project Location: $SCRIPT_DIR"

# ------------------------------------------
# Set this to your unreal engine location
UNREAL_DIR="$HOME/epic/UnrealEngine"
# ------------------------------------------


# Build lua5.0
pushd "$SCRIPT_DIR/ThirdParty/lua5.0"
#rm -rf ./build
cmake -S ./ -B ./build
cmake --build ./build --config Debug
popd

# Build LibSWBF2
pushd "$SCRIPT_DIR/ThirdParty/LibSWBF2"
#rm -rf ./build
cmake -S ./ -B ./build
cmake --build ./build --config Debug
popd


# Find embedded 'dotnet' binary
pushd "$UNREAL_DIR/Engine/Binaries/ThirdParty/DotNet"

# Create an array from the list of subdirectories
subdirs=($(find . -maxdepth 1 -type d ! -name '.'))

# Check if the array is not empty
if [ ${#subdirs[@]} -eq 0 ]; then
		echo "ERROR: Could not find embedded 'dotnet' binary in $UNREAL_DIR!"
		exit 1
fi

# Access the first entry (index 0) and remove './' prefix
DOT_NET_VERSION=$(echo ${subdirs[0]} | cut -c 3-)
echo "Unreal .NET Version: $DOT_NET_VERSION"

DOT_NET="$UNREAL_DIR/Engine/Binaries/ThirdParty/DotNet/$DOT_NET_VERSION/linux-x64/dotnet"
popd

# Build UnrealFront
#"$UNREAL_DIR/Engine/Build/BatchFiles/Linux/Build.sh" -projectfiles -project="$SCRIPT_DIR/UnrealFront.uproject" -game -rocket -progress
#"$UNREAL_DIR/Engine/Build/BatchFiles/Linux/Build.sh" UnrealFrontEditor Linux Development "$SCRIPT_DIR/UnrealFront.uproject"
"$DOT_NET" "$UNREAL_DIR/Engine/Binaries/DotNET/UnrealBuildTool/UnrealBuildTool.dll" \
	-ModuleWithSuffix=UnrealFront,4269 \
	UnrealFrontEditor \
	Linux \
	DebugGame \
	-Project="$SCRIPT_DIR/UnrealFront.uproject" \
	"$SCRIPT_DIR/UnrealFront.uproject" \
	-IgnoreJunk \
	-progress


#"$UNREAL_DIR/Engine/Build/BatchFiles/RunUAT.sh" BuildPlugin -plugin="$SCRIPT_DIR/Plugins/RealtimeMeshComponent/RealtimeMeshComponent.uplugin" -package="$SCRIPT_DIR/Plugins/RealtimeMeshComponent/PACKAGED"
