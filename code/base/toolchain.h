#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#endif

////////////////////////////////
//~ rjf: Toolchain/Environment Enums

typedef enum OperatingSystem
{
  OperatingSystem_Null,
  OperatingSystem_Windows,
  OperatingSystem_Linux,
  OperatingSystem_Mac,
  OperatingSystem_COUNT,
}
OperatingSystem;

typedef enum Architecture
{
  Architecture_Null,
  Architecture_x64,
  Architecture_x86,
  Architecture_arm64,
  Architecture_arm32,
  Architecture_COUNT,
}
Architecture;

typedef enum Compiler
{
  Compiler_Null,
  Compiler_msvc,
  Compiler_gcc,
  Compiler_clang,
  Compiler_COUNT,
}
Compiler;
