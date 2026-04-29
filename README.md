# OpenUSDImporter
OpenUSD importer for my hobby project.

## Prerequirements
- Visual Studio 2026
- CMake 3.24 or newer
- Git
- Python for building OpenUSD

## Dependencies
- OpenUSD v26.05

## Setup
Download and build a minimal OpenUSD C++ runtime:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\setup_openusd.ps1
powershell -ExecutionPolicy Bypass -File .\scripts\setup_openusd.ps1 -BuildVariant debug
```

Configure and build this importer with Visual Studio 2026:

```powershell
cmake --preset vs2026
cmake --preset vs2026-debug
cmake --build --preset vs2026-debug

cmake --preset vs2026-release
cmake --build --preset vs2026-release
```

Run the sample importer:

```powershell
.\build\vs2026\Release\OpenUSDImporter.exe .\assets\HelloWorld.usda
```
