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

#ifndef __cplusplus
#error Not compiled as C++, use .mm instead of .m on Mac/iOS
#endif

#include <limits.h>
#include <float.h>
#include <sstream>
#include <time.h>
#include <math.h>
#include <intrin.h>

#include "../os/predefines.h"

#if defined(PLATFORM_IOS) || defined(PLATFORM_MAC)
#include <cmath>
#endif

#if defined(PLATFORM_LINUX)
#include <cstring>
#endif


///////////////////////////////////////////////////////
// Advanced Bits Operations
namespace rt
{

#if defined(PLATFORM_WIN)
	// count leading zeros
	INLFUNC UINT LeadingZeroBits(WORD x){ return (UINT)__lzcnt16(x); }
	INLFUNC UINT LeadingZeroBits(DWORD x){ return (UINT)__lzcnt(x); }
	INLFUNC UINT LeadingZeroBits(ULONGLONG x){ return (UINT)__lzcnt64(x); }
	// count 1s
	INLFUNC UINT NonzeroBits(WORD x){ return (UINT)__popcnt16(x); }
	INLFUNC UINT NonzeroBits(DWORD x){ return (UINT)__popcnt(x); }
	INLFUNC UINT NonzeroBits(ULONGLONG x){ return (UINT)__popcnt64(x); }
#else
#pragma message ("Advanced Bits Operations are not defined")
#endif

}

///////////////////////////////////////////////////////
// Hash map/set
#if defined(PLATFORM_IOS) || defined(PLATFORM_ANDROID) || defined(PLATFORM_MAC) || (defined(PLATFORM_WIN) && ( _MSC_VER >= 1900 )) || defined(PLATFORM_LINUX)

#if __cplusplus == 199711L && !defined(PLATFORM_WIN)
#include <tr1/unordered_map>
#include <tr1/unordered_set>

namespace rt
{
	template<class _Key, class _Tp,
	class _HashFn = std::tr1::hash<_Key> >
	class hash_map: public std::tr1::unordered_map<_Key, _Tp, _HashFn>
    {   typedef _Key	t_Key; typedef _Tp	t_Value;
        public: hash_map(){}
    };
	template<class _Key,
	class _HashFcn  = std::tr1::hash<_Key> >
	class hash_set: public std::tr1::unordered_set<_Key, _HashFcn>
    {   typedef _Key t_Key;
        public: hash_set(){}
    };
}

#else

#include <unordered_map>
#include <unordered_set>

namespace rt
{
	template<class _Key, class _Tp,
	class _HashFn = std::hash<_Key> >
	class hash_map: public std::unordered_map<_Key, _Tp, _HashFn>
    {   typedef _Key	t_Key; typedef _Tp	t_Value;
        public: hash_map(){}
    };
	template<class _Key,
	class _HashFcn  = std::hash<_Key> >
	class hash_set: public std::unordered_set<_Key, _HashFcn>
    {   typedef _Key t_Key;
        public: hash_set(){}
    };
}

#endif

#else
#include <hash_map>
#include <hash_set>
namespace rt
{
    template<class _Kty, class _Ty,
	class _Tr = std::hash_compare<_Kty, std::less<_Kty> >,
	class _Alloc = std::allocator<std::pair<const _Kty, _Ty> > >
    class hash_map: public std::hash_map<_Kty, _Ty, _Tr, _Alloc>
    {   typedef _Kty t_Key; typedef _Ty t_Value;
        public: hash_map(){}
    };
    template<class _Kty,
	class _Tr = std::hash_compare<_Kty, std::less<_Kty> >,
	class _Alloc = std::allocator<_Kty> >
    class hash_set: public std::hash_set<_Kty, _Tr, _Alloc>
    {   typedef _Kty t_Key;
        public: hash_set(){}
    };
}
#endif

namespace rt
{
namespace _details
{
template<typename T>
struct _LOG_FULLDATA: public T{};
}

template<typename T>
INLFUNC const _details::_LOG_FULLDATA<T>& LOG_FULLDATA(const T& x){ return (const _details::_LOG_FULLDATA<T>&)x; }

enum _tagLogType
{
	LOGTYPE_VERBOSE = 0,
	LOGTYPE_UPDATING,
	LOGTYPE_INFORMATIONAL,
	LOGTYPE_HIGHLIGHT,
	LOGTYPE_WARNING,
	LOGTYPE_ERROR,
	LOGTYPE_LEVEL_MASK = 0xff,
	LOGTYPE_MAX,
	LOGTYPE_IN_CONSOLE = 0x100,
	LOGTYPE_IN_LOGFILE = 0x200,
	LOGTYPE_IN_CONSOLE_FORCE = 0x400,
	LOGTYPE_IN_CONSOLE_PROMPT = 0x800,
};
} // namespace rt

///////////////////////////////////////////////////////
namespace os
{	extern void LogWrite(LPCSTR log, LPCSTR file, int line_num, LPCSTR func, int type);
	extern void LogDisplayInConsole(bool yes);
	extern bool LogIsDisplayInConsole();
}

#ifndef PLATFORM_DISABLE_LOG
	#define __LOG_TYPE(x,type)	{	 \
									std::ostringstream _ostr;							\
									_ostr<<x;											\
									::os::LogWrite(	_ostr.rdbuf()->str().c_str(),__FILE__,__LINE__,__FUNCTION__, type); \
								}
    #define _LOGNL				{ ::os::LogWrite("",__FILE__,__LINE__,__FUNCTION__, rt::LOGTYPE_VERBOSE); }
#else
	#define __LOG_TYPE(x,type)	{}
	#define _LOGNL				{}
#endif


// Log to both console and file
#define _LOG_VERBOSE(x)				__LOG_TYPE(x, rt::LOGTYPE_IN_CONSOLE|rt::LOGTYPE_IN_LOGFILE|rt::LOGTYPE_VERBOSE)
#define _LOG_UPDATING(x)			__LOG_TYPE(x, rt::LOGTYPE_IN_CONSOLE|rt::LOGTYPE_UPDATING)
#define _LOG(x)						__LOG_TYPE(x, rt::LOGTYPE_IN_CONSOLE|rt::LOGTYPE_IN_LOGFILE|rt::LOGTYPE_INFORMATIONAL)
#define _LOG_CONSOLE(x)				__LOG_TYPE(x, rt::LOGTYPE_IN_CONSOLE_FORCE|rt::LOGTYPE_IN_LOGFILE|rt::LOGTYPE_INFORMATIONAL)
#define _LOG_CONSOLE_PROMPT()		__LOG_TYPE("", rt::LOGTYPE_IN_CONSOLE_PROMPT|rt::LOGTYPE_IN_LOGFILE|rt::LOGTYPE_INFORMATIONAL)
#define _LOG_HIGHLIGHT(x)			__LOG_TYPE(x, rt::LOGTYPE_IN_CONSOLE|rt::LOGTYPE_IN_LOGFILE|rt::LOGTYPE_HIGHLIGHT)
#define _LOG_WARNING(x)				__LOG_TYPE(x, rt::LOGTYPE_IN_CONSOLE|rt::LOGTYPE_IN_LOGFILE|rt::LOGTYPE_WARNING)
#define _LOG_ERROR(x)				__LOG_TYPE(x, rt::LOGTYPE_IN_CONSOLE|rt::LOGTYPE_IN_LOGFILE|rt::LOGTYPE_ERROR)

