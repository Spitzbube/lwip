/*
*****************************************************************************
**
** \file        ./subsystems/net/lwip/src/arch/sys_arch.c
**
** \version     $Id: sys_arch.c 9171 2012-04-25 15:03:00Z jrende $
**
** \brief       system-specific service functions for lwIP
**
** This file contains system-specific functions to implement lwip
** on FAPI with RTOS.
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

#include "lwip/arch/cc.h"
#include "lwip/sys.h"
#include "lwip/def.h"
#include "lwip/arch/sys_arch.h"
#if 0
#include "rtos/rtos.h"
#include "fapi/sys_services.h"
#include "fapi/drv_timer.h"
#endif
#include "ucos_ii.h"


uint8_t Data_238ce28f[20*44/*size???*/]; //238ce28f, 238CE28C
uint32_t Data_238ce600[6][800]; //238ce600 -> 238D3100
int Data_238d3100; //238d3100
int Data_238d3104; //238d3104
int Data_238d3108; //238d3108

extern int Data_234911e0; //234911e0

//2349439c
uint8_t bData_2349439c = 0; //2349439c +0
int/*OS_MEM* */ Data_234943a8 = 0; //234943a8 +0xc

#if 0

RTOS_MailboxT alive_mbox = NULL;

static uint32_t time_interval(uint64_t ts_start, uint64_t ts_end)
{
    if(ts_start <= ts_end) {
        return (uint32_t)(ts_end - ts_start);
    } else {
        return (uint32_t)(0xFFFFFFFFFFFFFFFF - ts_start + ts_end + 1);
    }
}

#endif

/*
 * void sys_init(void)
 *
 * Is called to initialize the sys_arch layer.
 */
/* 2346bbae / 2347eb82 - todo */
void sys_init(void)
{
#if 0
    alive_mbox = RTOS_CreateMailbox(RTOS_MSG_IS_POINTER);
#endif

    uint8_t sp;

    Data_234943a8 = OSMemCreate((uint32_t)&Data_238ce28f & ~3, 20, 44, &sp);

    bData_2349439c = 0;

    Data_238d3100 = 0xdeadbeef;
    Data_238d3104 = 0x7ce538a8;
    Data_238d3108 = 0x1b1cb261;

    //TODO!!!

	console_send_string("sys_init (sys_arch.c): TODO\r\n");

}

#if 0

void tcpip_thread_alive()
{
    (void)RTOS_SetMailbox(alive_mbox, NULL, RTOS_NO_SUSPEND);
}


/* Semaphore functions: */

#endif

/** Create a new semaphore
 * @param sem pointer to the semaphore to create
 * @param count initial count of the semaphore
 * @return ERR_OK if successful, another err_t otherwise 
 */
/* 2346bb42 - complete */
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
  sys_sem_t new_sem;

#if 0
  new_sem = (sys_sem_t)RTOS_CreateSemaphore((uint32_t)count);

  if (new_sem == NULL) {
#if SYS_DEBUG
    LWIP_DEBUGF(SYS_DEBUG, ("sys_sem_new: can't alloc sem\n"));
#endif
    return ERR_MEM;
  } 
#else
  new_sem = OSSemCreate(count);
  *sem = new_sem;
#endif
  return ERR_OK;
}

/** Signals a semaphore
 * @param sem the semaphore to signal 
 */
/* 2346bb96 - complete */
void sys_sem_signal(sys_sem_t *sem)
{
#if 0
  RTOS_SetSemaphore((RTOS_SemaphoreT)(*sem), RTOS_SUSPEND);
#else
  OSSemPost(*sem);
#endif
}

/** Wait for a semaphore for the specified timeout
 * @param sem the semaphore to wait for
 * @param timeout timeout in milliseconds to wait (0 = wait forever)
 * @return time (in milliseconds) waited for the semaphore
 *         or SYS_ARCH_TIMEOUT on timeout 
 */
