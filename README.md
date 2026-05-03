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

Configure and build this importer with Visual Studio 2026:

```powershell
& 'C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe' .\USDImporter.sln /t:Build /p:Configuration=Release /p:Platform=x64
```

Configure the OpenUSD CMake interface target if you need it from another CMake project:

```powershell
cmake --preset vs2026 -S .\usd
```

Run the sample importer:

```powershell
.\bin\x64\Release\USDImporter.exe .\assets\CubeMesh.usda
.\bin\x64\Release\USDImporter.exe .\assets\HelloWorld.usda
```
