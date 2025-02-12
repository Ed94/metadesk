#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "base_types.h"
#endif

////////////////////////////////
//~ rjf: Toolchain/Environment Enums

typedef enum OperatingSystem OperatingSystem;
enum OperatingSystem
{
	OperatingSystem_Null,
	OperatingSystem_Windows,
	OperatingSystem_Linux,
	OperatingSystem_Mac,
	OperatingSystem_COUNT,
};

typedef enum ImageType ImageType;
enum ImageType
{
  Image_Null,
  Image_CoffPe,
  Image_Elf32,
  Image_Elf64,
  Image_Macho
};

typedef enum Arch Arch;
enum Arch
{
	Arch_Null,
	Arch_x64,
	Arch_x86,
	Arch_arm64,
	Arch_arm32,
	Arch_COUNT,
};

typedef enum Compiler Compiler;
enum Compiler
{
	Compiler_Null,
	Compiler_msvc,
	Compiler_gcc,
	Compiler_clang,
	Compiler_COUNT,
};

////////////////////////////////
//~ rjf: Toolchain/Environment Enum Functions

inline U64
bit_size_from_arch(Arch arch)
{
	// TODO(rjf): metacode
	U64 arch_bitsize = 0;
	switch(arch)
	{
		case Arch_x64:   arch_bitsize = 64; break;
		case Arch_x86:   arch_bitsize = 32; break;
		case Arch_arm64: arch_bitsize = 64; break;
		case Arch_arm32: arch_bitsize = 32; break;
		default: break;
	}
	return arch_bitsize;
}

inline U64
max_instruction_size_from_arch(Arch arch)
{
  // TODO(rjf): make this real
  return 64;
}

inline OperatingSystem
operating_system_from_context(void) {
	OperatingSystem os = OperatingSystem_Null;
#if OS_WINDOWS
	os = OperatingSystem_Windows;
#elif OS_LINUX
	os = OperatingSystem_Linux;
#elif OS_MAC
	os = OperatingSystem_Mac;
#endif
	return os;
}

inline Arch
arch_from_context(void) {
	Arch arch = Arch_Null;
#if ARCH_X64
	arch = Arch_x64;
#elif ARCH_X86
	arch = Arch_x86;
#elif ARCH_ARM64
	arch = Arch_arm64;
#elif ARCH_ARM32
	arch = Arch_arm32;
#endif
	return arch;
}

inline Compiler
compiler_from_context(void) {
	Compiler compiler = Compiler_Null;
#if COMPILER_MSVC
	compiler = Compiler_msvc;
#elif COMPILER_GCC
	compiler = Compiler_gcc;
#elif COMPILER_CLANG
	compiler = Compiler_clang;
#endif
	return compiler;
}
