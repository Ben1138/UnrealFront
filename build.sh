#!/usr/bin/env bash
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"


# Set this to your unreal engine location
UNREAL_DIR="$HOME/epic/UnrealEngine"


# lua5.0
pushd "$SCRIPT_DIR/ThirdParty/lua5.0/mak.linux"
make lua50_lib
popd

# LibSWBF2
pushd "$SCRIPT_DIR/ThirdParty/LibSWBF2/LibSWBF2"
cmake -S ./ -B ./build
cmake --build ./build --config Debug
popd

# UnrealFront
"$UNREAL_DIR/Engine/Build/BatchFiles/Linux/Build.sh"  -projectfiles -project="$SCRIPT_DIR/UnrealFront.uproject" -game -rocket -progress

