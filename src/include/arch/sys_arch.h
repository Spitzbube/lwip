/*
*****************************************************************************
**
** \file        ./subsystems/net/lwip/src/include/arch/sys_arch.h
** 
** \version     $Id: sys_arch.h 9174 2012-04-25 15:15:42Z jrende $
**
** \brief       A header file for system-specific service functions
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

#ifndef __ARCH_SYS_ARCH_H__
#define __ARCH_SYS_ARCH_H__

#include <stdint.h>
#if 0
#include <stdchar.h>
#include "rtos/rtos.h"
#include "lwip/sys.h"
#endif

#if 0
typedef RTOS_SemaphoreT sys_sem_t;
#else
typedef struct os_event* sys_sem_t;
#endif
#if 0
typedef RTOS_MailqueueT sys_mbox_t;
#else
typedef struct
{
	 struct os_event* pQueue; //0
	 int Data_4; //4

}* sys_mbox_t;
#endif
#if 0
typedef RTOS_ThreadT    sys_thread_t;
#else
typedef uint8_t         sys_thread_t;
#endif
typedef uint32_t        sys_prot_t;

#if 0
/*
 * Used in api_lib.c, api_msg.c and tcpip.c
 */
#define SYS_MBOX_NULL    NULL
#define SYS_SEM_NULL     NULL

#endif

#define LWIP_COMPAT_MUTEX 1

#if 0

/*
 * depth of mailqueue created by RTOS as a sys_mbox_t
 */
#define SYS_MAILQ_DEPTH  10

#define SYS_THREAD_OPTSIZE  (sizeof(struct sys_timeouts))
extern void *sys_thread_opt(void);
#endif

#endif /* __ARCH_SYS_ARCH_H__ */