/* 2346bb52 - complete */
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
#if 0
  uint32_t retval;
  uint64_t ts_start;
  uint64_t ts_end;

  ts_start = FAPI_TIMER_GetTimeStamp(FAPI_TIMER_RESOLUTION_1_MSEC);

  if (timeout == 0) {
    retval = RTOS_GetSemaphore((RTOS_SemaphoreT) (*sem), RTOS_SUSPEND);
  }
  else {
    retval = RTOS_GetSemaphore((RTOS_SemaphoreT) (*sem), (uint32_t)timeout);
  }

  ts_end = FAPI_TIMER_GetTimeStamp(FAPI_TIMER_RESOLUTION_1_MSEC);

  if (retval == 0) {
    LWIP_DEBUGF(SYS_DEBUG, ("sys_arch_sem_wait: error\n"));
    /* We can't return anything but TIMEOUT in case of error. */
    return SYS_ARCH_TIMEOUT;
  } 
  else {
    return time_interval(ts_start, ts_end);
  }
#else
  uint8_t sp;
  uint32_t r0;

  if (timeout != 0)
  {
	  r0 = (timeout * 100) / 1000;
	  if (r0 == 0)
	  {
		  r0 = 1;
	  }
	  else if (r0 > 0xffff)
	  {
		  r0 = 0xffff;
	  }
  }
  else
  {
	  r0 = 0;
  }

  OSSemPend(*sem, (uint16_t)r0, &sp);
  if (sp == 10)
  {
	  return -1;
  }

  return 1;
#endif
}

/** Delete a semaphore
 * @param sem semaphore to delete 
 */
/* 2346bba0 - complete */
void sys_sem_free(sys_sem_t *sem)
{
#if 0
  RTOS_DestroySemaphore((RTOS_SemaphoreT)(*sem));
#else
  uint8_t sp;
  OSSemDel(*sem, 0, &sp);
#endif
}

/** Check if a sempahore is valid/allocated: return 1 for valid, 0 for invalid */
int sys_sem_valid(sys_sem_t *sem)
{
    return (*sem != NULL);
}

/** Set a semaphore invalid so that sys_sem_valid returns 0 */
void sys_sem_set_invalid(sys_sem_t *sem)
{
    *sem = NULL;
}

/* Mailbox functions. */


/** Create a new mbox of specified size
 * @param mbox pointer to the mbox to create
 * @param size (miminum) number of messages in this mbox
 * @return ERR_OK if successful, another err_t otherwise 
 */
/* 2346ba58 - complete */
err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
  /* FAPI_SYS_HandleT is a typedef of void *. */
  sys_mbox_t new_mbox;

#if 0
  new_mbox = (sys_mbox_t)RTOS_CreateMailqueue(size, RTOS_MSG_IS_POINTER);
  if (new_mbox == NULL) {
#if SYS_DEBUG
    LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_new: can't alloc mbox\n"));
#endif
    return ERR_MEM;
  } 
  *mbox = new_mbox;
  return ERR_OK;
#else
  uint8_t sp;
  sys_mbox_t r4 = (void*) OSMemGet(Data_234943a8, &sp);
#if 0
	{
		extern char debug_string[];
		sprintf(debug_string, "sys_mbox_new: Data_234943a8=0x%p, r4=0x%p, sp=%d\r\n",
				Data_234943a8, r4, sp);
		console_send_string(debug_string);
	}
#endif
  if (sp == 0)
  {
	  r4->pQueue = (void*) OSQCreate(&r4->Data_4, 10);
#if 0
	{
		extern char debug_string[];
		sprintf(debug_string, "sys_mbox_new: r4->pQueue=0x%p\r\n", r4->pQueue);
		console_send_string(debug_string);
	}
#endif
	  if (r4->pQueue != 0)
	  {
		  *mbox = r4;
		  return ERR_OK;
	  }
  }
  return ERR_MEM;
#endif
}

/** Post a message to an mbox - may not fail
 * -> blocks if full, only used from tasks not from ISR
 * @param mbox mbox to posts the message
 * @param msg message to post (ATTENTION: can be NULL) 
 */
/* 2346baae - complete */
void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
#if 0
  RTOS_LockScheduler( );
  RTOS_SetMailqueue((RTOS_MailqueueT)(*mbox), msg, 0, RTOS_SUSPEND);
  RTOS_UnlockScheduler( );
#endif

  sys_mbox_t r0 = *mbox;

  if (msg == NULL)
  {
	  msg = &Data_234911e0;
  }

  OSQPost(r0->pQueue, msg);
}

/** Try to post a message to an mbox - may fail if full or ISR
 * @param mbox mbox to posts the message
 * @param msg message to post (ATTENTION: can be NULL) 
 * @return ERR_MEM if mbox is full, else, ERR_OK if the "msg" is posted. 
 */
