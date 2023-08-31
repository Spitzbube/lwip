/*
*****************************************************************************
**
** \file        ./subsystems/net/lwip/src/include/arch/cc.h
**
** \version     $Id: cc.h 9174 2012-04-25 15:15:42Z jrende $
**
** \brief       Compiler-specific definitions
**
** \attention   THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
**              ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
**              OMMISSIONS.
**
** (C) Copyright 2006-2010 by Fujitsu Microelectronics Europe GmbH
** (C) Copyright 2010-2012 by Fujitsu Semiconductor Europe GmbH
**
*****************************************************************************
*/

#ifndef __ARCH_CC_H__
#define __ARCH_CC_H__

#include "fapi/sys_services.h"

typedef unsigned char   u8_t;
typedef signed char     s8_t;
typedef unsigned short  u16_t;
typedef signed short    s16_t;
typedef unsigned long   u32_t;
typedef signed long     s32_t;
typedef unsigned int    mem_ptr_t;

/*
 * printf-like format specifiers used in LWIP_DEBUGF
 */
#define U16_F "u"
#define S16_F "d"
#define X16_F "x"
#define U32_F "lu"
#define S32_F "ld"
#define X32_F "lx"

#if defined(__arm__) && !defined(__GNUC__) /* RVCT v3- */
#define PACK_STRUCT_FIELD(x) x
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_BEGIN __packed
#define PACK_STRUCT_END
#else /* GNU CC */
#define PACK_STRUCT_FIELD(x) x
#define PACK_STRUCT_STRUCT __attribute__((packed))
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_END
#endif

/*
 * It's funny, but works fine anyway.
 *#define LWIP_PLATFORM_DIAG(x)    FAPI_SYS_PRINT_MSG(x)
 */
#if 0
#define LWIP_PLATFORM_DIAG(x)    FAPI_SYS_PRINT_MSG x
#else
#define PRINT_MSG(...) \
        {                                                        \
			uint8_t buf[0x100];                                  \
			snprintf(buf, 0x100, ##__VA_ARGS__);                   \
        }
#define LWIP_PLATFORM_DIAG(x) PRINT_MSG x
#endif


/* FUJITSU(hkoblm) begin */
#define LWIP_PLATFORM_ASSERT(x) \
  do {                          \
    FAPI_SYS_PRINT_MSG(x);      \
    while(0);                   \
  } while(0)
/* FUJITSU(hkoblm) end */
/* #define LWIP_PLATFORM_ASSERT(x)  __breakpoint(1) */

#endif /* __ARCH_CC_H__ */
