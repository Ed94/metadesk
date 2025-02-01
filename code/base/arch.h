#if MD_INTELLISENSE_DIRECTIVES
#pragma once
#endif

#if defined( _WIN64 ) || defined( __x86_64__ ) || defined( _M_X64 ) || defined( __64BIT__ ) || defined( __powerpc64__ ) || defined( __ppc64__ ) || defined( __aarch64__ )
#	ifndef     MD_ARCH_64_BIT
#		define MD_ARCH_64_BIT 1
#	endif
#else
#	ifndef     MD_ARCH_32_BIT
#		define MD_ARCH_32_BIT 1
#	endif
#endif