/* ******************************************* */
/* Enable profiling for the following function */
/* ******************************************* */
#if 0
#undef PROF_TS_ENABLED
#define PROF_TS_ENABLED 0
#undef PROF_C_ENABLED
#define PROF_C_ENABLED 0
#include <profiling.h>
#endif
 /* ******************************************* */
/* 2346bac0 - complete */
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
#if 0
  uint32_t retval;
  PROF_TS_INIT(0, 1);

  PROF_TS_BEGIN();

  RTOS_LockScheduler( );
  retval = RTOS_SetMailqueue((RTOS_MailqueueT)(*mbox), msg, 0, RTOS_NO_SUSPEND);
  RTOS_UnlockScheduler( );

  PROF_TS_END(1);
  PROF_TS_GVAR_ACCUMULATE_TIME(sys_mbox_trypost);

  if (retval == 0) {
    PROF_C_GVAR_INC(post_fail);
    return ERR_MEM;
  }
  else {
    PROF_C_GVAR_INC(post_success);
    return ERR_OK;
  }
#else
  sys_mbox_t r0 = *mbox;

  if (0 != OSQPost(r0->pQueue, msg))
  {
	  return -1;
  }

  return 0;
#endif
}

/** Wait for a new message to arrive in the mbox
 * @param mbox mbox to get a message from
 * @param msg pointer where the message is stored
 * @param timeout maximum time (in milliseconds) to wait for a message
 * @return time (in milliseconds) waited for a message, may be 0 if not waited
 *         or SYS_ARCH_TIMEOUT on timeout
 *         The returned time has to be accurate to prevent timer jitter! 
 */
/* 2346bad4 - complete */
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
#if 0
  uint32_t retval;
  uint64_t ts_start;
  uint64_t ts_end;

  ts_start = FAPI_TIMER_GetTimeStamp(FAPI_TIMER_RESOLUTION_1_MSEC);

  if (timeout == 0) {
    retval = RTOS_GetMailqueue((RTOS_MailqueueT) (*mbox), msg, RTOS_SUSPEND);
  }
  else {
    retval = RTOS_GetMailqueue((RTOS_MailqueueT) (*mbox), msg, (uint32_t) timeout);
  }

  ts_end = FAPI_TIMER_GetTimeStamp(FAPI_TIMER_RESOLUTION_1_MSEC);

  if (retval == 0) {
    /* We can't return anything but TIMEOUT in case of error. */
    LWIP_DEBUGF(SYS_DEBUG, ("sys_arch_mbox_fetch(%d): error %d\n", timeout, RTOS_GetLastError()));
    return SYS_ARCH_TIMEOUT;
  } 
  else {
    return time_interval(ts_start, ts_end);
  }
#else
  uint8_t sp;
  uint32_t r0;

  sys_mbox_t r5 = *mbox;

  if (timeout != 0)
  {
	  r0 = (timeout * 100) / 1000;
	  if (r0 == 0)
	  {
		  r0 = 1;
	  }
	  else if (r0 > 0xffff)
	  {
		  r0 = 0xffff;
	  }
  }
  else
  {
	  r0 = 0;
  }

  void* r0_ = (void*) OSQPend(r5->pQueue, (uint16_t)r0, &sp);

#if 0
	{
		extern char debug_string[];
		sprintf(debug_string, "sys_arch_mbox_fetch: r5->pQueue=0x%p, r0_=0x%p, sp=%d\r\n",
				r5->pQueue, r0_, sp);
		console_send_string(debug_string);
	}
#endif

  if (msg != 0)
  {
	  if (r0_ == &Data_234911e0)
	  {
		  r0_ = 0;
	  }
	  //loc_2346bb14
	  *msg = r0_;
  }
  //loc_2346bb16
  if (sp == 10)
  {
	  return -1;
  }
  //loc_2346bb24
  return 1;
#endif
}


/** Wait for a new message to arrive in the mbox
 * @param mbox mbox to get a message from
 * @param msg pointer where the message is stored
 * @param timeout maximum time (in milliseconds) to wait for a message
 * @return 0 (milliseconds) if a message has been received
 *         or SYS_MBOX_EMPTY if the mailbox is empty 
 */
/* 2346bb28 - complete */
u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
#if 0
  uint32_t retval;

  retval = RTOS_GetMailqueue((RTOS_MailqueueT)(*mbox), msg, RTOS_NO_SUSPEND);

  if (retval == 0) {
    return SYS_MBOX_EMPTY;
  } else {
    return 0;
  }