#define _LOG_POS					__LOG_TYPE(__FUNCTION__<<":L"<<__LINE__<<":"<<__FILE__, rt::LOGTYPE_IN_CONSOLE|rt::LOGTYPE_IN_LOGFILE|rt::LOGTYPE_VERBOSE)
#define _LOG_POS_WARNING			__LOG_TYPE(__FUNCTION__<<":L"<<__LINE__<<":"<<__FILE__, rt::LOGTYPE_IN_CONSOLE|rt::LOGTYPE_IN_LOGFILE|rt::LOGTYPE_WARNING)

// Only Log to console
#define _LOGC_VERBOSE(x)			__LOG_TYPE(x, rt::LOGTYPE_IN_CONSOLE|rt::LOGTYPE_VERBOSE)
#define _LOGC(x)					__LOG_TYPE(x, rt::LOGTYPE_IN_CONSOLE|rt::LOGTYPE_INFORMATIONAL)
#define _LOGC_HIGHLIGHT(x)			__LOG_TYPE(x, rt::LOGTYPE_IN_CONSOLE|rt::LOGTYPE_HIGHLIGHT)
#define _LOGC_WARNING(x)			__LOG_TYPE(x, rt::LOGTYPE_IN_CONSOLE|rt::LOGTYPE_WARNING)
#define _LOGC_ERROR(x)				__LOG_TYPE(x, rt::LOGTYPE_IN_CONSOLE|rt::LOGTYPE_ERROR)
			
#define _LOGC_POS					__LOG_TYPE(__FUNCTION__<<":L"<<__LINE__<<":"<<__FILE__, rt::LOGTYPE_IN_CONSOLE|rt::LOGTYPE_VERBOSE)
#define _LOGC_POS_WARNING			__LOG_TYPE(__FUNCTION__<<":L"<<__LINE__<<":"<<__FILE__, rt::LOGTYPE_IN_CONSOLE|rt::LOGTYPE_WARNING)

// Only Log to file
#define _LOGF_VERBOSE(x)			__LOG_TYPE(x, rt::LOGTYPE_IN_LOGFILE|rt::LOGTYPE_VERBOSE)
#define _LOGF(x)					__LOG_TYPE(x, rt::LOGTYPE_IN_LOGFILE|rt::LOGTYPE_INFORMATIONAL)
#define _LOGF_HIGHLIGHT(x)			__LOG_TYPE(x, rt::LOGTYPE_IN_LOGFILE|rt::LOGTYPE_HIGHLIGHT)
#define _LOGF_WARNING(x)			__LOG_TYPE(x, rt::LOGTYPE_IN_LOGFILE|rt::LOGTYPE_WARNING)
#define _LOGF_ERROR(x)				__LOG_TYPE(x, rt::LOGTYPE_IN_LOGFILE|rt::LOGTYPE_ERROR)

#define _LOGF_POS					__LOG_TYPE(__FUNCTION__<<":L"<<__LINE__<<":"<<__FILE__, rt::LOGTYPE_IN_LOGFILE|rt::LOGTYPE_VERBOSE)
#define _LOGF_POS_WARNING			__LOG_TYPE(__FUNCTION__<<":L"<<__LINE__<<":"<<__FILE__, rt::LOGTYPE_IN_LOGFILE|rt::LOGTYPE_WARNING)

#define _LOGP						os::LogPrefix()


///////////////////////////////////////////////////////
// Debugging and error handling
#ifndef ASSERT
	#ifdef PLATFORM_DEBUG_BUILD
		#if defined(PLATFORM_WIN)
			#define ASSERT(x) if(!(x)){ _LOG_ERROR("ASSERTION FAILED: " #x); _LOG_POS; _ASSERT(x); }
		#elif defined(PLATFORM_ANDROID)
			#define ASSERT(x) if(!(x)){ _LOG_ERROR("ASSERTION FAILED: " #x); _LOG_POS; }
		#else
			#include <assert.h>
			#define ASSERT(x) assert(x)
		#endif
	#else
		#define ASSERT(x) {}
	#endif
#endif


#ifdef VERIFY
#undef VERIFY
#endif

#define VERIFY(x)	rt::_details::_VERIFY_RET(x, #x, __FUNCTION__ , __LINE__ , __FILE__)

#define ASSERTH(re) ASSERT(SUCCEEDED(re))
#define VERIFYH(re) VERIFY(SUCCEEDED(re))

namespace rt
{
namespace _details
{

INLFUNC bool _VERIFY_RET(bool exp, LPCSTR exp_str, LPCSTR func, int line, LPCSTR file)
{
#ifdef PLATFORM_DEBUG_BUILD
	if(!exp)
	{
		_LOG_ERROR("ASSERTION FAILED: "<<exp_str);
		__LOG_TYPE(func<<":L"<<line<<":"<<file, rt::LOGTYPE_IN_LOGFILE|rt::LOGTYPE_VERBOSE);
		ASSERT(0);
	}
#endif
	return exp;
}

} // namespace _details
} // namespace rt

///////////////////////////////////////////////////////

#if defined(PLATFORM_IOS) || defined(PLATFORM_MAC)
#define ASSERT_STATIC(x)	static int MARCO_JOIN(__assertion_failed__,__COUNTER__)[((int)((bool)(x)))*2 - 1] __attribute__((unused));
#else
#define ASSERT_STATIC(x)	static int MARCO_JOIN(__assertion_failed__,__COUNTER__)[((int)((bool)(x)))*2 - 1];
#endif

///////////////////////////////////////////////////////
// Runtime diagnosis
#if defined(PLATFORM_DEBUG_BUILD) && defined(PLATFORM_WIN)
	#define _CheckHeap	ASSERT(_CrtCheckMemory())
#else
	#define _CheckHeap	{}
#endif

#ifdef PLATFORM_DEBUG_BUILD
	#define ASSERT_FLOAT(x)	{ ASSERT(rt::IsNumberOk(x)); }
	#define VERIFY_FLOAT(x)	ASSERT_FLOAT(x)
#else
	#define ASSERT_FLOAT(x)	while(0)
	#define VERIFY_FLOAT(x)	while(x){ break; }
#endif


///////////////////////////////////////////////////////


