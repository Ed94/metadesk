# This is meant to be used with build.ps1, and is not a standalone script.
$target_arch = Join-Path $PSScriptRoot 'target_arch.psm1'

import-module $target_arch

if ($IsWindows) {
  # This HandmadeHero implementation is only designed for 64-bit systems
  & $devshell -arch amd64
}

if ( $vendor -eq $null ) {
  write-host "No vendor specified, assuming clang available"
  $vendor = "clang"
}

