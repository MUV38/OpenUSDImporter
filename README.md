# USDImporter
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

Configure and build the USD wrapper library:

```powershell
Push-Location .\usd
cmake --preset vs2026
cmake --build --preset vs2026-release
Pop-Location
```

Build this importer with Visual Studio 2026:

Open `USDImporter.sln`, select `Release|x64`, and build the solution.

Build the Debug configuration:

```powershell
Push-Location .\usd
cmake --build --preset vs2026-debug
Pop-Location
```

Open `USDImporter.sln`, select `Debug|x64`, and build the solution.

Run the sample importer:

```powershell
.\bin\x64\Release\USDImporter.exe .\assets\CubeMesh.usda
.\bin\x64\Release\USDImporter.exe .\assets\HelloWorld.usda
```