///////////////////////////////////////////////////////
// Some constant 
namespace rt
{
 
struct _DummyType;

template<typename T>
INLFUNC T Sqr(T x){ return x*x; }

template<typename T>
INLFUNC T max(const T& x, const T& y)
{
#pragma warning(disable:4018)
	return (x<y? y:x);
#pragma warning(default:4018)
}
    
template<typename T>
INLFUNC T min(const T& x, const T& y)
{
#pragma warning(disable:4018)
	return (x>y? y:x);
#pragma warning(default:4018)
}
	
template<typename T>
INLFUNC T abs(const T& x)
{
#pragma warning(disable:4018)
	return (x>=0? x:-x);
#pragma warning(default:4018)
}

template<typename T, typename T_R>
INLFUNC bool IsInRange_OO(T var,T_R min_v,T_R max_v)
{
	return (var > min_v) && (var < max_v);
}

template<typename T, typename T_R>
INLFUNC bool IsInRange_CC(T var,T_R min_v,T_R max_v)
{
	return (var >= min_v) && (var <= max_v);
}

template<typename T, typename T_R>
INLFUNC bool IsInRange_CO(T var,T_R min_v,T_R max_v)
{
	return (var >= min_v) && (var < max_v);
}

template<typename T, typename T_R>
INLFUNC bool IsInRange_OC(T var,T_R min_v,T_R max_v)
{
	return (var > min_v) && (var <= max_v);
}

#if defined(PLATFORM_WIN)
FORCEINL bool IsNumberOk				(float val){ return ::rt::IsInRange_CC(_fpclass(val),_FPCLASS_NN,_FPCLASS_PN); }
FORCEINL bool IsNumberOk				(double val){ return ::rt::IsInRange_CC(_fpclass(val),_FPCLASS_NN,_FPCLASS_PN); }
#elif defined(PLATFORM_ANDROID)
FORCEINL bool IsNumberOk				(float val){ return __isnormalf(val); }
FORCEINL bool IsNumberOk				(double val){ return __isnormal(val); }
#else
FORCEINL bool IsNumberOk				(float val){ int c = fpclassify(val); return c!=FP_INFINITE && c!=FP_NAN; }
FORCEINL bool IsNumberOk				(double val){ int c = fpclassify(val); return c!=FP_INFINITE && c!=FP_NAN; }
#endif

FORCEINL bool IsPositiveInfinity		(float val){ return ((DWORD&)val)==0x7f800000; }
FORCEINL bool IsNegativeInfinity		(float val){ return ((DWORD&)val)==0xff800000; }
FORCEINL bool IsNotANumber				(float val){ return ((DWORD&)val)==0x7fffffff; }
FORCEINL bool IsPositiveInfinity		(double val){ return ((ULONGLONG&)val)==0x7ff0000000000000; }
FORCEINL bool IsNegativeInfinity		(double val){ return ((ULONGLONG&)val)==0xfff0000000000000; }
FORCEINL bool IsNotANumber				(double val){ return ((ULONGLONG&)val)==0x7fffffffffffffff; }
FORCEINL void MakePositiveInfinity		(float& val){ ((DWORD&)val)=0x7f800000; }
FORCEINL void MakeNegativeInfinity		(float& val){ ((DWORD&)val)=0xff800000; }
FORCEINL void MakeNotANumber			(float& val){ ((DWORD&)val)=0x7fffffff; }
FORCEINL void MakePositiveInfinity		(double& val){ ((ULONGLONG&)val)=0x7ff0000000000000; }
FORCEINL void MakeNegativeInfinity		(double& val){ ((ULONGLONG&)val)=0xfff0000000000000; }
FORCEINL void MakeNotANumber			(double& val){ ((ULONGLONG&)val)=0x7fffffffffffffff; }

// PopCount
namespace _details
{
const ULONGLONG m1  = 0x5555555555555555; //binary: 0101...
const ULONGLONG m2  = 0x3333333333333333; //binary: 00110011..
const ULONGLONG m4  = 0x0f0f0f0f0f0f0f0f; //binary:  4 zeros,  4 ones ...
};

FORCEINL int PopCount(ULONGLONG x) {
    x -= (x >> 1) & _details::m1;             //put count of each 2 bits into those 2 bits
    x = (x & _details::m2) + ((x >> 2) & _details::m2); //put count of each 4 bits into those 4 bits 
    x = (x + (x >> 4)) & _details::m4;        //put count of each 8 bits into those 8 bits 
    x += x >>  8;  //put count of each 16 bits into their lowest 8 bits
    x += x >> 16;  //put count of each 32 bits into their lowest 8 bits
    x += x >> 32;  //put count of each 64 bits into their lowest 8 bits
    return x & 0x7f;
}



// xxHash
namespace _details
{
	static const UINT PRIME1 =  2654435761U;
	static const UINT PRIME2 =  2246822519U;
	static const UINT PRIME3 =  3266489917U;
	static const UINT PRIME4 =   668265263U;
	static const UINT PRIME5 =  0x165667b1;

