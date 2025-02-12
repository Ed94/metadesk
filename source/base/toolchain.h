#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "base_types.h"
#endif

////////////////////////////////
//~ rjf: Toolchain/Environment Enums

typedef enum MD_OperatingSystem MD_OperatingSystem;
enum MD_OperatingSystem
{
	MD_OperatingSystem_Null,
	MD_OperatingSystem_Windows,
	MD_OperatingSystem_Linux,
	MD_OperatingSystem_Mac,
	MD_OperatingSystem_COUNT,
};

typedef enum MD_ImageType MD_ImageType;
enum MD_ImageType
{
  MD_Image_Null,
  MD_Image_CoffPe,
  MD_Image_Elf32,
  MD_Image_Elf64,
  MD_Image_Macho
};

typedef enum MD_Arch MD_Arch;
enum MD_Arch
{
	MD_Arch_Null,
	MD_Arch_x64,
	MD_Arch_x86,
	MD_Arch_arm64,
	MD_Arch_arm32,
	MD_Arch_COUNT,
};

typedef enum MD_Compiler MD_Compiler;
enum MD_Compiler
{
	MD_Compiler_Null,
	MD_Compiler_msvc,
	MD_Compiler_gcc,
	MD_Compiler_clang,
	MD_Compiler_COUNT,
};

////////////////////////////////
//~ rjf: Toolchain/Environment Enum Functions

inline MD_U64
md_bit_size_from_arch(MD_Arch arch)
{
	// TODO(rjf): metacode
	MD_U64 arch_bitsize = 0;
	switch(arch)
	{
		case MD_Arch_x64:   arch_bitsize = 64; break;
		case MD_Arch_x86:   arch_bitsize = 32; break;
		case MD_Arch_arm64: arch_bitsize = 64; break;
		case MD_Arch_arm32: arch_bitsize = 32; break;
		default: break;
	}
	return arch_bitsize;
}

inline MD_U64
md_max_instruction_size_from_arch(MD_Arch arch)
{
  // TODO(rjf): make this real
  return 64;
}

inline MD_OperatingSystem
md_operating_system_from_context(void) {
	MD_OperatingSystem os = MD_OperatingSystem_Null;
#if MD_OS_WINDOWS
	os = MD_OperatingSystem_Windows;
#elif MD_OS_LINUX
	os = MD_OperatingSystem_Linux;
#elif MD_OS_MAC
	os = MD_OperatingSystem_Mac;
#endif
	return os;
}

inline MD_Arch
md_arch_from_contexto(void) {
	MD_Arch arch = MD_Arch_Null;
#if MD_ARCH_X64
	arch = MD_Arch_x64;
#elif MD_ARCH_X86
	arch = MD_Arch_x86;
#elif MD_ARCH_ARM64
	arch = MD_Arch_arm64;
#elif MD_ARCH_ARM32
	arch = MD_Arch_arm32;
#endif
	return arch;
}

inline MD_Compiler
md_compiler_from_context(void) {
	MD_Compiler compiler = MD_Compiler_Null;
#if MD_COMPILER_MSVC
	compiler = MD_Compiler_msvc;
#elif MD_COMPILER_GCC
	compiler = MD_Compiler_gcc;
#elif MD_COMPILER_CLANG
	compiler = MD_Compiler_clang;
#endif
	return compiler;
}
