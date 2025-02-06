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

typedef enum ImageType
{
  Image_Null,
  Image_CoffPe,
  Image_Elf32,
  Image_Elf64,
  Image_Macho
} ImageType;

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

////////////////////////////////
//~ rjf: Toolchain/Environment Enum Functions

internal U64 bit_size_from_arch(Arch arch);
internal U64 max_instruction_size_from_arch(Arch arch);

internal OperatingSystem operating_system_from_context(void);
internal Arch arch_from_context(void);
internal Compiler compiler_from_context(void);