	// This version is for very small inputs (< 16  bytes)
	FORCEINL unsigned int xxHash_small(const void* key, int len, unsigned int seed)
	{	const unsigned char* p = (unsigned char*)key;
		const unsigned char* const bEnd = p + len;
		unsigned int idx = seed + PRIME1;
		unsigned int crc = PRIME5;
		const unsigned char* const limit = bEnd - 4;
		while (p<limit)
		{	crc += ((*(unsigned int*)p) + idx++);
			crc += _rotl(crc, 17) * PRIME4;
			crc *= PRIME1;
			p+=4;
		}
		while (p<bEnd)
		{	crc += ((*p) + idx++);
			crc *= PRIME1;
			p++;
		}
		crc += len;
		crc ^= crc >> 15;
		crc *= PRIME2;
		crc ^= crc >> 13;
		crc *= PRIME3;
		crc ^= crc >> 16;
		return crc;
	}
}

FORCEINL UINT xxHash(const void* input, int len, unsigned int seed = 0)
{	// Special case, for small inputs
	if (len < 16) return _details::xxHash_small(input, len, seed);
	const unsigned char* p = (const unsigned char*)input;
	const unsigned char* const bEnd = p + len;
	unsigned int v1 = seed + _details::PRIME1;
	unsigned int v2 = v1 * _details::PRIME2 + len;
	unsigned int v3 = v2 * _details::PRIME3;
	unsigned int v4 = v3 * _details::PRIME4;	
	const unsigned char* const limit = bEnd - 16;
	unsigned int crc;
	while (p<limit)
	{	v1 = _rotl(v1, 13) + (*(unsigned int*)p); p+=4;
		v2 = _rotl(v2, 11) + (*(unsigned int*)p); p+=4;
		v3 = _rotl(v3, 17) + (*(unsigned int*)p); p+=4;
		v4 = _rotl(v4, 19) + (*(unsigned int*)p); p+=4;
	} 

	p = bEnd - 16;
	v1 += _rotl(v1, 17); v2 += _rotl(v2, 19); v3 += _rotl(v3, 13); v4 += _rotl(v4, 11); 
	v1 *= _details::PRIME1; v2 *= _details::PRIME1; v3 *= _details::PRIME1; v4 *= _details::PRIME1; 
	v1 += *(unsigned int*)p; p+=4; v2 += *(unsigned int*)p; p+=4; v3 += *(unsigned int*)p; p+=4; v4 += *(unsigned int*)p;   // p+=4;
	v1 *= _details::PRIME2; v2 *= _details::PRIME2; v3 *= _details::PRIME2; v4 *= _details::PRIME2; 
	v1 += _rotl(v1, 11); v2 += _rotl(v2, 17); v3 += _rotl(v3, 19); v4 += _rotl(v4, 13); 
	v1 *= _details::PRIME3; v2 *= _details::PRIME3; v3 *= _details::PRIME3; v4 *= _details::PRIME3;

	crc = v1 + _rotl(v2, 3) + _rotl(v3, 6) + _rotl(v4, 9);
	crc ^= crc >> 11;
	crc += (_details::PRIME4+len) * _details::PRIME1;
	crc ^= crc >> 15;
	crc *= _details::PRIME2;
	crc ^= crc >> 13;

	return crc;
}

FORCEINL UINT xxHashStrong(const void* input, int len, unsigned int seed = 0)
{	// Special case, for small inputs
	if (len < 16) return _details::xxHash_small(input, len, seed);
	const unsigned char* p = (const unsigned char*)input;
	const unsigned char* const bEnd = p + len;
	unsigned int v1 = seed + _details::PRIME1;
	unsigned int v2 = v1 * _details::PRIME2 + len;
	unsigned int v3 = v2 * _details::PRIME3;
	unsigned int v4 = v3 * _details::PRIME4;	
	const unsigned char* const limit = bEnd - 16;
	unsigned int crc;

	while (p<limit)
	{
		v1 += _rotl(v1, 13); v1 *= _details::PRIME1; v1 += (*(unsigned int*)p); p+=4;
		v2 += _rotl(v2, 11); v2 *= _details::PRIME1; v2 += (*(unsigned int*)p); p+=4;
		v3 += _rotl(v3, 17); v3 *= _details::PRIME1; v3 += (*(unsigned int*)p); p+=4;
		v4 += _rotl(v4, 19); v4 *= _details::PRIME1; v4 += (*(unsigned int*)p); p+=4;
	} 

	p = bEnd - 16;
	v1 += _rotl(v1, 17); v2 += _rotl(v2, 19); v3 += _rotl(v3, 13); v4 += _rotl(v4, 11); 
	v1 *= _details::PRIME1; v2 *= _details::PRIME1; v3 *= _details::PRIME1; v4 *= _details::PRIME1; 
	v1 += *(unsigned int*)p; p+=4; v2 += *(unsigned int*)p; p+=4; v3 += *(unsigned int*)p; p+=4; v4 += *(unsigned int*)p;   // p+=4;
	v1 *= _details::PRIME2; v2 *= _details::PRIME2; v3 *= _details::PRIME2; v4 *= _details::PRIME2; 
	v1 += _rotl(v1, 11); v2 += _rotl(v2, 17); v3 += _rotl(v3, 19); v4 += _rotl(v4, 13); 
	v1 *= _details::PRIME3; v2 *= _details::PRIME3; v3 *= _details::PRIME3; v4 *= _details::PRIME3;

	crc = v1 + _rotl(v2, 3) + _rotl(v3, 6) + _rotl(v4, 9);
	crc ^= crc >> 11;
	crc += (_details::PRIME4+len) * _details::PRIME1;
	crc ^= crc >> 15;
	crc *= _details::PRIME2;
	crc ^= crc >> 13;
	return crc;
}


struct _File
{
	static const int Seek_Begin		= 0;
	static const int Seek_Current	= 1;
	static const int Seek_End		= 2;

	virtual SIZE_T		Read(LPVOID lpBuf,SIZE_T nCount) = 0;
	virtual SIZE_T		Write(LPCVOID lpBuf,SIZE_T nCount) = 0;
	virtual SIZE_T		Seek(SSIZE_T offset,UINT nFrom = Seek_Begin) = 0; // return ULLONG_MAX for failure.
	virtual SIZE_T		GetLength() const = 0;
};

namespace _details
{

template<int Size, SIZE_T set> struct _PodFill
{	static FORCEINL void Fill(LPBYTE p)
	{	*(SIZE_T*)p = set;
		_PodFill<Size-sizeof(SIZE_T), set>::Fill(p+sizeof(SIZE_T));
}	};
	template<SIZE_T set> struct _PodFill<0, set>{ static FORCEINL void Fill(LPBYTE p){}};
	template<SIZE_T set> struct _PodFill<1, set>{ static FORCEINL void Fill(LPBYTE p){ *p = (BYTE)set; }};
	template<SIZE_T set> struct _PodFill<2, set>{ static FORCEINL void Fill(LPBYTE p){ *(WORD*)p = (WORD)set; }};
	template<SIZE_T set> struct _PodFill<3, set>{ static FORCEINL void Fill(LPBYTE p){ *(WORD*)p = (WORD)set; p[2] = (BYTE)set; }};
#if defined(PLATFORM_64BIT)
	template<SIZE_T set> struct _PodFill<4, set>{ static FORCEINL void Fill(LPBYTE p){ *(DWORD*)p = (DWORD)set; }};
	template<SIZE_T set> struct _PodFill<5, set>{ static FORCEINL void Fill(LPBYTE p){ *(DWORD*)p = (DWORD)set; _PodFill<1, set>::Fill(p+4); }};
	template<SIZE_T set> struct _PodFill<6, set>{ static FORCEINL void Fill(LPBYTE p){ *(DWORD*)p = (DWORD)set; _PodFill<2, set>::Fill(p+4); }};
	template<SIZE_T set> struct _PodFill<7, set>{ static FORCEINL void Fill(LPBYTE p){ *(DWORD*)p = (DWORD)set; _PodFill<3, set>::Fill(p+4); }};
#endif

} // namespace _details

template<typename T> FORCEINL void Zero(T& obj){ _details::_PodFill<sizeof(T), 0>::Fill((LPBYTE)&obj); }
template<typename T> FORCEINL void Void(T& obj){ _details::_PodFill<sizeof(T), (SIZE_T)-1>::Fill((LPBYTE)&obj); }
template<UINT LEN> FORCEINL void Zero(LPVOID obj){ _details::_PodFill<LEN, 0>::Fill((LPBYTE)&obj); }
template<UINT LEN> FORCEINL void Void(LPVOID obj){ _details::_PodFill<LEN, (SIZE_T)-1>::Fill((LPBYTE)&obj); }

FORCEINL void Zero(LPVOID obj, SIZE_T size)
{	SIZE_T* p = (SIZE_T*)obj;
	for(;size>=sizeof(SIZE_T);size-=sizeof(SIZE_T), p++)*p = 0;
	switch(size)
	{	case 0: return;
		case 1: _details::_PodFill<1, 0>::Fill((LPBYTE)p); return;
		case 2: _details::_PodFill<2, 0>::Fill((LPBYTE)p); return;
		case 3: _details::_PodFill<3, 0>::Fill((LPBYTE)p); return;
#if defined(PLATFORM_64BIT)
		case 4: _details::_PodFill<4, 0>::Fill((LPBYTE)p); return;
		case 5: _details::_PodFill<5, 0>::Fill((LPBYTE)p); return;
		case 6: _details::_PodFill<6, 0>::Fill((LPBYTE)p); return;
		case 7: _details::_PodFill<7, 0>::Fill((LPBYTE)p); return;
#endif
		default: ASSERT(0); return;
	}
}

FORCEINL void Void(LPVOID obj, SIZE_T size)
{	SIZE_T* p = (SIZE_T*)obj;
	for(;size>=sizeof(SIZE_T);size-=sizeof(SIZE_T), p++)*p = (SIZE_T)-1;
	switch(size)
	{	case 0: return;
		case 1: _details::_PodFill<1, (SIZE_T)-1>::Fill((LPBYTE)p); return;
		case 2: _details::_PodFill<2, (SIZE_T)-1>::Fill((LPBYTE)p); return;
		case 3: _details::_PodFill<3, (SIZE_T)-1>::Fill((LPBYTE)p); return;
#if defined(PLATFORM_64BIT)
		case 4: _details::_PodFill<4, (SIZE_T)-1>::Fill((LPBYTE)p); return;
		case 5: _details::_PodFill<5, (SIZE_T)-1>::Fill((LPBYTE)p); return;
		case 6: _details::_PodFill<6, (SIZE_T)-1>::Fill((LPBYTE)p); return;
		case 7: _details::_PodFill<7, (SIZE_T)-1>::Fill((LPBYTE)p); return;
#endif
		default: ASSERT(0); return;
	}
}


template<typename T1, typename T2>
FORCEINL void Copy(T1& obj, const T2& from){ ASSERT(sizeof(T1) == sizeof(T2)); memcpy(&obj, &from, sizeof(T1)); }


namespace _details
{
	// FNV Hash function (http://isthe.com/chongo/tech/comp/fnv/)
#if defined(PLATFORM_64BIT)
	static const SIZE_T	_HashInit = 14695981039346656037ULL;
	INLFUNC SIZE_T		_HashKernel(SIZE_T new_data, SIZE_T prev_val){ return 1099511628211ULL * (prev_val ^ new_data); }
#else
	static const SIZE_T	_HashInit = 2166136261U;
	INLFUNC SIZE_T		_HashKernel(SIZE_T new_data, SIZE_T prev_val){ return 16777619U * (prev_val ^ new_data); }
#endif

