param(
    [string]$Version = "v26.05",
    [ValidateSet("release", "debug", "relwithdebuginfo")]
    [string]$BuildVariant = "release",
    [string]$RepositoryUrl = "https://github.com/PixarAnimationStudios/OpenUSD.git",
    [string]$PythonExe = "",
    [int]$Jobs = 0,
    [switch]$ForceRebuild
)

$ErrorActionPreference = "Stop"

$RepoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$ExternalDir = Join-Path $RepoRoot "external"
$SourceDir = Join-Path $ExternalDir "OpenUSD-$Version"
$VariantSuffix = if ($BuildVariant -eq "release") { "" } else { "-$BuildVariant" }
$InstallDir = Join-Path $ExternalDir "OpenUSD-$Version$VariantSuffix-install"
$BuildDir = Join-Path $ExternalDir "OpenUSD-$Version$VariantSuffix-build"
$DepsSrcDir = Join-Path $ExternalDir "OpenUSD-$Version-deps-src"
$BundledPython = "C:\Users\MUV08\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe"

if (!(Get-Command cl.exe -ErrorAction SilentlyContinue) -and !$env:OPENUSD_VSDEV_LOADED) {
    $VsDevCmdCandidates = @(
        "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat",
        "C:\Program Files\Microsoft Visual Studio\18\Professional\Common7\Tools\VsDevCmd.bat",
        "C:\Program Files\Microsoft Visual Studio\18\Enterprise\Common7\Tools\VsDevCmd.bat"
    )
    $VsDevCmd = $VsDevCmdCandidates | Where-Object { Test-Path $_ } | Select-Object -First 1
    if ($VsDevCmd) {
        $ForceArg = if ($ForceRebuild) { " -ForceRebuild" } else { "" }
        $Command = "`"$VsDevCmd`" -arch=x64 -host_arch=x64 && set OPENUSD_VSDEV_LOADED=1 && powershell -ExecutionPolicy Bypass -File `"$PSCommandPath`" -Version `"$Version`" -BuildVariant `"$BuildVariant`" -RepositoryUrl `"$RepositoryUrl`" -PythonExe `"$PythonExe`" -Jobs $Jobs$ForceArg"
        cmd /d /c $Command
        exit $LASTEXITCODE
    }
}

if ([string]::IsNullOrWhiteSpace($PythonExe)) {
    if (Test-Path $BundledPython) {
        $PythonExe = $BundledPython
    } else {
        $PythonExe = "python"
    }
}

if ($Jobs -le 0) {
    $Jobs = [Environment]::ProcessorCount
}

New-Item -ItemType Directory -Force -Path $ExternalDir | Out-Null

if (!(Test-Path $SourceDir)) {
    git clone --branch $Version --depth 1 $RepositoryUrl $SourceDir
}

$BuildUsd = Join-Path $SourceDir "build_scripts\build_usd.py"
if (!(Test-Path $BuildUsd)) {
    throw "OpenUSD build script was not found: $BuildUsd"
}

$Args = @(
    $BuildUsd,
    $InstallDir,
    "--src", $DepsSrcDir,
    "--build", $BuildDir,
    "--generator", "Visual Studio 18 2026",
    "--build-variant", $BuildVariant,
    "--build-monolithic",
    "--no-python",
    "--no-imaging",
    "--no-usdview",
    "--no-tools",
    "--no-examples",
    "--no-tutorials",
    "--no-tests",
    "--no-docs",
    "--no-python-docs",
    "--no-materialx",
    "--no-usdValidation",
    "--jobs", "$Jobs"
)

if ($ForceRebuild) {
    $Args += "--force-all"
}

& $PythonExe @Args
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

Write-Host ""
Write-Host "OpenUSD $Version ($BuildVariant) is installed at:"
Write-Host "  $InstallDir"
Write-Host ""
Write-Host "Configure this project with:"
Write-Host "  cmake --preset vs2026-$BuildVariant"
Write-Host "Build it with:"
Write-Host "  cmake --build --preset vs2026-$BuildVariant"
