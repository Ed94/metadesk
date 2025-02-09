# TODO(Ed): Need to eventually utilize the original build scripts
# For now just using something I'm used to for getting the library working..
clear-host

$misc = join-path $PSScriptRoot 'helpers/misc.ps1'
. $misc

$path_root = Get-ScriptRepoRoot
Push-Location $path_root

$path_bin         = join-path $path_root bin
$path_scripts     = $path_bin

$devshell           = Join-Path $PSScriptRoot 'helpers/devshell.ps1'
$incremental_checks = Join-Path $PSScriptRoot 'helpers/incremental_checks.ps1'
$ini                = join-path $PSScriptRoot 'helpers/ini.ps1'
$vendor_toolchain   = Join-Path $PSScriptRoot 'helpers/vendor_toolchain.ps1'

. $ini
. $incremental_checks

$path_system_details = join-path $path_scripts 'system_details.ini'
if ( test-path $path_system_details ) {
    $iniContent = Get-IniContent $path_system_details
    $CoreCount_Physical = $iniContent["CPU"]["PhysicalCores"]
    $CoreCount_Logical  = $iniContent["CPU"]["LogicalCores"]
}
elseif ( $IsWindows ) {
	$CPU_Info = Get-CimInstance –ClassName Win32_Processor | Select-Object -Property NumberOfCores, NumberOfLogicalProcessors
	$CoreCount_Physical, $CoreCount_Logical = $CPU_Info.NumberOfCores, $CPU_Info.NumberOfLogicalProcessors

	new-item -path $path_system_details -ItemType File
    "[CPU]"                             | Out-File $path_system_details
    "PhysicalCores=$CoreCount_Physical" | Out-File $path_system_details -Append
    "LogicalCores=$CoreCount_Logical"   | Out-File $path_system_details -Append
}
write-host "Core Count - Physical: $CoreCount_Physical Logical: $CoreCount_Logical"

#region Arguments
$vendor       = $null
$release      = $null
[bool] $verbose      = $false
[bool] $code_sanity  = $false

[array] $vendors = @( "clang", "msvc" )

# This is a really lazy way of parsing the args, could use actual params down the line...

if ( $args ) { $args | ForEach-Object {
	switch ($_) {
		{ $_ -in $vendors } { $vendor       = $_; break }
		"verbose"			{ $verbose      = $true }
		"release"           { $release      = $true }
		"debug"             { $release      = $false }
		"code_sanity"       { $code_sanity  = $true }
		}
	}
}
#endregion Arguments

if ($IsWindows) {
	# This HandmadeHero implementation is only designed for 64-bit systems
	& $devshell -arch amd64
}

if ( $vendor -eq $null ) {
	write-host "No vendor specified, assuming clang available"
	$vendor = "clang"
}

if ( $release -eq $null ) {
	write-host "No build type specified, assuming debug"
	$release = $false
}
elseif ( $release -eq $false ) {
	$debug = $true
}
else {
	$optimize = $true
}

$cannot_build = $code_snaity -eq $false
if ( $cannot_build ) {
	throw "No build target specified. One must be specified, this script will not assume one"
}

. $vendor_toolchain

write-host "Build Type: $(if ($release) {"Release"} else {"Debug"} )"

$path_build       = join-path $path_root build
$path_code        = join-path $path_root code
$path_examples    = join-path $path_root examples
$path_gen_c11     = join-path $path_root gen_c11
$path_gen_cpp17   = join-path $path_root gen_cpp17
$path_tests       = join-path $path_root tests
$path_third_party = join-path $path_root third_party

verify-path $path_build

if ($code_sanity)
{
	write-host "Building code/metadesk.c (sanity compile) with $vendor"

	$compiler_args = @()
	$compiler_args += $flag_all_c
	$compiler_args += $flag_updated_cpp_macro
	$compiler_args += $flag_c11

	$linker_args = @()
	$linker_args += $flag_link_win_subsystem_console

	$path_base = join-path $path_code base

	$includes   = @( $path_base )
	$unit       = join-path $path_code  'metadesk.c'
	$executable = join-path $path_build 'metadesk.lib'

	$result = build-simple $path_build $includes $compiler_args $linker_args $unit $executable
}

Pop-Location # $path_root