	template<int Size=0> struct _HashValueFix
	{	static INLFUNC SIZE_T Val(LPCBYTE p, SIZE_T prev_val = _HashInit)
		{	return _HashValueFix<Size-sizeof(SIZE_T)>::Val(p+sizeof(SIZE_T), _HashKernel(*(SIZE_T*)p, prev_val));
	}	};
		template<> struct _HashValueFix<0>{	static INLFUNC SIZE_T Val(LPCBYTE p, SIZE_T prev_val = _HashInit){ return prev_val; }};
		template<> struct _HashValueFix<1>{	static INLFUNC SIZE_T Val(LPCBYTE p, SIZE_T prev_val = _HashInit){ return _HashKernel((SIZE_T)*p, prev_val); }};
		template<> struct _HashValueFix<2>{	static INLFUNC SIZE_T Val(LPCBYTE p, SIZE_T prev_val = _HashInit){ return _HashKernel((SIZE_T)*(WORD*)p, prev_val); }};
		template<> struct _HashValueFix<3>{	static INLFUNC SIZE_T Val(LPCBYTE p, SIZE_T prev_val = _HashInit){ return _HashKernel((0xffffffU&(SIZE_T)*(DWORD*)p), prev_val); }};
#if defined(PLATFORM_64BIT)
		template<> struct _HashValueFix<4>{	static INLFUNC SIZE_T Val(LPCBYTE p, SIZE_T prev_val = _HashInit){ return _HashKernel((SIZE_T)*(DWORD*)p, prev_val); }};
		template<> struct _HashValueFix<5>{	static INLFUNC SIZE_T Val(LPCBYTE p, SIZE_T prev_val = _HashInit){ return _HashKernel(0xffffffffffULL&(SIZE_T)*(ULONGLONG*)p, prev_val); }};
		template<> struct _HashValueFix<6>{	static INLFUNC SIZE_T Val(LPCBYTE p, SIZE_T prev_val = _HashInit){ return _HashKernel(0xffffffffffffULL&(SIZE_T)*(ULONGLONG*)p, prev_val); }};
		template<> struct _HashValueFix<7>{	static INLFUNC SIZE_T Val(LPCBYTE p, SIZE_T prev_val = _HashInit){ return _HashKernel(0xffffffffffffffULL&(SIZE_T)*(ULONGLONG*)p, prev_val); }};
	// x64
	INLFUNC SIZE_T _HashValue(LPCVOID p_in, SIZE_T size, SIZE_T val = _HashInit)
	{	const SIZE_T* p = (const SIZE_T*)p_in;
		for(; size >= sizeof(SIZE_T); size -=sizeof(SIZE_T), p++)
			val = _HashKernel(*p, val);
		switch(size)
		{	case 0: return _HashValueFix<0>::Val((LPCBYTE)p, val);
			case 1: return _HashValueFix<1>::Val((LPCBYTE)p, val);
			case 2: return _HashValueFix<2>::Val((LPCBYTE)p, val);
			case 3: return _HashValueFix<3>::Val((LPCBYTE)p, val);
			case 4: return _HashValueFix<4>::Val((LPCBYTE)p, val);
			case 5: return _HashValueFix<5>::Val((LPCBYTE)p, val);
			case 6: return _HashValueFix<6>::Val((LPCBYTE)p, val);
			case 7: return _HashValueFix<7>::Val((LPCBYTE)p, val);
			default: ASSERT(0); return val;
		}
	}
#else
	// x86
	INLFUNC SIZE_T _HashValue(LPCVOID p_in, SIZE_T size, SIZE_T val = _HashInit)
	{	const SIZE_T* p = (const SIZE_T*)p_in;
		for(; size >= sizeof(SIZE_T); size -=sizeof(SIZE_T), p++)
			val = _HashKernel(*p, val);
		switch(size)
		{	case 0: return _HashValueFix<0>::Val((LPCBYTE)p, val);
			case 1: return _HashValueFix<1>::Val((LPCBYTE)p, val);
			case 2: return _HashValueFix<2>::Val((LPCBYTE)p, val);
			case 3: return _HashValueFix<3>::Val((LPCBYTE)p, val);
			default: ASSERT(0); return val;
		}
	}
#endif
	template<typename T>
	INLFUNC SIZE_T _HashValuePod(const T& obj, SIZE_T val = _HashInit)
	{	return _HashValueFix<sizeof(T)>::Val((LPCBYTE)&obj, val);
	}
	template<typename T>
	struct hash_compare_str
	{	// traits class for hash containers
		enum // parameters for hash table
		{	bucket_size = 4,	// 0 < bucket_size
			min_buckets = 8		// min_buckets = 2 ^^ N, 0 < N
		};
		INLFUNC size_t operator()(const T& key) const
		{	return _HashValue(key.Begin(),(key.End() - key.Begin())*sizeof(*key.Begin()));	
        }
		INLFUNC bool operator()(const T& _Keyval1, const T& _Keyval2) const
		{	return _Keyval1 < _Keyval2;		}
	};
	//template<typename T, int t_Block = sizeof(T)%4>
	template<typename T, int t_Block = 1>
	struct hash_compare_fix;
	template<typename T> struct hash_compare_fix<T, 1>
	{	
		enum // parameters for hash table
		{	bucket_size = 4,	// 0 < bucket_size
			min_buckets = 8		// min_buckets = 2 ^^ N, 0 < N
		};
		INLFUNC size_t operator()(const T& key) const
		{	return _HashValueFix<sizeof(T)>::Val((LPCBYTE)&key);}
		INLFUNC bool operator()(const T& _Keyval1, const T& _Keyval2) const
		{	return memcmp(&_Keyval1, &_Keyval2, sizeof(T)) < 0;	}
	};
}


} // namespace rt


