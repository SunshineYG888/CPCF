#pragma once

//////////////////////////////////////////////////////////////////////
// Cross-Platform Core Foundation (CPCF)
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of CPF.  nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//////////////////////////////////////////////////////////////////////

#include "platform.h"

#if defined(PLATFORM_ANDROID)
#include <android/log.h>
#endif


#ifdef PLATFORM_WIN

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

// Define PLATFORM_MAX_COMPATIBILITY to eliminate some APIs only available on later version of OS

#include <windows.h>
#include <tchar.h>
#undef GetCurrentTime

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

///////////////////////////////////////////////////////
// Warning style
#pragma warning(disable: 4996) //warning C4996: 'xxx' was declared deprecated
#pragma warning(disable: 4819) //warning C4819: The file contains a character that cannot be represented in the current code page (936)
#pragma warning(disable: 4996) //warning C4996: 'xxx' was declared deprecated
#pragma warning(disable: 4616) //warning C4996: 'xxx' was declared deprecated
#pragma warning(disable: 4311) //warning C4311: 'type cast' : pointer truncation
#pragma warning(disable: 4311) //warning C4311: 'type cast' : pointer truncation
#pragma warning(disable: 4355) //warning C4355: 'this' used in ctor of base class
#pragma warning(disable: 4197) //warning C4197: 'volatile int' : top-level volatile in cast is ignored
#pragma warning(disable: 4800) // warning C4800: 'bool' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(error : 4715 4700) //not all control paths return a value


///////////////////////////////////////////////////////
// Enable frequently used intrinsic function
#pragma intrinsic(strcmp,strcpy,strcat,strlen)
#pragma intrinsic(memcmp,memcpy,memset)

#endif	// #ifdef PLATFORM_WIN

#include <stdlib.h>
#include <stdio.h>


///////////////////////////////////////////////////////
// Basic data types
#ifndef PLATFORM_WIN
typedef void*               	LPVOID;
typedef const void*         	LPCVOID;
typedef unsigned char			BYTE;
typedef char					CHAR;
typedef signed short            SHORT;
typedef unsigned short			WORD;
typedef unsigned int			DWORD;
typedef unsigned int			UINT;
typedef signed int             	INT;
typedef signed long long int	LONGLONG;
typedef unsigned long long int 	ULONGLONG;
typedef LPVOID					HANDLE;
typedef BYTE*               	LPBYTE;
typedef char*               	LPSTR;
typedef const char*         	LPCSTR;
typedef WORD*               	LPWORD;
typedef DWORD*              	LPDWORD;
typedef LONGLONG				__time64_t;
typedef LONGLONG				__int64;
typedef ULONGLONG				__uint64;

#if defined(PLATFORM_64BIT)
typedef ULONGLONG              	SIZE_T;
typedef LONGLONG             	SSIZE_T;
#else
typedef UINT            	  	SIZE_T;
typedef INT             		SSIZE_T;
#endif

#define __FUNCTION__   __func__ 
typedef ULONGLONG	__uint64;

#endif


#ifndef INFINITE
#define INFINITE (0xffffffff)
#endif

#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE	((HANDLE)INFINITE)
#endif

typedef float*				LPFLOAT;
typedef const float*		LPCFLOAT;
typedef const BYTE*         LPCBYTE;
typedef const WORD*			LPCWORD;
typedef const DWORD*		LPCDWORD;
typedef ULONGLONG			QWORD;
typedef QWORD*				LPQWORD;
typedef const QWORD*		LPCQWORD;
///////////////////////////////////////////////////////

#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[ 8 ];
} GUID;
#endif

// GCC does not support _rotl outside of Windows
#if !defined(PLATFORM_WIN)
#define _rotl(x,r) ((x << r) | (x >> (32 - r)))
#endif

///////////////////////////////////////////////////////
// Misc Helper macro
#define sizeofArray(array_name) (sizeof(array_name)/sizeof(array_name[0]))

#ifndef MARCO_JOIN
#	define MARCO_JOIN( X, Y ) _MACRO_DO_JOIN( X, Y )
#	define _MACRO_DO_JOIN( X, Y ) _MACRO_DO_JOIN2(X,Y)
#	define _MACRO_DO_JOIN2( X, Y ) X##Y
#endif

#define MARCO_CONCAT(...) __VA_ARGS__

#ifdef __STRING
#undef __STRING
#endif
#define __STRING(...) #__VA_ARGS__ 


#ifndef INLFUNC
#define INLFUNC	inline
#endif

#ifndef FORCEINL

#ifdef PLATFORM_WIN
	#define FORCEINL __forceinline
#elif defined(__GNUC__) && __GNUC__ > 3
    #define FORCEINL inline __attribute__ ((always_inline))
#else
	#define FORCEINL inline
#endif

#endif
