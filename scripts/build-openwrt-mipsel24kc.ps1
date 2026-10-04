param(
    [Parameter(Mandatory=$true)]
    [string]$OpenWrtSdk
)

$ErrorActionPreference = "Stop"
$repo = Split-Path -Parent $PSScriptRoot
$pkgSrc = Join-Path $repo "package\nva-udp-core"
$pkgDst = Join-Path $OpenWrtSdk "package\nva-udp-core"

if (!(Test-Path $OpenWrtSdk)) { throw "OpenWrt SDK path not found: $OpenWrtSdk" }
if (Test-Path $pkgDst) { Remove-Item -LiteralPath $pkgDst -Recurse -Force }
New-Item -ItemType Directory -Force -Path (Split-Path -Parent $pkgDst) | Out-Null
Copy-Item -LiteralPath $pkgSrc -Destination $pkgDst -Recurse

Push-Location $OpenWrtSdk
try {
    & make package/nva-udp-core/compile V=s
} finally {
    Pop-Location
}