///////////////////////////////////////////////////////
// Safe delete Objects
#define _SafeDel(x)			{ if(x){delete x; x=NULL;} }
#define _SafeDelArray(x)	{ if(x){delete [] x; x=NULL;} }
#define _SafeRelease(x)		{ if(x){x->Release(); x=NULL;} }
#define _SafeFree(x)		{ if(x){::free(x); x=NULL;} }

#if defined(PLATFORM_MAC) || defined(PLATFORM_IOS)
#define _SafeCFRelease(x)	{ if(x){::CFRelease(x); x=NULL;} }
#endif
///////////////////////////////////////////////////////


//////////////////////////////////////////////////////////
// 32byte Aligned Memory allocation and free 
//
// 1. use 32byte-Aligned Memory is optimized for cache hitting rate on CPUs of PIII and above
// 2. Add prefix and suffix bytes to allocated memory block to detect buffer overflow

///////////////////////////////////////////////////////////////////////////////////////////////////
//// 32byte Aligned Memory allocation layout
//// additional 32+1+4 bytes (or 32+1+8 bytes in Win64) is added to every memory block
//// 
////                 |----  Offset  ----|
//// /-- LEN-BYTE --\/-- Prefix bytes -\#/---------- User block -----------\/-- Suffix bytes --\
//// UUUUUUUUUUUUUUUUU ......... UUUUUUUUUUUUUUUUUUUUUUUUUU .... UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU
//// ^                                  ^^
//// |Original                          ||
//// \ allocated           Saved Offset /\ First aligned address after LEN-BYTE, the return 
////
//// 1. LEN-BYTE (size_t) indicates the size of User block
//// 
//// 2. If the p is returned by malloc and subtracted by 8, then &p[32-p%32] is the output address
////    and the offset=32-p%32 is saved at p[offset-1] as one byte the legal range of offset is from
////    1 to 32, and this value will be checked when free and used to calculate the original address
//// 
//// 3. The Prefix bytes (size=offset-1) and Suffix bytes (size=33-offset) will be check to ensure 
////    no boundary memory written occurred (buffer overflow)
/////////////////////////////////////////////////////////////////////////////////////////////////////

#define _Malloc32AL(type, co)	((type*)rt::mem32AL::Malloc32AL(sizeof(type)*(co), true))
#define _SafeFree32AL(ptr)		{ rt::mem32AL::Free32AL(ptr); ptr=0; }

namespace rt
{

FORCEINL SIZE_T EnlargeTo32AL(SIZE_T num){ return (((num) + 0x7)&(~((SIZE_T)0x7))); }
#define _Alloca32AL(sz)	(rt::EnlargeTo32AL((SIZE_T)alloca(sz + 4)))
#define _StackNew(type)	new (alloca(sizeof(type))) type

struct mem32AL
{
INLFUNC static LPVOID Malloc32AL(size_t size, bool allow_fail = false)   //size in byte
{
#pragma warning(disable:4244 4127)	// conversion from 'int' to 'BYTE', possible loss of data
									// conditional expression is constant
	LPBYTE p;
	//32 for alignment, 1 for suffix, 4(or 8) for LEN-BYTE
	ASSERT(size < size+32+1+sizeof(size_t));
	p = new(std::nothrow) BYTE[size+32+1+sizeof(size_t)]; 

#if defined CPF_MEMORY_LEAK_ADDRESS
	if( p == (LPBYTE)CPF_MEMORY_LEAK_ADDRESS )
	{	ASSERT(0);
		//Leaked memory block is located, check [Call Stack] for high-level code
	}
#endif

	if(p)
	{	//Record user block size for determining the high boundary of the memory block
		*((size_t*)p) = size; //size of User block
		p += sizeof(size_t);
		int offset;
		offset = 32 - (int)( ((size_t)p) & 0x1f);	// align to 32
		p[offset-1] = offset;	//Store offset

#if defined CPF_MEMORY_LEAK_ADDRESS
	if( &p[offset] == (LPBYTE)CPF_MEMORY_LEAK_ADDRESS )
	{	ASSERT(0);
		//abnormal used memory block is located, check [Call Stack] for high-level code
	}
#endif
		//Additional guard bytes is set for detecting broundry-overwriting, 
		//which will be checked when memory free. 
		int i;
		for(i=0;i<offset-1;i++)p[i] = 0x61+i;				//Set prefix bytes to 0x61+i = "abcde..."
		for(i=0;i<33-offset;i++)p[offset+size+i] = 0x41+i;	//Set suffix bytes to 0x41+i = "ABCDE..."

#ifdef CPF_REPORT_MEMORY_ACTIVITY
		_LOG("User block [0x"<<((LPVOID)&p[offset])<<"] allocated.\n");
#endif
		return &p[offset];
	}
	else  //Allocation failed
	{	
#ifdef CPF_REPORT_MEMORY_ACTIVITY
		_LOG("\n\nOut of memory: Failed to allocate "<<((size+41)/1024)<<"KB memory block!\7\n");
#endif
		ASSERT(0);
		return NULL;
	}
}

INLFUNC static void Free32AL(LPCVOID ptr_in)
{	if( ptr_in ) //NULL pointer is allowed but take no effect
	{	
#ifdef CPF_REPORT_MEMORY_ACTIVITY
		_LOG(_T("User block [0x")<<ptr_in<<_T("] try to free.\n"));
#endif
		LPCBYTE ptr = reinterpret_cast<LPCBYTE>(ptr_in);

		bool Prefix_Err_Detected = false; 
		bool Suffix_Err_Detected = false;
		size_t user_block_size = ((size_t)(-1));
		int offset = ptr[-1];
		if( offset <=32 && offset>0 )
		{	//Detect buffer overrun
			user_block_size = *((size_t*)&ptr[-offset-sizeof(size_t)]);

			int i; 
			for(i=0;i<offset-1;i++)
			{// check prefix bytes
				if( ptr[-(offset-i)] == 0x61+i ){}
				else{ Prefix_Err_Detected = true; break; }
			}
			for(i=0;i<33-offset;i++)
			{// check suffix bytes
				if( ptr[user_block_size+i] == 0x41+i ){}
				else{ Suffix_Err_Detected = true; break; }
			}

			if( !Prefix_Err_Detected && !Suffix_Err_Detected )
			{	
				//ensure heap integration 
				//_CheckHeap;
			
#ifdef CPF_SECURE_MEMORY_RELEASE
				rt::Zero((LPVOID)ptr_in, user_block_size);	// security enforcement
#endif
				delete [] (&ptr[-offset-sizeof(size_t)]);
				return;
			}
		}
		else{ Prefix_Err_Detected = true; }

		LPCSTR emsg = NULL;
		if(Prefix_Err_Detected && Suffix_Err_Detected)
		{	emsg = ("Both low and high");	}
		else if( Prefix_Err_Detected )
		{	emsg = ("Low");	}
		else if( Suffix_Err_Detected )
		{	emsg = ("High");	}
		else{ ASSERT(0); }

		_LOG("Abnormal block at 0x"<<((LPVOID)ptr)<<", "<<emsg<<" boundry was overwritten.");

		ASSERT(0);
	}
}
#pragma warning(default:4244 4127)
}; // class mem32AL

} // namespace rt


