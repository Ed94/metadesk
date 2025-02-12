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
[bool] $verbose         = $false
[bool] $code_sanity     = $false
[bool] $compile_sanity  = $false
[bool] $gen_c11         = $false
[bool] $tests           = $false
[bool] $c11_sanity      = $false
[bool] $sanity_tests    = $false

[array] $vendors = @( "clang", "msvc" )

# This is a really lazy way of parsing the args, could use actual params down the line...

if ( $args ) { $args | ForEach-Object {
	switch ($_) {
		{ $_ -in $vendors } { $vendor         = $_; break }
		"verbose"			{ $verbose        = $true }
		"release"           { $release        = $true }
		"debug"             { $release        = $false }
		"code_sanity"       { $code_sanity    = $true }
		"compile_sanity"    { $compile_sanity = $true }
		"gen_c11"           { $gen_c11        = $true }
		"tests"             { $tests          = $true }
		"c11_sanity"        { $c11_sanity     = $true }
		"sanity_tests"      { $sanity_tests   = $true }
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

$cannot_build =                    $code_sanity    -eq $false
$cannot_build = $cannot_build -and $compile_sanity -eq $false
$cannot_build = $cannot_build -and $gen_c11        -eq $false
$cannot_build = $cannot_build -and $test           -eq $false
$cannot_build = $cannot_build -and $c11_sanity     -eq $false
$cannot_build = $cannot_build -and $sanity_tests   -eq $false
if ( $cannot_build ) {
	Pop-Location
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

if ($compile_sanity)
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

if ($code_sanity)
{
	write-host "Building test/code_sanity.c"

	$compiler_args = @()
	$compiler_args += $flag_all_c
	$compiler_args += $flag_updated_cpp_macro
	$compiler_args += $flag_c11

	$linker_args = @()
	$linker_args += $flag_link_win_subsystem_console

	$includes   = @( $path_code, $path_root )
	$unit       = join-path $path_tests 'code_sanity.c'
	$executable = join-path $path_build 'code_sanity.exe'

	$result = build-simple $path_build $includes $compiler_args $linker_args $unit $executable

	Push-Location $path_build
		if ( Test-Path( $executable ) ) {
			write-host "`nRunning test/code_sanity"
			$time_taken = Measure-Command { & $executable
					| ForEach-Object {
						write-host `t $_ -ForegroundColor Green
					}
				}
			write-host "`ntest/code_sanity completed in $($time_taken.TotalMilliseconds) ms"
		}
	Pop-Location
}

if ($gen_c11)
{
	write-host "Building gen_c11/gen_c11.c"

	$compiler_args = @()
	$compiler_args += $flag_all_c
	$compiler_args += $flag_updated_cpp_macro
	$compiler_args += $flag_c11

	$linker_args = @()
	$linker_args += $flag_link_win_subsystem_console

	$includes   = @( $path_gen_c11, $path_root )
	$unit       = join-path $path_gen_c11 'gen_c11.c'
	$executable = join-path $path_build   'gen_c11.exe'

	$result = build-simple $path_build $includes $compiler_args $linker_args $unit $executable

	$path_gen = join-path $path_gen_c11 'gen'
	verify-path $path_gen

	Push-Location $path_root
		if ( Test-Path( $executable ) ) {
			write-host "`nRunning gen_c11/gen_c11.exe"
			$time_taken = Measure-Command { & $executable
					| ForEach-Object {
						write-host `t $_ -ForegroundColor Green
					}
				}
			write-host "`ntest/code_sanity completed in $($time_taken.TotalMilliseconds) ms"
		}
	Pop-Location
}

if ($c11_sanity)
{
	write-host " Building tests/c11_sanity.c"

	$compiler_args = @()
	$compiler_args += $flag_all_c
	$compiler_args += $flag_updated_cpp_macro
	$compiler_args += $flag_c11

	$linker_args = @()
	$linker_args += $flag_link_win_subsystem_console

	$path_gen = join-path $path_gen_c11 'gen'

	$includes   = @( $path_gen, $path_root )
	$unit       = join-path $path_tests  'c11_sanity.c'
	$executable = join-path $path_build  'c11_sanity.exe'

	$result = build-simple $path_build $includes $compiler_args $linker_args $unit $executable

	Push-Location $path_root
		if ( Test-Path( $executable ) ) {
			write-host "`nRunning tests/c11_sanity_segregated.exe (segregated)"
			$time_taken = Measure-Command { & $executable
					| ForEach-Object {
						write-host `t $_ -ForegroundColor Green
					}
				}
			write-host "`ntest/code_sanity completed in $($time_taken.TotalMilliseconds) ms"
		}
	Pop-Location

	$compiler_args += ($flag_define + "TEST_SINGLEHEADER")

	$result = build-simple $path_build $includes $compiler_args $linker_args $unit $executable

	Push-Location $path_root
	if ( Test-Path( $executable ) ) {
		write-host "`nRunning tests/c11_sanity.exe (single header)"
		$time_taken = Measure-Command { & $executable
				| ForEach-Object {
					write-host `t $_ -ForegroundColor Green
				}
			}
		write-host "`ntest/code_sanity completed in $($time_taken.TotalMilliseconds) ms"
	}
	Pop-Location
}

if ($cpp17_sanity)
{

}

if ($sanity_tests)
{
	write-host " Building tests/sanity_tests.c"

	$compiler_args = @()
	$compiler_args += $flag_all_c
	$compiler_args += $flag_updated_cpp_macro
	$compiler_args += $flag_c11

	$linker_args = @()
	$linker_args += $flag_link_win_subsystem_console

	$path_gen = join-path $path_gen_c11 'gen'

	$includes   = @( $path_gen, $path_root )
	$unit       = join-path $path_tests  'sanity_tests.c'
	$executable = join-path $path_build  'sanity_tests.exe'

	$result = build-simple $path_build $includes $compiler_args $linker_args $unit $executable

	Push-Location $path_root
		if ( Test-Path( $executable ) ) {
			write-host "`nRunning gen_c11/sanity_tests.exe"
			$time_taken = Measure-Command { & $executable
					| ForEach-Object {
						write-host `t $_ -ForegroundColor Green
					}
				}
			write-host "`ntest/sanity_tests completed in $($time_taken.TotalMilliseconds) ms"
		}
	Pop-Location
}

if ($unicode_test)
{

}

if ($expression_tests)
{

}



Pop-Location # $path_root