#endif

  sys_mbox_t r0 = *mbox;
  *msg = (void*) OSQAccept(r0->pQueue);
  if (*msg == 0)
  {
	  return -1;
  }

  return 0;
}


/** Delete an mbox
 * @param mbox mbox to delete 
 */
/* 2346ba8a - todo */
void sys_mbox_free(sys_mbox_t *mbox)
{
//  RTOS_DestroyMailqueue((RTOS_MailqueueT)(*mbox));

#if 0
	console_send_string("sys_mbox_free (todo.c): TODO\r\n");
#endif

	uint8_t sp;

	sys_mbox_t r4 = *mbox;

	OSQFlush(r4->pQueue);

	OSQDel(r4->pQueue, 0, &sp);

	sp = sub_23463b0c(Data_234943a8, *mbox);
}

/** Check if an mbox is valid/allocated: return 1 for valid, 0 for invalid */
/* 2346bc5a - complete */
int sys_mbox_valid(sys_mbox_t *mbox)
{
    return (*mbox != NULL);
}


/* 2346bc6e - complete */
void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
    *mbox = NULL;
}


/*********************************************************************
 * If threads are supported by the underlying operating system and if
 * such functionality is needed in lwIP, the following function will have
 * to be implemented as well:
 *********************************************************************
 */


/** The only thread function:
 * Creates a new thread
 * @param name human-readable name for the thread (used for debugging purposes)
 * @param thread thread-function
 * @param arg parameter passed to 'thread'
 * @param stacksize stack size in bytes for the new thread (may be ignored by ports)
 * @param prio priority of the new thread (may be ignored by ports) 
 * sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio);
 */
/* 2346bbfa - todo */
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
#if 0
  uint32_t stacksize_int;
  sys_thread_t new_thread;

  stacksize_int = (stacksize + sizeof(uint32_t) - 1)/sizeof(uint32_t);

  new_thread = (sys_thread_t)RTOS_CreateThread(NULL, stacksize_int,
						prio, thread, arg, NULL, name);
  if (new_thread == NULL) {
    LWIP_DEBUGF(SYS_DEBUG, ("sys_thread_new: can't alloc thread\n"));
  }
  return new_thread;
#else
  if (bData_2349439c >= 6)
  {
	  return 0;
  }

  bData_2349439c++;

  int r0 = OSTaskCreateExt(thread, arg, Data_238ce600[bData_2349439c] - 1,
		  (uint8_t)prio, (uint16_t)prio, Data_238ce600[bData_2349439c] - 800, 800, 0, 3);
  if (r0 != 0)
  {
	  bData_2349439c--;

	  while (1)
	  {
		  /*loc_2346bc56*/;
	  }
  }
  //loc_2346bc58
  return bData_2349439c;
#endif
}

/*
 * sys_prot_t sys_arch_protect(void)
 *
 * This optional function does a "fast" critical region protection and returns
 * the previous protection level. This function is only called during very short
 * critical regions. An embedded system which supports ISR-based drivers might
 * want to implement this function by disabling interrupts. Task-based systems
 * might want to implement this by using a mutex or disabling tasking. This
 * function should support recursive calls from the same task or interrupt. In
 * other words, sys_arch_protect() could be called while already protected. In
 * that case the return value indicates that it is already protected.
 *
 * sys_arch_protect() is only required if your port is supporting an operating
 * system.
 */
sys_prot_t sys_arch_protect(void)
{
//  return (sys_prot_t)RTOS_EnterCriticalSection();
}

/*
 * void sys_arch_unprotect(sys_prot_t pval)
 *
 * This optional function does a "fast" set of critical region protection to the
 * value specified by pval. See the documentation for sys_arch_protect() for
 * more information. This function is only required if your port is supporting
 * an operating system.
 */
void sys_arch_unprotect(sys_prot_t pval)
{
//  RTOS_LeaveCriticalSection((uint32_t)pval);
}


#if NOT_IMPLEMENTED
/*
 * Temporally here.
 * the global variable, RTOS_local_errno_addr, should be updated
 * and set to the address of dedicated (thread-local) errno space
 * on every context switch.
 */
#if ERRNO
int *__aeabi_errno_addr(void)
{
  return RTOS_local_errno_addr;
}
#endif /* ERRNO */
#endif /* NOT_IMPLEMENTED */