///////////////////////////////////////////////////////
// Misc inline templates
namespace rt
{

//INLFUNC void Randomize( DWORD seed = time(NULL) ){ srand(seed); }

template<typename T>
FORCEINL T MaxPower2Within(T within)
{
	if(within)
	{
		UINT ret=1;
		while(within!=1)
		{
			ret<<=1;
			within>>=1;
		}

		return ret;
	}
	else
		return 0;
}

template<typename T>
FORCEINL T* _CastToNonconst(const T * p)
{
	return (T*)((size_t)p);
}
} // namespace rt



/////////////////////////////////////////////////////
// switch little-endian/big-endian values, inplace
namespace rt
{
namespace _details
{	
	template<int sz>
	FORCEINL void _SwitchByteOrder(LPBYTE p)
	{	BYTE t = p[0];
		p[0] = p[sz-1];
		p[sz-1] = t;
		_SwitchByteOrder<sz-2>(p+1);
	}
	template<>
	FORCEINL void _SwitchByteOrder<1>(LPBYTE p){}
	template<>
	FORCEINL void _SwitchByteOrder<0>(LPBYTE p){}
} // namespace _details
} // namespace rt

namespace rt
{
	template<typename T>
	FORCEINL void SwitchByteOrder(T& x)
	{	_details::_SwitchByteOrder<sizeof(T)>((LPBYTE)&x);
	}
	template<typename T>
	FORCEINL T ConvertByteOrder(const T& x)
	{	T out = x;
		_details::_SwitchByteOrder<sizeof(T)>((LPBYTE)&out);
		return out;
	}
}

namespace rt
{
namespace _details
{	
	template<int sz>
	FORCEINL void _SwitchByteOrderTo(LPCBYTE f, LPBYTE t)
	{	*t = *f;
 		_SwitchByteOrderTo<sz-1>(f-1, t+1);
	}
	template<>
	FORCEINL void _SwitchByteOrderTo<0>(LPCBYTE f, LPBYTE t){}

	template<size_t sz, int block = sz>=8?8:(sz>=4?4:(sz>=2?2:1)), bool is_small = (sz<=128) >
	struct _CopyByteTo;
		template<size_t sz> struct _CopyByteTo<sz,8,true>
		{	static FORCEINL void Copy(LPCBYTE f, LPBYTE t)
			{	*((ULONGLONG*)t) = *((ULONGLONG*)f);
				_CopyByteTo<sz-8>::Copy(f+8, t+8);
		}	};
		template<size_t sz> struct _CopyByteTo<sz,4,true>
		{	static FORCEINL void Copy(LPCBYTE f, LPBYTE t)
			{	*((DWORD*)t) = *((DWORD*)f);
				_CopyByteTo<sz-4>::Copy(f+4, t+4);
		}	};
		template<size_t sz> struct _CopyByteTo<sz,2,true>
		{	static FORCEINL void Copy(LPCBYTE f, LPBYTE t)
			{	*((WORD*)t) = *((WORD*)f);
				_CopyByteTo<sz-2>::Copy(f+2, t+2);
		}	};
		template<> struct _CopyByteTo<1,1,true>
		{	static FORCEINL void Copy(LPCBYTE f, LPBYTE t)
			{	*((BYTE*)t) = *((BYTE*)f);
		}	};
		template<> struct _CopyByteTo<0, 1,true>
		{	static FORCEINL void Copy(LPCBYTE f, LPBYTE t){}
		};
		template<size_t sz, int block> struct _CopyByteTo<sz, block, false>
		{	static FORCEINL void Copy(LPCBYTE f, LPBYTE t){ memcpy(t, f, sz); }
		};
} // namespace _details
} // namespace rt

namespace rt
{
	template<typename T>
	FORCEINL void SwitchByteOrderTo(const T& from, T& to)
	{	_details::_SwitchByteOrderTo<sizeof(T)>(((LPCBYTE)&from) + sizeof(T) - 1, (LPBYTE)&to);
	}
	template<int SIZE>
	FORCEINL void SwitchByteOrderTo(LPCVOID from, LPVOID to)
	{	_details::_SwitchByteOrderTo<SIZE>(((LPCBYTE)from) + SIZE - 1, (LPBYTE)to);
	}
	template<typename T>
	FORCEINL void CopyByteTo(const T& from, T& to)
	{	_details::_CopyByteTo<sizeof(T)>::Copy((LPCBYTE)&from, (LPBYTE)&to);
	}
	template<int SIZE>
	FORCEINL void CopyByteTo(LPCVOID from, LPVOID to)
	{	_details::_CopyByteTo<SIZE>::Copy((LPCBYTE)from, (LPBYTE)to);
	}
	template<typename T>
	FORCEINL void CopyBits(T& x, T val, T mask)
	{
		ASSERT((val & (~mask)) == 0);
		x = (x&(~mask)) | val;
	}
} // namespace rt


