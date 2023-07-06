//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    エンディアン
//
//-------------------------------------------------------------------

#pragma once

#ifdef __GNUC__  // GCC
#    if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#        define UDON_BIG_ENDIAN
#    elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#        define UDON_LITTLE_ENDIAN
#    else
#        error "Unknown Endian"
#    endif
#elif defined(_MSC_VER)  // MSVC
#    if defined(_M_IX86) || defined(_M_X64) || defined(_M_IA64) || defined(_M_ARM) || defined(_M_ARM64) || defined(_M_AMD64) || defined(_M_MRX000) || defined(_M_PPC) || defined(_M_ALPHA) || defined(_M_SH)
#        define UDON_LITTLE_ENDIAN
#    elif defined(_MIPSEB) || defined(_POWER) || defined(__s390__)
#        define UDON_BIG_ENDIAN
#    else
#        error "Unknown Endian"
#    endif
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#    define UDON_BIG_ENDIAN
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#    define UDON_LITTLE_ENDIAN
#elif defined(__sparc) || defined(__sparc__) || defined(_POWER) || defined(__powerpc__) || defined(__ppc__) || defined(__hpux) || defined(_MIPSEB) || defined(_POWER) || defined(__s390__)
#    define UDON_BIG_ENDIAN
#else
#    error "Unknown Endian"
#endif

