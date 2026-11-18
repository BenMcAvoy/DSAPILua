# DSAPILua

DSAPILua is a Windows C++ experiment in exposing native driver and data helpers to Lua. It embeds a Lua interpreter and registers types such as `DriverManager`, `Address`, `FVector`, `FRotator` and `TArray` for scripts to inspect.

## Build

Open `DSAPILua.sln` in Visual Studio and build the x64 configuration. The project uses vcpkg for its C++ dependencies.

## Status

This is a personal reverse-engineering and scripting prototype that expects the companion driver and game-specific data structures. It is not a standalone Lua library or a supported general-purpose tool.