////////////////////////////////////////////////////////////////////////////
// Optimized Object Swap, UNSAFE if the object has self or member pointed pointers
// No additional temporary object, no ctor/dtor called
namespace rt
{
	namespace _details
	{
		template<UINT Len>
		FORCEINL void _Exchange_32AL(LPDWORD obj_a,LPDWORD obj_b)
		{
			DWORD t = *obj_a;
			*obj_a = *obj_b;
			*obj_b = t;

			rt::_details::_Exchange_32AL<Len-1>(obj_a+1,obj_b+1);
		}
		template<>
		FORCEINL void _Exchange_32AL<0>(LPDWORD,LPDWORD){} // dummy
	} // namespace _details
} // namespace rt

namespace rt
{
#pragma warning(disable: 4307) //warning C4307: '*' : integral constant overflow
template<typename T>
FORCEINL void Swap(T& a, T& b)
{	
	rt::_details::_Exchange_32AL<sizeof(T)/4> ((LPDWORD)&a, (LPDWORD)&b);
	if( (sizeof(T)&0x1) == 0x1 )
	{	DWORD t;
		t = ((LPBYTE)&a)[sizeof(T)-1];
		((LPBYTE)&a)[sizeof(T)-1] = ((LPBYTE)&b)[sizeof(T)-1];
		((LPBYTE)&b)[sizeof(T)-1] = (BYTE)t;
	}
	if( (sizeof(T)&0x2) == 0x2 )
	{	DWORD t;
		t = ((LPWORD)&a)[sizeof(T)/2-1];
		((LPWORD)&a)[sizeof(T)/2-1] = ((LPWORD)&b)[sizeof(T)/2-1];
		((LPWORD)&b)[sizeof(T)/2-1] = (WORD)t;
	}
}
#pragma warning(default: 4307) //warning C4307: '*' : integral constant overflow

} // namespace rt


namespace rt
{

namespace _details
{
template<class T, typename t_Val, class t_GetKey>	// T should have operator [], and the element should have operator <
INLFUNC SIZE_T _LowerBound(const T& arr, SIZE_T len, const t_Val &key)	// first item equal to or greater than that of a specified key
{
	if(len == 0 || !(t_GetKey::Key(arr[0]) < key))return 0;
	if( !(t_GetKey::Key(arr[len - 1]) < key) ){}
	else{ return len; }

	SIZE_T ret = len - 1;
	SIZE_T L = 0;
	while ( L + 1 < ret )
	{	SIZE_T mid = (L + ret) >> 1;
		if ( t_GetKey::Key(arr[mid]) < key )
			L = mid;
		else
			ret = mid;					
	}
	return ret;
}

template<class T, typename t_Val, class t_GetKey>
INLFUNC SIZE_T _BinarySearch(const T& arr, SIZE_T len, const t_Val &key)	// first item equal to key, or length of arr
{
	SIZE_T r = _LowerBound<T,t_Val,t_GetKey>(arr,len,key);
	if(r<len && key < t_GetKey::Key(arr[r]))return len;
	return r;
}

struct _TrivialGetKey
{	template<typename T> static FORCEINL T& Key(T& x){ return x; }
};

} // namespace _details

template<class t_GetKey, class T, typename t_Val>	// T should have operator [], and the element should have operator <
FORCEINL SIZE_T LowerBound(const T& arr, SIZE_T len, const t_Val &key)	// first item equal to or greater than that of a specified key
{	return _details::_LowerBound<T,t_Val,t_GetKey>(arr,len,key);
}

template<class t_GetKey, class T, typename t_Val>
FORCEINL SIZE_T BinarySearch(const T& arr, SIZE_T len, const t_Val &key)	// first item equal to key, or length of arr
{	return _details::_BinarySearch<T,t_Val,t_GetKey>(arr,len,key);
}

template<class T, typename t_Val>	// T should have operator [], and the element should have operator <
FORCEINL SIZE_T LowerBound(const T& arr, SIZE_T len, const t_Val &key)	// first item equal to or greater than that of a specified key
{	return _details::_LowerBound<T,t_Val,_details::_TrivialGetKey>(arr,len,key);
}

template<class T, typename t_Val>
FORCEINL SIZE_T BinarySearch(const T& arr, SIZE_T len, const t_Val &key)	// first item equal to key, or length of arr
{	return _details::_BinarySearch<T,t_Val,_details::_TrivialGetKey>(arr,len,key);
}



template<class T>
class ObjRef
{	
protected:
	T*	_p;
public:
	typedef T	t_Obj;
	INLFUNC ObjRef(T*x):_p(x){}
	INLFUNC ObjRef(){ Empty(); }
	INLFUNC T* operator ->(){ return _p; }
	INLFUNC const T* operator ->() const { return _p; }
	INLFUNC operator T& () { return *_p; }
	INLFUNC operator const T& () const { return *_p; }
	INLFUNC operator T* () { return _p; }
	INLFUNC operator const T* () const { return _p; }

	INLFUNC T* operator = (T*x){ _p = x; return x; }

	INLFUNC bool	IsEmpty() const { return _p == NULL; }  
	INLFUNC void	Empty(){ _p=NULL; }
	INLFUNC void	SafeDel(){ _SafeDel(_p); }
	INLFUNC void	SafeRelease(){ _SafeRelease(_p); }
};


// simple pseudo-random number generator invented by George Marsaglia.
// http://en.wikipedia.org/wiki/Random_number_generation
class Randomizer
{
	UINT m_z, m_w;
public:
	INLFUNC Randomizer(UINT seed = 17)
	{	if(seed==0 || seed==0x464fffff || (seed*seed)==0x9068ffff)seed+=0x52897528;
		m_w = seed;
		m_z = seed*seed;
	}
	FORCEINL UINT GetNext()
	{	m_z = 36969 * (m_z & 65535) + (m_z >> 16);
		m_w = 18000 * (m_w & 65535) + (m_w >> 16);
		return (m_z << 16) + m_w;
	}
	template<typename T>
	FORCEINL void Randomize(T& pod)
	{	Randomize(&pod, sizeof(T));
	}
	FORCEINL void Randomize(LPVOID p_in, SIZE_T sz)
	{	UINT* p = (UINT*)p_in;
		for(UINT i=0;i<sz/sizeof(UINT);i++)
			p[i] = GetNext();
		if(sz&3)
		{	BYTE num[4];
			*((UINT*)num) = GetNext();
			LPBYTE t = ((LPBYTE)p_in) + (sz&(~3));
			LPBYTE tend = ((LPBYTE)p_in) + sz;
			for(SIZE_T i=0;t<tend;t++, i++)
				*t = num[i];
		}
	}
	FORCEINL operator UINT(){ return GetNext(); }
};


} // namespace rt
////////////////////////////////////////////////////////////////////////////

